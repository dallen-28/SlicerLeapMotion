/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// GestureRecognition Logic includes
#include "vtkSlicerGestureRecognitionLogic.h"

// MRML includes
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>

// STD includes
#include <cassert>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerGestureRecognitionLogic);

// Callback function for transform modified event
static void func(vtkObject *caller, unsigned long eid, void *clientdata, void *calldata);
static double* RotMatrixToEulerAngles(vtkMatrix4x4 *R);
static GRT::ANBC anbcModel;
static GRT::DecisionTree decisionTreeModel;
static GRT::AdaBoost adaBoostModel;
static GRT::KNN knnModel;


//----------------------------------------------------------------------------
vtkSlicerGestureRecognitionLogic::vtkSlicerGestureRecognitionLogic()
{
  this->transformModifiedCallback = vtkSmartPointer<vtkCallbackCommand>::New();
  this->transformModifiedCallback->SetCallback(func);
 
}

//----------------------------------------------------------------------------
vtkSlicerGestureRecognitionLogic::~vtkSlicerGestureRecognitionLogic()
{
}

//----------------------------------------------------------------------------
void vtkSlicerGestureRecognitionLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkSlicerGestureRecognitionLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}
void vtkSlicerGestureRecognitionLogic::StartPrediction(vtkMRMLLinearTransformNode* transformNode)
{

  if(!decisionTreeModel.load("C:\\GRT\\grt-bin\\Debug\\DecisionTreeModel.grt"))
  {
	vtkErrorMacro("Failed to load Decision Tree Model");
  }
  
  if (!adaBoostModel.load("C:\\GRT\\grt-bin\\Debug\\AdaBoostModel.grt"))
  {
	vtkErrorMacro("Failed to load AdaBoost Model");
  }

  // Load Adaptive Naive Bayes Classifier Model
  if (!anbcModel.load("C:\\GRT\\grt-bin\\Debug\\anbcModel.grt"))
  {
	vtkErrorMacro("Failed to load ANBC Model");
  }
  if (!knnModel.load("C:\\GRT\\grt-bin\\Debug\\kNNModel.grt"))
  {
	vtkErrorMacro("Failed to load KNN Model");
  }

  // Attach observer to transformNode
  transformNode->AddObserver(vtkMRMLLinearTransformNode::TransformModifiedEvent, this->transformModifiedCallback);

}

static void func(vtkObject *caller, unsigned long eid, void* clientdata, void *calldata)
{
  vtkMRMLLinearTransformNode* node = static_cast<vtkMRMLLinearTransformNode*>(caller);
  vtkNew<vtkMatrix4x4> matr;
  node->GetMatrixTransformFromParent(matr);
  double *ang = RotMatrixToEulerAngles(matr);
  //double *ang = RotMatrixToEulerAngles(matr);

  GRT::VectorFloat inputVector(3);
  inputVector.at(0) = ang[0];
  inputVector.at(1) = ang[1];
  inputVector.at(2) = ang[2];

  //-0.373671739183 - 0.0542107947893 - 0.0389651794363
  //inputVector.at(0) = -0.373671739183;
  //inputVector.at(1) = -0.0542107947893;
  //inputVector.at(2) = -0.0389651794363;

  /*if (!anbcModel.predict(inputVector))
  {
	std::cout << "Failed to perform prediction\n";
	return;
  }*/

  if (!decisionTreeModel.predict(inputVector))
  {
	std::cout << "Failed to perform prediction\n";
	return;
  }
  /*if (!adaBoostModel.predict(inputVector))
  {
	std::cout << "Failed to perform prediction\n";
	return;
  }*/
  /*if (!knnModel.predict(inputVector))
  {
	std::cout << "Failed to perform prediction\n";
	return;
  }*/

  //UINT predictedClass = anbcModel.getPredictedClassLabel();
  UINT predictedClass = decisionTreeModel.getPredictedClassLabel();
  //UINT predictedClass = adaBoostModel.getPredictedClassLabel();
  //UINT predictedClass = knnModel.getPredictedClassLabel();

  // If we have predicted a valid gesure fire an event
  if (predictedClass > 0)
  {
	node->InvokeEvent(vtkSlicerGestureRecognitionLogic::GestureRecognizedEvent, &predictedClass);
  }

}
static double* RotMatrixToEulerAngles(vtkMatrix4x4* R)
{
  double* a = new double[3];

  float sy = sqrt(R->Element[0][0] * R->Element[0][0] + R->Element[1][0] * R->Element[1][0]);

  bool singular = sy < 1e-6;

  float x, y, z;
  if (!singular)
  {
	x = atan2(R->Element[2][1], R->Element[2][2]);
	y = atan2(-R->Element[2][0], sy);
	z = atan2(R->Element[1][0], R->Element[0][0]);
  }
  else
  {
	x = atan2(-R->Element[1][2], R->Element[1][1]);
	y = atan2(-R->Element[2][0], sy);
	z = 0;
  }

  // Convert Radian to degrees (normalized)
  a[0] = (x/PI);
  a[1] = (y/PI);
  a[2] = (z/PI);


  //std::cout << a[0] << ", " << a[1] << ", " << a[2] << std::endl;

  return a;
}

//-----------------------------------------------------------------------------
void vtkSlicerGestureRecognitionLogic::RegisterNodes()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerGestureRecognitionLogic::UpdateFromMRMLScene()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerGestureRecognitionLogic
::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicerGestureRecognitionLogic
::OnMRMLSceneNodeRemoved(vtkMRMLNode* vtkNotUsed(node))
{
}

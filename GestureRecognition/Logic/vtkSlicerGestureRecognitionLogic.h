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

// .NAME vtkSlicerGestureRecognitionLogic - slicer logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkSlicerGestureRecognitionLogic_h
#define __vtkSlicerGestureRecognitionLogic_h

// Slicer includes
#include "vtkSlicerModuleLogic.h"

// MRML includes
#include "vtkMRMLLinearTransformNode.h"

// STD includes
#include <cstdlib>

// GRT includes
#include "GRT.h"
#include "vtkMatrix4x4.h"

#include "vtkSlicerGestureRecognitionModuleLogicExport.h"
#include "vtkCallbackCommand.h"


/// \ingroup Slicer_QtModules_ExtensionTemplate
class VTK_SLICER_GESTURERECOGNITION_MODULE_LOGIC_EXPORT vtkSlicerGestureRecognitionLogic :
  public vtkSlicerModuleLogic
{
public:

  enum
  {
	GestureRecognizedEvent = 180023918
  };

  static vtkSlicerGestureRecognitionLogic *New();
  vtkTypeMacro(vtkSlicerGestureRecognitionLogic, vtkSlicerModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent);

  void StartPrediction(vtkMRMLLinearTransformNode *node);

protected:
  vtkSlicerGestureRecognitionLogic();
  virtual ~vtkSlicerGestureRecognitionLogic();

  virtual void SetMRMLSceneInternal(vtkMRMLScene* newScene);
  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  virtual void RegisterNodes();
  virtual void UpdateFromMRMLScene();
  virtual void OnMRMLSceneNodeAdded(vtkMRMLNode* node);
  virtual void OnMRMLSceneNodeRemoved(vtkMRMLNode* node);

  //virtual void Execute(vtkObject *vtkNotUsed(caller), unsigned long eid, void *vtkNotUsed(calldata));


private:

  //static void func(vtkObject *caller, unsigned long eid, void *clientdata, void *calldata);
  vtkSmartPointer<vtkCallbackCommand> transformModifiedCallback;

  vtkSlicerGestureRecognitionLogic(const vtkSlicerGestureRecognitionLogic&); // Not implemented
  void operator=(const vtkSlicerGestureRecognitionLogic&); // Not implemented
};

#endif

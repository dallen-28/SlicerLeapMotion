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

// Qt includes
#include <QDebug>

// SlicerQt includes
#include "qSlicerGestureRecognitionModuleWidget.h"
#include "ui_qSlicerGestureRecognitionModuleWidget.h"

#include "vtkSlicerGestureRecognitionLogic.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerGestureRecognitionModuleWidgetPrivate: public Ui_qSlicerGestureRecognitionModuleWidget
{
	Q_DECLARE_PUBLIC(qSlicerGestureRecognitionModuleWidget);
protected:
	qSlicerGestureRecognitionModuleWidget* const q_ptr;
public:
  qSlicerGestureRecognitionModuleWidgetPrivate(qSlicerGestureRecognitionModuleWidget& object);
	vtkSlicerGestureRecognitionLogic * logic() const;
};

//-----------------------------------------------------------------------------
// qSlicerGestureRecognitionModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerGestureRecognitionModuleWidgetPrivate::qSlicerGestureRecognitionModuleWidgetPrivate(qSlicerGestureRecognitionModuleWidget& object)
	: q_ptr(&object)
{
	
}

vtkSlicerGestureRecognitionLogic* qSlicerGestureRecognitionModuleWidgetPrivate::logic() const
{
	Q_Q(const qSlicerGestureRecognitionModuleWidget);
	return vtkSlicerGestureRecognitionLogic::SafeDownCast(q->logic());
	
}

//-----------------------------------------------------------------------------
// qSlicerGestureRecognitionModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerGestureRecognitionModuleWidget::qSlicerGestureRecognitionModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerGestureRecognitionModuleWidgetPrivate (*this) )
{
}

//-----------------------------------------------------------------------------
qSlicerGestureRecognitionModuleWidget::~qSlicerGestureRecognitionModuleWidget()
{
}

void qSlicerGestureRecognitionModuleWidget::OnPredictButtonClicked()
{
	Q_D(qSlicerGestureRecognitionModuleWidget);

	d->logic()->StartPrediction((vtkMRMLLinearTransformNode*)d->transformDropDown->currentNode());
}

//-----------------------------------------------------------------------------
void qSlicerGestureRecognitionModuleWidget::setup()
{
  Q_D(qSlicerGestureRecognitionModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();


	// Wire up transform nodes to dropdown
	d->transformDropDown->setNodeTypes(QStringList() << (QString)"vtkMRMLLinearTransformNode");
	d->transformDropDown->setSelectNodeUponCreation(true);
	d->transformDropDown->setAddEnabled(false);
	d->transformDropDown->setRemoveEnabled(false);
	d->transformDropDown->setNoneEnabled(false);
	this->setMRMLScene(d->logic()->GetMRMLScene());
	d->transformDropDown->setMRMLScene(this->mrmlScene());
	d->transformDropDown->setToolTip("Pick a transform to observe1");
	

	connect(d->pushButton, SIGNAL(clicked()), this, SLOT(OnPredictButtonClicked()));


}

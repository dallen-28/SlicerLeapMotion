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

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerGestureRecognitionModuleWidgetPrivate: public Ui_qSlicerGestureRecognitionModuleWidget
{
public:
  qSlicerGestureRecognitionModuleWidgetPrivate();
};

//-----------------------------------------------------------------------------
// qSlicerGestureRecognitionModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerGestureRecognitionModuleWidgetPrivate::qSlicerGestureRecognitionModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerGestureRecognitionModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerGestureRecognitionModuleWidget::qSlicerGestureRecognitionModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerGestureRecognitionModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicerGestureRecognitionModuleWidget::~qSlicerGestureRecognitionModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerGestureRecognitionModuleWidget::setup()
{
  Q_D(qSlicerGestureRecognitionModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();
}

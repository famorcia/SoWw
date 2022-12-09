/**************************************************************************\
 * BSD 3-Clause License
 *
 * Copyright (c) 2022, Fabrizio Morciano
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/
#include <Inventor/Ww/viewers/SoWwFullViewer.h>
#include "Inventor/Ww/viewers/SoWwFullViewerP.h"
#include "Inventor/Ww/widgets/SoWwThumbWheel.h"


#define PUBLIC(o) (o->pub)
#define PRIVATE(o) (o->pimpl)

SOWW_OBJECT_ABSTRACT_SOURCE(SoWwFullViewer);

SoWwFullViewer::SoWwFullViewer(wxWindow* parent,
                               const char * name,
                               SbBool embed,
                               BuildFlag flag,
                               Type type,
                               SbBool build): inherited(parent, name, embed, type, FALSE)
{
    PRIVATE(this) = new SoWwFullViewerP(this);

    //TODO: PRIVATE(this)->viewerwidget = NULL;
    //TODO: PRIVATE(this)->canvas = NULL;
//  PRIVATE(this)->canvasparent = NULL;

    // TODO PRIVATE(this)->viewbutton = NULL;
    //TODO: PRIVATE(this)->interactbutton = NULL;

    this->leftDecoration = NULL;
    this->bottomDecoration = NULL;
    this->rightDecoration = NULL;

    this->leftWheel = NULL;
    this->leftWheelLabel = NULL;
    this->leftWheelStr = NULL;
    this->leftWheelVal = 0.0f;

    this->bottomWheel = NULL;
    this->bottomWheelLabel = NULL;
    this->bottomWheelStr = NULL;
    this->bottomWheelVal = 0.0f;

    this->rightWheel = NULL;
    this->rightWheelLabel = NULL;
    this->rightWheelStr = NULL;
    this->rightWheelVal = 0.0f;

    this->setLeftWheelString("Motion X");
    this->setBottomWheelString("Motion Y");
    this->setRightWheelString("Dolly");

    // TODO: PRIVATE(this)->mainlayout = NULL;
    // TODO: PRIVATE(this)->appbuttonlayout = NULL;

    // TODO: PRIVATE(this)->menuenabled = buildFlag & SoWwFullViewer::BUILD_POPUP;
    // TODO: PRIVATE(this)->decorations =            (buildFlag & SoWwFullViewer::BUILD_DECORATION) ? TRUE : FALSE;

    this->prefmenu = NULL;
    PRIVATE(this)->menutitle = "Viewer Menu";

    PRIVATE(this)->viewerbuttons = new SbPList;
    PRIVATE(this)->appbuttonlist = new SbPList;
    // TODO: PRIVATE(this)->appbuttonform = NULL;

    this->setSize(SbVec2s(500, 390));

    if (! build) return;

    this->setClassName("SoWwFullViewer");
    wxWindow * viewer = this->buildWidget(this->getParentWidget());
    this->setBaseWidget(viewer);
}


wxWindow* SoWwFullViewer::buildWidget(wxWindow* parent) {
    // This will build the main view widgets, along with the decorations
    // widgets and popup menu if they are enabled.
#if 0 // TODO
#if SoWw_DEBUG && 0
    SoDebugError::postInfo("SoWwFullViewer::buildWidget", "[invoked]");
#endif

    PRIVATE(this)->viewerwidget = new QWidget(parent);
    this->registerWidget(PRIVATE(this)->viewerwidget);

    PRIVATE(this)->viewerwidget->move(0, 0);

#if SoWw_DEBUG && 0
    PRIVATE(this)->viewerwidget->setBackgroundColor(QColor(250, 0, 0));
#endif

    PRIVATE(this)->canvas = inherited::buildWidget(PRIVATE(this)->viewerwidget);

    QSize s(PRIVATE(this)->viewerwidget->size().width(),
            PRIVATE(this)->viewerwidget->size().height());

    PRIVATE(this)->canvas->resize(s);

    this->buildDecoration( PRIVATE(this)->viewerwidget );
    PRIVATE(this)->showDecorationWidgets( PRIVATE(this)->decorations );

    if (PRIVATE(this)->menuenabled)
        this->buildPopupMenu();

    return PRIVATE(this)->viewerwidget;
#endif
}


void SoWwFullViewer::setDecoration(const SbBool enable){
#if SoWw_DEBUG
    if ((enable  && this->isDecoration()) ||
       (!enable && !this->isDecoration())) {
    SoDebugError::postWarning("SoWwFullViewer::setDecoration",
                              "decorations already turned %s",
                              enable ? "on" : "off");
    return;
  }
#endif // SoWw_DEBUG

    PRIVATE(this)->decorations = enable;
    // TODO: if (PRIVATE(this)->viewerwidget)        PRIVATE(this)->showDecorationWidgets(enable);

}
SbBool SoWwFullViewer::isDecoration(void) const{ }

void SoWwFullViewer::setPopupMenuEnabled(const SbBool on){ }
SbBool SoWwFullViewer::isPopupMenuEnabled(void) const{ }

wxWindow* SoWwFullViewer::getAppPushButtonParent(void) const{ }
void SoWwFullViewer::addAppPushButton(wxWindow* newButton){ }
void SoWwFullViewer::insertAppPushButton(wxWindow* newButton, int index){ }
void SoWwFullViewer::removeAppPushButton(wxWindow* oldButton){ }
int SoWwFullViewer::findAppPushButton(wxWindow* oldButton) const{ }
int SoWwFullViewer::lengthAppPushButton(void) const{ }

wxWindow* SoWwFullViewer::getRenderAreaWidget(void) const{ }

void SoWwFullViewer::setViewing(SbBool on){ }

/*
SoWwFullViewer::SoWwFullViewer(wxWindow* parent,
                               const char * name,
                               SbBool embed,
                               BuildFlag flag,
                               Type type,
                               SbBool build){

}*/

SoWwFullViewer::~SoWwFullViewer(){ }

void SoWwFullViewer::buildDecoration(wxWindow* parent){
    this->leftDecoration = this->buildLeftTrim(parent);
    this->bottomDecoration = this->buildBottomTrim(parent);
    this->rightDecoration = this->buildRightTrim(parent);
}
wxWindow* SoWwFullViewer::buildLeftTrim(wxWindow* parent){
#if 0 // TODO
    wxFrame * w = new wxFrame(parent, wxID_ANY, "");
    w->setFixedWidth(30);

    QGridLayout * gl = new QGridLayout(w);
#if QT_VERSION >= 0x040000
    gl->setContentsMargins(0,0,0,0);
#endif
    gl->addWidget(this->buildAppButtons(w), 0, 0);

    SoWwThumbWheel * t = new SoWwThumbWheel(SoWwThumbWheel::Vertical, w);
    this->leftWheel = t;
    t->setRangeBoundaryHandling(SoWwThumbWheel::ACCUMULATE);
    this->leftWheelVal = t->value();

    QObject::connect(t, SIGNAL(wheelMoved(float)), PRIVATE(this), SLOT(leftWheelChanged(float)));
    QObject::connect(t, SIGNAL(wheelPressed()), PRIVATE(this), SLOT(leftWheelPressed()));
    QObject::connect(t, SIGNAL(wheelReleased()), PRIVATE(this), SLOT(leftWheelReleased()));

    gl->addWidget(t, 1, 0, Qt::AlignBottom | Qt::AlignHCenter);
    gl->activate();

    return w;
#endif
    return 0;
}
wxWindow* SoWwFullViewer::buildBottomTrim(wxWindow* parent){ }
wxWindow* SoWwFullViewer::buildRightTrim(wxWindow* parent){ }
wxWindow* SoWwFullViewer::buildAppButtons(wxWindow* parent){ }
wxWindow* SoWwFullViewer::buildViewerButtons(wxWindow* parent){ }
void SoWwFullViewer::createViewerButtons(wxWindow* parent, SbPList * buttonlist){ }

void SoWwFullViewer::buildPopupMenu(void){ }
void SoWwFullViewer::openPopupMenu(const SbVec2s position){ }

void SoWwFullViewer::setLeftWheelString(const char * const name){ }
void SoWwFullViewer::setBottomWheelString(const char * const name){ }

void SoWwFullViewer::setRightWheelString(const char * const name){ }
const char * SoWwFullViewer::getRightWheelString() const{ }

void SoWwFullViewer::sizeChanged(const SbVec2s & size) {

}

void  SoWwFullViewerP::setThumbWheelValue(wxWindow*, float value) {

}

void  SoWwFullViewerP::seekbuttonClicked()  {

}

SoWwFullViewerP::SoWwFullViewerP(SoWwFullViewer *pViewer) : SoGuiFullViewerP(pViewer) {

}

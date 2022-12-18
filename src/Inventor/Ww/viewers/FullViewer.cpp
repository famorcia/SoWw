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
#include "sowwdefs.h"

#include <wx/stattext.h>
#include <wx/sizer.h>

#define PUBLIC(o) (o->pub)
#define PRIVATE(o) (o->pimpl)

SOWW_OBJECT_ABSTRACT_SOURCE(SoWwFullViewer);

SoWwFullViewer::SoWwFullViewer(wxWindow* parent,
                               const char * name,
                               SbBool embed,
                               BuildFlag buildFlag,
                               Type type,
                               SbBool build)
        : inherited(parent,
                    name,
                    embed,
                    type,
                    FALSE)
{
    PRIVATE(this) = new SoWwFullViewerP(this);

    PRIVATE(this)->viewerwidget = NULL;
    PRIVATE(this)->canvas = NULL;
    // TODO: PRIVATE(this)->canvasparent = NULL;

    PRIVATE(this)->viewbutton = NULL;
    PRIVATE(this)->interactbutton = NULL;

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

    PRIVATE(this)->mainlayout = NULL;
    PRIVATE(this)->appbuttonlayout = NULL;

    PRIVATE(this)->menuenabled = buildFlag & SoWwFullViewer::BUILD_POPUP;
    PRIVATE(this)->decorations = (buildFlag & SoWwFullViewer::BUILD_DECORATION) ? TRUE : FALSE;

    this->prefmenu = NULL;
    PRIVATE(this)->menutitle = "Viewer Menu";

    PRIVATE(this)->viewerbuttons = new SbPList;
    PRIVATE(this)->appbuttonlist = new SbPList;
    PRIVATE(this)->appbuttonform = NULL;

    this->setSize(SbVec2s(500, 390));

    if (! build) return;

    this->setClassName("SoWwFullViewer");
    wxWindow * viewer = this->buildWidget(this->getParentWidget());
    this->setBaseWidget(viewer);
}

wxWindow*
SoWwFullViewer::buildWidget(wxWindow* parent) {
    // This will build the main view widgets, along with the decorations
    // widgets and popup menu if they are enabled.
#if SOWW_DEBUG
    SoDebugError::postInfo("SoWwFullViewer::buildWidget", "[invoked]");
#endif

    wxBoxSizer *main_sizer = new wxBoxSizer(wxHORIZONTAL);
    PRIVATE(this)->viewerwidget = new wxPanel(parent,
                                              wxID_ANY);
#if SOWW_DEBUG
    SoDebugError::postInfo("SoWwFullViewer::buildWidget",
                           "parent size:%d %d",
                           parent->GetSize().GetWidth(),
                           parent->GetSize().GetHeight());
#endif
    main_sizer->Add(PRIVATE(this)->viewerwidget,1,wxEXPAND|wxALL,5);
    parent->SetSizer(main_sizer);
    parent->Layout();
    this->registerWidget(PRIVATE(this)->viewerwidget);
#if SOWW_DEBUG
    SoDebugError::postInfo("SoWwFullViewer::buildWidget",
                           "viewerwidget size:%d %d",
                           PRIVATE(this)->viewerwidget->GetSize().GetWidth(),
                           PRIVATE(this)->viewerwidget->GetSize().GetHeight());
#endif

#if SOWW_DEBUG
    PRIVATE(this)->viewerwidget->SetBackgroundColour(wxColour(250, 0, 0));
#endif

    PRIVATE(this)->canvas = inherited::buildWidget(PRIVATE(this)->viewerwidget);

#if SOWW_DEBUG
    PRIVATE(this)->canvas->SetBackgroundColour(wxColour(250, 0, 0));
#endif

    this->buildDecoration( PRIVATE(this)->viewerwidget );
    PRIVATE(this)->showDecorationWidgets( PRIVATE(this)->decorations );

    if (PRIVATE(this)->menuenabled)
        this->buildPopupMenu();

    return PRIVATE(this)->viewerwidget;
}


void SoWwFullViewer::setDecoration(const SbBool enable){
#if SOWW_DEBUG
    if ((enable  && this->isDecoration()) ||
        (!enable && !this->isDecoration())) {
        SoDebugError::postWarning("SoWwFullViewer::setDecoration",
                                  "decorations already turned %s",
                                  enable ? "on" : "off");
        return;
    }
#endif // SOWW_DEBUG

    PRIVATE(this)->decorations = enable;
    if (PRIVATE(this)->viewerwidget)
        PRIVATE(this)->showDecorationWidgets(enable);
}

SbBool
SoWwFullViewer::isDecoration(void) const{
    SOWW_STUB();
    return (FALSE);
}

void SoWwFullViewer::setPopupMenuEnabled(const SbBool on){
    SOWW_STUB();
}

SbBool
SoWwFullViewer::isPopupMenuEnabled(void) const{
    SOWW_STUB();
    return (FALSE);
}

wxWindow*
SoWwFullViewer::getAppPushButtonParent(void) const {
    SOWW_STUB();
    return (0);
}

void
SoWwFullViewer::addAppPushButton(wxWindow* newButton)  {
    SOWW_STUB();
}

void
SoWwFullViewer::insertAppPushButton(wxWindow* newButton, int index) {
    SOWW_STUB();
}

void
SoWwFullViewer::removeAppPushButton(wxWindow* oldButton) {
    SOWW_STUB();
}

int
SoWwFullViewer::findAppPushButton(wxWindow* oldButton) const {
    SOWW_STUB();
    return (0);
}

int
SoWwFullViewer::lengthAppPushButton(void) const {
    SOWW_STUB();
    return (0);
}

wxWindow*
SoWwFullViewer::getRenderAreaWidget(void) const {
    SOWW_STUB();
    return (0);
}

void
SoWwFullViewer::setViewing(SbBool on) {
    SOWW_STUB();
}

SoWwFullViewer::~SoWwFullViewer() {
    SOWW_STUB();
}

void
SoWwFullViewer::buildDecoration(wxWindow* parent) {
    this->leftDecoration = this->buildLeftTrim(parent);
#if SOWW_DEBUG && 0
    this->leftDecoration->SetBackgroundColour(wxColour(255, 0, 0));
#endif
    this->bottomDecoration = this->buildBottomTrim(parent);
#if SOWW_DEBUG  && 0
    this->bottomDecoration->SetBackgroundColour(wxColour(0, 0, 0));
#endif
    this->rightDecoration = this->buildRightTrim(parent);
#if SOWW_DEBUG  && 0
    this->rightDecoration->SetBackgroundColour(wxColour(0, 0, 255));
#endif
}

wxWindow*
SoWwFullViewer::buildLeftTrim(wxWindow* parent){
    SoWwThumbWheel * t = new SoWwThumbWheel(SoWwThumbWheel::Vertical, parent);
    t->SetSize(40,100);
    this->leftWheel = t;
    t->setRangeBoundaryHandling(SoWwThumbWheel::ACCUMULATE);
    this->leftWheelVal = t->value();
    return t;
}

wxWindow*
SoWwFullViewer::buildBottomTrim(wxWindow* parent) {
    wxWindow * w = new wxPanel(parent);

    wxStaticText* label = new wxStaticText( w, wxID_ANY, this->leftWheelStr);
    this->leftWheelLabel = label;

    label = new wxStaticText( w, wxID_ANY, this->bottomWheelStr ,wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
    this->bottomWheelLabel = label;

    label = new wxStaticText( w, wxID_ANY, this->rightWheelStr, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    this->rightWheelLabel = label;

    SoWwThumbWheel * t = new SoWwThumbWheel(SoWwThumbWheel::Horizontal, w);
    this->bottomWheel = t;
    t->SetSize(200, 30);
    t->setRangeBoundaryHandling(SoWwThumbWheel::ACCUMULATE);

    this->bottomWheelVal = t->value();

    wxBoxSizer* layout = new wxBoxSizer(wxHORIZONTAL);

    layout->Add( this->leftWheelLabel, 0, wxALL, 1 );
    layout->Add(0,0,1, wxEXPAND, 1);
    layout->Add( this->bottomWheelLabel, 0, wxALL, 1 );
    layout->Add( this->bottomWheel, 1, wxEXPAND | wxALL, 1 );
    layout->Add(0,0,1, wxEXPAND, 1);
    layout->Add( this->rightWheelLabel, 1, wxALL, 1 );

    w->SetSizer( layout );
    w->Layout();
    w->Centre( wxBOTH );
    return w;
}

wxWindow*
SoWwFullViewer::buildRightTrim(wxWindow* parent) {
    SoWwThumbWheel * t = new SoWwThumbWheel(SoWwThumbWheel::Vertical, parent);
    t->SetSize(40,100);
    this->rightWheel = t;
    t->setRangeBoundaryHandling(SoWwThumbWheel::ACCUMULATE);
    this->rightWheelVal = t->value();
    return t;
}

wxWindow*
SoWwFullViewer::buildAppButtons(wxWindow* parent) {
    SOWW_STUB();
    return (0);
}

wxWindow*
SoWwFullViewer::buildViewerButtons(wxWindow* parent) {
    SOWW_STUB();
    return (0);
}

void
SoWwFullViewer::createViewerButtons(wxWindow* parent,
                                    SbPList * buttonlist) {
    SOWW_STUB();
}

void
SoWwFullViewer::buildPopupMenu(void) {
    SOWW_STUB();
}

void
SoWwFullViewer::openPopupMenu(const SbVec2s position) {
    SOWW_STUB();
}

void
SoWwFullViewer::setLeftWheelString(const char * const name) {
    delete [] this->leftWheelStr;
    this->leftWheelStr = NULL;

    if (name)
        this->leftWheelStr = strcpy(new char [strlen(name)+1], name);
    if (this->leftWheelLabel)
        this->leftWheelLabel->SetName(name ? name : "");
}

void
SoWwFullViewer::setBottomWheelString(const char * const name) {
    delete [] this->bottomWheelStr;
    this->bottomWheelStr = NULL;

    if (name)
        this->bottomWheelStr = strcpy(new char [strlen(name)+1], name);
    if (this->bottomWheelLabel)
        this->leftWheelLabel->SetName(name ? name : "");

}

void
SoWwFullViewer::setRightWheelString(const char * const name) {
    delete [] this->rightWheelStr;
    this->rightWheelStr = NULL;

    if (name)
        this->rightWheelStr = strcpy(new char [strlen(name)+1], name);
    if (this->rightWheelLabel)
        this->leftWheelLabel->SetName(name ? name : "");
}

short
width(const wxWindow* w) {
    short ret = 0;
    if(w) {
        wxSize size = w->GetSize();
        ret = size.GetX();
    }
    return (ret);
}

short
height(const wxWindow* w) {
    short ret = 0;
    if(w) {
        wxSize size = w->GetSize();
        ret = size.GetY();
    }
    return (ret);
}


void
SoWwFullViewer::sizeChanged(const SbVec2s & size) {
#if SOWW_DEBUG
    SoDebugError::postInfo("SoWwFullViewer::sizeChanged", "(%d, %d)",
                         size[0], size[1]);
#endif

    SbVec2s newsize(size);
    if (PRIVATE(this)->decorations) {
        newsize[0] -= width(this->leftDecoration);
        newsize[0] -= width(this->rightDecoration);
        newsize[1] -= height(this->bottomDecoration);
    }

    newsize = SbVec2s(SoWwMax(newsize[0], (short)1),
                      SoWwMax(newsize[1], (short)1));

    inherited::sizeChanged(newsize);
}

#undef PUBLIC
#undef PRIVATE

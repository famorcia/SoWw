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

#ifndef SOWW_INTERNAL
#error this is a private header file
#endif

#include "Inventor/Ww/viewers/SoWwFullViewerP.h"
#include "Inventor/Ww/viewers/SoWwFullViewer.h"
#include "Inventor/Ww/widgets/WheelEvents.h"
#include "Inventor/Ww/SoWwP.h"
#include "ViewersWxIds.h"
#include "ButtonIndexValues.h"
#include "sowwdefs.h"

#include <wx/gbsizer.h>

const wxWindowID FULL_VIEWER_P = wxID_ANY;

/*
        VIEW_ALL_BUTTON,
*/

wxBEGIN_EVENT_TABLE(SoWwFullViewerP, wxWindow)
                EVT_COMMAND(FULL_VIEWER_P, SO_WW_MOUSE_WHEEL_PRESSED, SoWwFullViewerP::wheelPressed)
                EVT_COMMAND(FULL_VIEWER_P, SO_WW_MOUSE_WHEEL_RELEASED, SoWwFullViewerP::wheelReleased)
                EVT_COMMAND(FULL_VIEWER_P, SO_WW_MOUSE_WHEEL_MOVED, SoWwFullViewerP::wheelMoved)
                EVT_BUTTON(INTERACT_BUTTON, SoWwFullViewerP::interactbuttonClicked)
                EVT_BUTTON(HOME_BUTTON, SoWwFullViewerP::homebuttonClicked)
                EVT_BUTTON(SET_HOME_BUTTON, SoWwFullViewerP::sethomebuttonClicked)
                EVT_BUTTON(SEEK_BUTTON, SoWwFullViewerP::seekbuttonClicked)
                EVT_BUTTON(EXAMINE_BUTTON, SoWwFullViewerP::viewbuttonClicked)
                EVT_BUTTON(VIEW_ALL_BUTTON, SoWwFullViewerP::viewallbuttonClicked)
wxEND_EVENT_TABLE()

#define PUBLIC(o) (o->pub)
#define PRIVATE(o) (o->pimpl)

SoWwFullViewerP::SoWwFullViewerP(SoWwFullViewer *pViewer)
        : SoGuiFullViewerP(pViewer)
        , wxPanel(pViewer->getParentWidget()) {
    assert(PUBLIC(this));
}

void
SoWwFullViewerP::setThumbWheelValue(wxWindow*, float value) {
    SOWW_STUB();
}

void
SoWwFullViewerP::showDecorationWidgets(SbBool onOff) {
#if SOWW_DEBUG && 0
    SoDebugError::postInfo("SoWwFullViewerP::showDecorationWidgets", "[invoked]");
#endif
    // remove old one
    delete this->mainlayout;

    assert(this->viewerwidget);

    assert(PUBLIC(this)->leftDecoration && PUBLIC(this)->bottomDecoration && PUBLIC(this)->rightDecoration);
    if (onOff) {
        PUBLIC(this)->leftDecoration->Show();
        PUBLIC(this)->bottomDecoration->Show();
        PUBLIC(this)->rightDecoration->Show();

        wxGridBagSizer* g = new wxGridBagSizer( 0, 0 );
        g->SetFlexibleDirection( wxBOTH );
        g->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );

        const int border_size = 0;
        g->Add( PUBLIC(this)->leftDecoration, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND | wxALL, border_size);
        g->Add( this->canvas, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxEXPAND | wxALL, border_size );

        g->Add( PUBLIC(this)->rightDecoration, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxEXPAND | wxALL, border_size );
        g->Add( PUBLIC(this)->bottomDecoration, wxGBPosition( 1, 0 ), wxGBSpan( 1, 3 ), wxEXPAND | wxALL, border_size );

#if SOWW_DEBUG && 0
        SoWwP::dumpWindowData(PUBLIC(this)->leftDecoration);
        SoWwP::dumpWindowData(PUBLIC(this)->rightDecoration);
        SoWwP::dumpWindowData(PUBLIC(this)->bottomDecoration);
#endif

        g->AddGrowableCol( 1 );
        g->AddGrowableRow( 0 );

        this->mainlayout = g;
    } else {
        wxBoxSizer * g = new wxBoxSizer(wxVERTICAL);
        g->Add(this->canvas);
        this->mainlayout = g;
        PUBLIC(this)->leftDecoration->Hide();
        PUBLIC(this)->bottomDecoration->Hide();
        PUBLIC(this)->rightDecoration->Hide();
    }

    this->viewerwidget->SetSizer(this->mainlayout);
    this->viewerwidget->Layout();

#if SOWW_DEBUG && 0
    SoDebugError::postInfo("SoWwFullViewerP::showDecorationWidgets", "dumpWindowData");
    SoWwP::dumpWindowData(this->viewerwidget->GetParent());
#endif

    wxSize size = this->viewerwidget->GetSize();
    SbVec2s resize = SbVec2s(size.GetX(), size.GetY());
    PUBLIC(this)->sizeChanged(resize);
}

void
SoWwFullViewerP::wheelPressed(wxCommandEvent& event) {
#if SOWW_DEBUG && 0
    SoDebugError::postInfo("SoWwFullViewerP::wheelPressed", "event arrived!");
#endif
    MapEvent::iterator it = objectMap.find( static_cast<wxWindow*>(event.GetEventObject()));
    if( it != objectMap.end() ) {
        VoidFuncNoPar function = it->second.onPress;
        (PUBLIC(this)->*function)();
    }
#if SOWW_DEBUG && 0
    else {
        SoDebugError::postInfo("SoWwFullViewerP::wheelPressed", "not valid event found!");
    }
#endif

    event.Skip();
}

void
SoWwFullViewerP::wheelReleased(wxCommandEvent& event) {
#if SOWW_DEBUG && 0
    SoDebugError::postInfo("SoWwFullViewerP::wheelReleased", "event arrived!");
#endif
    MapEvent::iterator it = objectMap.find( static_cast<wxWindow*>(event.GetEventObject()));
    if( it != objectMap.end() ) {
        VoidFuncNoPar function = it->second.onRelease;
        (PUBLIC(this)->*function)();
    }
#if SOWW_DEBUG && 0
    else {
        SoDebugError::postInfo("SoWwFullViewerP::wheelReleased", "not valid event found!");
    }
#endif

    event.Skip();
}

void processEvent();

void
SoWwFullViewerP::wheelMoved(wxCommandEvent & event) {
#if SOWW_DEBUG && 0
    SoDebugError::postInfo("SoWwFullViewerP::wheelMoved", "event arrived!");
#endif
    wxWindow* emitting_thumb_wheel = static_cast<wxWindow*>(event.GetEventObject());
    MapEvent::iterator it = objectMap.find( emitting_thumb_wheel);
    if( it != objectMap.end() ) {
        VoidFuncOnePar function = it->second.onMove;
        float *v = static_cast<float*>(event.GetClientData());
        if(v)
            (PUBLIC(this)->*function)(*v);
    }
#if SOWW_DEBUG && 0
    else {
        SoDebugError::postInfo("SoWwFullViewerP::wheelMoved", "not valid event found!");
    }
#endif

    event.Skip();
}

#define ADD_DATA_IN_MAP(objectName) \
assert(PUBLIC(this)->objectName && "Thumbwheel Can no be null"); \
objectMap[PUBLIC(this)->objectName] = WheelFunctions(&SoWwFullViewer::objectName ## Start,\
                                                     &SoWwFullViewer::objectName ## Finish, \
                                                     &SoWwFullViewer::objectName ## Motion)
void
SoWwFullViewerP::initThumbWheelEventMap() {
    ADD_DATA_IN_MAP(rightWheel);
    ADD_DATA_IN_MAP(leftWheel);
    ADD_DATA_IN_MAP(bottomWheel);
}

void
SoWwFullViewerP::homebuttonClicked(wxCommandEvent & ) {
    PUBLIC(this)->resetToHomePosition();
}

void
SoWwFullViewerP::sethomebuttonClicked(wxCommandEvent &) {
    PUBLIC(this)->saveHomePosition();
}

void
SoWwFullViewerP::viewallbuttonClicked(wxCommandEvent &) {
    PUBLIC(this)->viewAll();
}

void
SoWwFullViewerP::seekbuttonClicked(wxCommandEvent &) {
    PUBLIC(this)->setSeekMode(PUBLIC(this)->isSeekMode() ? FALSE : TRUE);
}

void
SoWwFullViewerP::interactbuttonClicked(wxCommandEvent &) {

    if (this->interactbutton)
        this->interactbutton->SetValue(TRUE);

    if (this->viewbutton)
        this->viewbutton->SetValue(FALSE);

    if (PUBLIC(this)->isViewing())
        PUBLIC(this)->setViewing(FALSE);
}

void
SoWwFullViewerP::seekbuttonClicked() {
    PUBLIC(this)->setSeekMode(PUBLIC(this)->isSeekMode() ? FALSE : TRUE);
}

void
SoWwFullViewerP::increaseInteractiveCount() {
    PUBLIC(this)->interactiveCountInc();
}

void
SoWwFullViewerP::decreaseInteractiveCount() {
    PUBLIC(this)->interactiveCountDec();
}

void
SoWwFullViewerP::viewbuttonClicked(wxCommandEvent &) {
    if (this->interactbutton)
        this->interactbutton->SetValue(FALSE);
    if (this->viewbutton)
        this->viewbutton->SetValue(TRUE);

    if (!PUBLIC(this)->isViewing())
        PUBLIC(this)->setViewing(TRUE);

}

#undef ADD_DATA_IN_MAP
#undef PUBLIC
#undef PRIVATE

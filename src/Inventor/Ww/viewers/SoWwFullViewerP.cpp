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

#include "Inventor/Ww/viewers/SoWwFullViewerP.h"
#include "Inventor/Ww/viewers/SoWwFullViewer.h"
#include "sowwdefs.h"
#include "Inventor/Ww/SoWwP.h"

#define PUBLIC(o) (o->pub)
#define PRIVATE(o) (o->pimpl)

SoWwFullViewerP::SoWwFullViewerP(SoWwFullViewer *pViewer)
: SoGuiFullViewerP(pViewer) {

}

void
SoWwFullViewerP::setThumbWheelValue(wxWindow*, float value) {
    SOWW_STUB();
}

void
SoWwFullViewerP::seekbuttonClicked()  {
    SOWW_STUB();
}

void
SoWwFullViewerP::showDecorationWidgets(SbBool onOff) {
#if SOWW_DEBUG
    SoDebugError::postInfo("SoWwFullViewerP::showDecorationWidgets", "[invoked]");
#endif // SOWW_DEBUG

    if (this->mainlayout) 
        delete this->mainlayout;

    assert(this->viewerwidget);

    assert(PUBLIC(this)->leftDecoration && PUBLIC(this)->bottomDecoration && PUBLIC(this)->rightDecoration);
    if (onOff) {
        PUBLIC(this)->leftDecoration->Show();
        PUBLIC(this)->bottomDecoration->Show();
        PUBLIC(this)->rightDecoration->Show();

        wxBoxSizer * g = new wxBoxSizer(wxHORIZONTAL); // VIEWERBORDER);

        g->Add(PUBLIC(this)->leftDecoration,1 , wxALL, 5);

        wxBoxSizer * subLayout = new wxBoxSizer(wxVERTICAL);

        //subLayout->Add(this->canvas,1 , wxALL, 5);
        subLayout->Add(PUBLIC(this)->bottomDecoration,1 , wxALL, 5);
        g->Add(subLayout);

        g->Add(PUBLIC(this)->rightDecoration,1 , wxALL, 5);

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

#if SOWW_DEBUG
    SoDebugError::postInfo("SoWwFullViewerP::showDecorationWidgets", "dumpWindowData");
    SoWwP::dumpWindowData(this->viewerwidget->GetParent());
#endif // SOWW_DEBUG


    wxSize size = this->viewerwidget->GetSize();
    SbVec2s resize = SbVec2s(size.GetX(), size.GetY());
    PUBLIC(this)->sizeChanged(resize);
}

#undef PUBLIC
#undef PRIVATE

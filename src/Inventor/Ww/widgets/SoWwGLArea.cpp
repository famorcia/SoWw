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

#include "Inventor/Ww/widgets/SoWwGLArea.h"
#include "Inventor/Ww/SoWwGLWidgetP.h"
#include "sowwdefs.h"

#include "wx/wx.h"
#include "wx/file.h"
#include "wx/dcclient.h"

#include <GL/gl.h>

wxBEGIN_EVENT_TABLE(SoWwGLArea, wxGLCanvas)
                EVT_PAINT(SoWwGLArea::OnPaint)
                EVT_ERASE_BACKGROUND(SoWwGLArea::OnEraseBackground)
wxEND_EVENT_TABLE()

wxDEFINE_EVENT(SO_WW_GL_INIT, wxCommandEvent);
wxDEFINE_EVENT(SO_WW_GL_DRAW, wxCommandEvent);


SoWwGLArea::SoWwGLArea(wxWindow *parent,
                       wxGLAttributes& attributes)

#ifdef __WXQT__
        : wxGLCanvas(parent,
                     wxID_ANY,
                     0,
                     wxDefaultPosition,
                     parent->GetClientSize())
#else
: wxGLCanvas(parent,
                     attributes,
                     wxID_ANY,
                     wxDefaultPosition,
                     parent->GetClientSize())
#endif
                     {
    this->SetName("SoWwGLArea");

    gl_real_context = new wxGLContext(this);
    is_gl_initialized = false;
    gl_attributes = attributes;
}

SoWwGLArea::~SoWwGLArea() {
    delete gl_real_context;
}

void SoWwGLArea::OnPaint(wxPaintEvent& event ) {
#if SOWW_DEBUG
    SoDebugError::postInfo("SoWwGLArea::OnPaint",
                           "size:%d %d",
                           GetSize().x,
                           GetSize().y);
#endif

    // must always be here
    wxPaintDC dc(this);

    InitGL();

    wxCommandEvent a_wx_event(SO_WW_GL_DRAW, GetId());
    a_wx_event.SetEventObject(this);
    a_wx_event.SetString("SO_WW_GL_INIT");
    ProcessWindowEvent(a_wx_event);

    event.Skip();
}

void SoWwGLArea::OnEraseBackground(wxEraseEvent& WXUNUSED(event)) {
    SOWW_STUB();
    // Do nothing, to avoid flashing on MSW
}

void SoWwGLArea::InitGL() {
    if(!is_gl_initialized) {
        SetCurrent(*gl_real_context);
        is_gl_initialized = true;

        wxCommandEvent a_wx_event(SO_WW_GL_INIT, GetId());
        a_wx_event.SetEventObject(this);
        a_wx_event.SetString("SO_WW_GL_INIT");
        ProcessWindowEvent(a_wx_event);
    } else {
        SetCurrent(*gl_real_context);
    }
}

void SoWwGLArea::makeCurrent() {
    if(gl_real_context)
        SetCurrent(*gl_real_context);
}

const wxGLContext *SoWwGLArea::context() {
    return gl_real_context;
}

// TODO: should be used glattributes, otherwise is not related to current glArea attributes
bool
SoWwGLArea::isDoubleBuffer() const {
    SOWW_STUB();
    return (true);
}

bool
SoWwGLArea::isStereo() const {
    SOWW_STUB();
    return (false);
}

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
                EVT_SIZE(SoWwGLArea::OnSize)
                EVT_PAINT(SoWwGLArea::OnPaint)
                EVT_ERASE_BACKGROUND(SoWwGLArea::OnEraseBackground)
                EVT_LEFT_DOWN(SoWwGLArea::OnMouse)
                EVT_RIGHT_DOWN(SoWwGLArea::OnMouse)
                EVT_LEFT_UP(SoWwGLArea::OnMouse)
                EVT_RIGHT_UP(SoWwGLArea::OnMouse)
                EVT_MOTION(SoWwGLArea::OnMouse)
                EVT_MOUSEWHEEL(SoWwGLArea::OnMouse)
wxEND_EVENT_TABLE()


static int wxIsDoubleBuffer() {
    static int is_double_buffer = -1;
    if(is_double_buffer == -1) {
        wxGLAttributes attributes;
        attributes.DoubleBuffer();
        attributes.EndList();
        is_double_buffer = wxGLCanvasBase::IsDisplaySupported(attributes) ? 1 : 0;
    }
    return (is_double_buffer);
}


SoWwGLArea::SoWwGLArea(SoWwGLWidgetP* aGLWidget,
                       wxGLAttributes& attributes,
                       wxWindowID id,
                       const wxPoint& pos,
                       const wxSize& size,
                       long style,
                       const wxString& name)
        : wxGLCanvas(aGLWidget->glparent,
                     attributes,
                     id,
                     pos,
                     size,
                     style | wxFULL_REPAINT_ON_RESIZE,
                     name) {
    ww_gl_widget = aGLWidget;
    gl_real_context = 0;
    is_gl_initialized = false;
    gl_attributes = attributes;
}

SoWwGLArea::~SoWwGLArea() {
    delete gl_real_context;
}

void SoWwGLArea::OnPaint(wxPaintEvent& event ) {
    // must always be here
    wxPaintDC dc(this);

    InitGL();
    ww_gl_widget->gl_exposed();
    event.Skip();
}

void SoWwGLArea::OnSize(wxSizeEvent& event) {
    // on size Coin need to know the new view port
    ww_gl_widget->gl_reshape(event.GetSize().x,
                             event.GetSize().y);
    ww_gl_widget->gl_changed();
    event.Skip();
}

void SoWwGLArea::OnEraseBackground(wxEraseEvent& WXUNUSED(event)) {
    SOWW_STUB();
    // Do nothing, to avoid flashing on MSW
}

void SoWwGLArea::InitGL() {
    if(!is_gl_initialized) {
        gl_real_context = new wxGLContext(this);
        SetCurrent(*gl_real_context);
        is_gl_initialized = true;
        ww_gl_widget->gl_init();
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

void SoWwGLArea::OnMouse(wxMouseEvent &event) {
    SoWwGLWidgetP::eventHandler(ww_gl_widget->glparent,
                                ww_gl_widget,
                                event,
                                0);
}

// TODO: should be used glattributes, otherwise is not related to current glArea attributes
bool
SoWwGLArea::isDoubleBuffer() const {
    return (wxIsDoubleBuffer());
}

bool
SoWwGLArea::isStereo() const {
    SOWW_STUB();
    return (false);
}
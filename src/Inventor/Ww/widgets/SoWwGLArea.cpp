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

#include "wx/wx.h"
#include "wx/file.h"
#include "wx/dcclient.h"
#include "wx/wfstream.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/SoInteraction.h>

wxBEGIN_EVENT_TABLE(SoWwGLArea, wxGLCanvas)
                EVT_SIZE(SoWwGLArea::OnSize)
                EVT_PAINT(SoWwGLArea::OnPaint)
                EVT_ERASE_BACKGROUND(SoWwGLArea::OnEraseBackground)
                EVT_TIMER(TIMER_ID, SoWwGLArea::OnTimer)
wxEND_EVENT_TABLE()


SoWwGLArea::SoWwGLArea(wxWindow *parent,
                       wxGLAttributes& attributes,
                       wxWindowID id,
                       const wxPoint& pos,
                       const wxSize& size,
                       long style,
                       const wxString& name)
        : wxGLCanvas(parent,
                     attributes,
                     id,
                     pos,
                     size,
                     style | wxFULL_REPAINT_ON_RESIZE,
                     name)
        , timer(this, TIMER_ID)
{
    glRealContext = 0;
    // Explicitly create a new rendering context instance for this canvas.
    isGLInitialized = false;

    W = size.x;
    H = size.y;
    const int timer_fire_interval = 1000/12;  // 1/12 second interval
    timer.Start(timer_fire_interval);
}

SoWwGLArea::~SoWwGLArea() {
    delete glRealContext;
}

void SoWwGLArea::OnPaint(wxPaintEvent& WXUNUSED(event) )
{
    // must always be here
    wxPaintDC dc(this);

    InitGL();

#if 0
    // Set the viewport
    SbViewportRegion myViewport(W, H);

    // Apply the render action for viewing the Coin3D on GL Window
    SoGLRenderAction myRenderAction(myViewport);
    myRenderAction.apply(root);
#endif

    glFlush();
    SwapBuffers();
}

void SoWwGLArea::OnSize(wxSizeEvent& event)
{
    // on size Coin need to know the new view port
    W = event.GetSize().x;
    H = event.GetSize().y;
}

void SoWwGLArea::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
    // Do nothing, to avoid flashing on MSW
}

void SoWwGLArea::OnTimer(wxTimerEvent& event)
{
    // Very important, Coin need to process internal timer, this need to be performed in the canvas periodically
    SoDB::getSensorManager()->processTimerQueue();
    Refresh(false);
}

void SoWwGLArea::InitGL()
{

    if(!isGLInitialized) {
        glRealContext = new wxGLContext(this);
        SetCurrent(*glRealContext);


        glEnable(GL_DEPTH_TEST);
        isGLInitialized = true;
    }
    glClearColor( 0.3f, 0.4f, 0.6f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

const wxGLContext *SoWwGLArea::context() {
    return glRealContext;
}

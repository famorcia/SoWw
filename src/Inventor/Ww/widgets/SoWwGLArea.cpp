//
// Created by fmorciano on 11/27/22.
//

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
    // Explicitly create a new rendering context instance for this canvas.
    glRealContext = new wxGLContext(this);
    isGLInitialized = false;

    SoDB::init();
    SoNodeKit::init();
    SoInteraction::init();

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
    SetCurrent(*glRealContext);
    if(!isGLInitialized) {
        glEnable(GL_DEPTH_TEST);
        isGLInitialized = true;
    }
    glClearColor( 0.3f, 0.4f, 0.6f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

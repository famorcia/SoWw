//
// Created by fmorciano on 11/27/22.
//

#include "Inventor/Ww/SoWwGLWidget.h"
#include "Inventor/Ww/SoWwGLWidgetP.h"

SOWW_OBJECT_ABSTRACT_SOURCE(SoWwGLWidget);

SoWwGLWidget::SoWwGLWidget(wxFrame* const parent ,
                           const char * const name ,
                           const SbBool embed ,
                           const int glmodes ,
                           const SbBool build)
inherited(parent, name, embed),
waitForExpose(true),
drawToFrontBuffer(false)
{

waitForExpose(true),
drawToFrontBuffer(false)
{
PRIVATE(this) = new SoQtGLWidgetP(this);

PRIVATE(this)->glSize = SbVec2s(0, 0);
#if QT_VERSION >= 0x050000
PRIVATE(this)->glSizeUnscaled = SbVec2s(0, 0);
#endif
PRIVATE(this)->wasresized = false;

#if QT_VERSION >= 0x060000
PRIVATE(this)->glformat = new QSurfaceFormat(QSurfaceFormat::defaultFormat());
  PRIVATE(this)->glformat->setSwapBehavior((glmodes & SO_GL_DOUBLE) ? QSurfaceFormat::DoubleBuffer : QSurfaceFormat::SingleBuffer);
  PRIVATE(this)->glformat->setDepthBufferSize((glmodes & SO_GL_ZBUFFER) ? 32 : 0);
  //PRIVATE(this)->glformat->setRgba((glmodes & SO_GL_RGB) ? true : false);
#else
PRIVATE(this)->glformat = new QGLFormat(QGLFormat::defaultFormat());
PRIVATE(this)->glformat->setDoubleBuffer((glmodes & SO_GL_DOUBLE) ? true : false);
PRIVATE(this)->glformat->setDepth((glmodes & SO_GL_ZBUFFER) ? true : false);
PRIVATE(this)->glformat->setRgba((glmodes & SO_GL_RGB) ? true : false);
#endif
PRIVATE(this)->glformat->setStereo((glmodes & SO_GL_STEREO) ? true : false);
bool enableoverlay = (glmodes & SO_GL_OVERLAY) ? true : false;
QGLFormat_setOverlay(PRIVATE(this)->glformat, enableoverlay);

PRIVATE(this)->glparent = NULL;
PRIVATE(this)->currentglwidget = NULL;
PRIVATE(this)->previousglwidget = NULL;
PRIVATE(this)->currentglarea = NULL;
PRIVATE(this)->previousglarea = NULL;
PRIVATE(this)->borderwidget = NULL;

#if QT_VERSION < 0x060000
if (! QGLFormat::hasOpenGL()) {
SoDebugError::post("SoQtGLWidget::SoQtGLWidget",
"OpenGL not available!");
// FIXME: this is not the way to handle this -- we should call the
// fatal error handler. 20011112 mortene.
return;
}
#endif

if (! build) { return; }

this->setClassName("SoQtGLWidget");
QWidget * parentwidget = this->getParentWidget();
QWidget * widget = this->buildWidget(parentwidget);
this->setBaseWidget(widget);
}
                           {

}

SoWwGLWidget::~SoWwGLWidget() {

}


void SoWwGLWidget::setBorder(const SbBool enable) {

}

void  SoWwGLWidget::setQuadBufferStereo(const SbBool enable) {

}

SbBool  SoWwGLWidget::isQuadBufferStereo(void) const {

}


void   SoWwGLWidget::setGLSize(const SbVec2s size){ }
SbVec2s   SoWwGLWidget::getGLSize(void) const{ }
float   SoWwGLWidget::getGLAspectRatio(void) const{ }

SbBool   SoWwGLWidget::isRGBMode(void){ }

void   SoWwGLWidget::glLockNormal(void){ }
void   SoWwGLWidget::glUnlockNormal(void){ }

void   SoWwGLWidget::glLockOverlay(void){ }
void   SoWwGLWidget::glUnlockOverlay(void){ }

void   SoWwGLWidget::glSwapBuffers(void){ }
void   SoWwGLWidget::glFlushBuffer(void){ }

SbBool   SoWwGLWidget::glScheduleRedraw(void){ }

SbBool SoWwGLWidget::isBorder(void) const{ }

void SoWwGLWidget::setDoubleBuffer(const SbBool enable){ }
SbBool SoWwGLWidget::isDoubleBuffer(void) const{ }

void SoWwGLWidget::setDrawToFrontBufferEnable(const SbBool enable){ }
SbBool SoWwGLWidget::isDrawToFrontBufferEnable(void) const{ }


void SoWwGLWidget::setAccumulationBuffer(const SbBool enable){ }
SbBool SoWwGLWidget::getAccumulationBuffer(void) const{ }

void SoWwGLWidget::setStencilBuffer(const SbBool enable){ }
SbBool SoWwGLWidget::getStencilBuffer(void) const{ }

void SoWwGLWidget::setAlphaChannel(const SbBool enable){ }
SbBool SoWwGLWidget::getAlphaChannel(void) const{ }

void SoWwGLWidget::setOverlayRender(const SbBool onoff){ }
SbBool SoWwGLWidget::isOverlayRender(void) const{ }

void SoWwGLWidget::setSampleBuffers(const int numsamples){ }
int SoWwGLWidget::getSampleBuffers(void) const{ }


wxFrame* SoWwGLWidget::getGLWidget(void) const{ }
wxFrame* SoWwGLWidget::getNormalWidget(void) const{ }
wxFrame* SoWwGLWidget::getOverlayWidget(void) const{ }

SbBool SoWwGLWidget::hasOverlayGLArea(void) const{ }
SbBool SoWwGLWidget::hasNormalGLArea(void) const{ }

unsigned long SoWwGLWidget::getOverlayTransparentPixel(void){ }

void SoWwGLWidget::processEvent(wxEvent* event){ }

wxFrame* SoWwGLWidget::buildWidget(wxFrame* parent){ }

void SoWwGLWidget::redrawOverlay(void){ }

void SoWwGLWidget::initGraphic(void){ }
void SoWwGLWidget::initOverlayGraphic(void){ }

void SoWwGLWidget::sizeChanged(const SbVec2s & size){ }
void SoWwGLWidget::widgetChanged(wxFrame* w){ }


void SoWwGLWidgetP::eventHandler(wxFrame*, void*, wxEvent*, bool*) {

}


wxBEGIN_EVENT_TABLE(SoWwGLWidgetP, wxGLCanvas)
                EVT_SIZE(SoWwGLWidgetP::OnSize)
                EVT_PAINT(SoWwGLWidgetP::OnPaint)
                EVT_ERASE_BACKGROUND(SoWwGLWidgetP::OnEraseBackground)
                EVT_TIMER(TIMER_ID, SoWwGLWidgetP::OnTimer)
wxEND_EVENT_TABLE()

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

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


SoWwGLWidgetP::SoWwGLWidgetP(wxWindow *parent,
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

    /*SoDB::init();
    SoNodeKit::init();
    SoInteraction::init();
    */
    W = size.x;
    H = size.y;
    const int timer_fire_interval = 1000/12;  // 1/12 second interval
    timer.Start(timer_fire_interval);
}

SoWwGLWidgetP::~SoWwGLWidgetP()
{
    delete glRealContext;
}

void SoWwGLWidgetP::OnPaint(wxPaintEvent& WXUNUSED(event) )
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

void SoWwGLWidgetP::OnSize(wxSizeEvent& event)
{
    // on size Coin need to know the new view port
    W = event.GetSize().x;
    H = event.GetSize().y;
}

void SoWwGLWidgetP::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
    // Do nothing, to avoid flashing on MSW
}

void SoWwGLWidgetP::OnTimer(wxTimerEvent& event)
{
    // Very important, Coin need to process internal timer, this need to be performed in the canvas periodically
    SoDB::getSensorManager()->processTimerQueue();
    Refresh(false);
}

void SoWwGLWidgetP::InitGL()
{
    SetCurrent(*glRealContext);
    if(!isGLInitialized) {
        glEnable(GL_DEPTH_TEST);
        isGLInitialized = true;
    }
    glClearColor( 0.3f, 0.4f, 0.6f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


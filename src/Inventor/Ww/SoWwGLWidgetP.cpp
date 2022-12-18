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

#include "Inventor/Ww/SoWwGLWidgetP.h"
#include "Inventor/Ww/SoWwGLWidget.h"
#include "Inventor/Ww/widgets/SoWwGLArea.h"
#include "Inventor/Ww/SoAny.h"

#include <Inventor/SbTime.h>

#include "sowwdefs.h"

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

SoWwGLWidgetP::SoWwGLWidgetP(SoWwGLWidget * o)
        : SoGuiGLWidgetP(o)
{
    this->borderthickness = 0;
    this->oldcontext = NULL;
}

SoWwGLWidgetP::~SoWwGLWidgetP()
{
}

/*// Gets called by the SoWwGLArea instance upon keyboard presses. These
// are then forwarded to subclasses for handling.
void
SoWwGLWidgetP::GLAreaKeyEvent(QKeyEvent * e, void * userdata)
{
    SoWwGLWidget * that = (SoWwGLWidget *)userdata;
    that->processEvent(e);
}*/

// slot invoked upon QScreen change
void
SoWwGLWidgetP::gl_changed(void)
{
    if (SOWW_DEBUG && 0) { // debug
        SoDebugError::postInfo("gl_changed", "invoked");
    }

    if (this->currentglwidget) {
        // SbVec2s glSize = this->glSizeUnscaled * this->currentglwidget->devicePixelRatioF();
        if (glSize != this->glSize) {
            this->glSize = glSize;
            PUBLIC(this)->setSize(PUBLIC(this)->getSize());
        }
    }
}

// slot invoked upon QGLWidget initialization
void
SoWwGLWidgetP::gl_init(void)
{
    if (SOWW_DEBUG && 0) { // debug
        SoDebugError::postInfo("gl_init", "invoked");
    }

    PUBLIC(this)->initGraphic();
}

// slot invoked upon QGLWidget resizes
void
SoWwGLWidgetP::gl_reshape(int width, int height)
{
#if SOWW_DEBUG && 0
    // debug
    SoDebugError::postInfo("gl_reshape", "<%d, %d>", width, height);
#endif

    this->glSize = SbVec2s((short) width, (short) height);
    this->wasresized = true;
    pub->setSize(this->glSize);
}

// slot invoked upon QGLWidget expose events
void
SoWwGLWidgetP::gl_exposed(void)
{
    if (SOWW_DEBUG && 0) { // debug
        SoDebugError::postInfo("gl_exposed", "%f", SbTime::getTimeOfDay().getValue());
    }

    if (PUBLIC(this)->waitForExpose) {
        PUBLIC(this)->waitForExpose = false; // Gets flipped from TRUE on first expose.
        PUBLIC(this)->setSize(PUBLIC(this)->getSize());
    }
    if (this->wasresized) {
        PUBLIC(this)->sizeChanged(this->glSize);
        this->wasresized = false;
    }

    if (!PUBLIC(this)->glScheduleRedraw()) {
        PUBLIC(this)->redraw();
    }
}


static const char eventnaming[][50] = {
        "None", // 0
        "Timer",
        "MouseButtonPress",
        "MouseButtonRelease",
        "MouseButtonDblClick",
        "MouseMove",
        "KeyPress",
        "KeyRelease",
        "FocusIn",
        "FocusOut",
        "Enter",
        "Leave",
        "Paint",
        "Move",
        "Resize",
        "Create",
        "Destroy",
        "Show",
        "Hide",
        "Close",
        "Quit", // 20
        "*unknown*", "*unknown*", "*unknown*", "*unknown*", "*unknown*",
        "*unknown*", "*unknown*", "*unknown*", "*unknown*",
        "Accel", // 30
        "Wheel",
        "AccelAvailable", // 32
        "CaptionChange",
        "*unknown*", "*unknown*", "*unknown*", "*unknown*", "*unknown*", "*unknown*",
        "Clipboard", // 40
        "*unknown*", "*unknown*", "*unknown*", "*unknown*", "*unknown*",
        "*unknown*", "*unknown*", "*unknown*", "*unknown*",
        "SockAct", // 50
        "AccelOverride", "*unknown*", "*unknown*", "*unknown*", "*unknown*",
        "*unknown*", "*unknown*", "*unknown*", "*unknown*",
        "DragEnter", // 60
        "DragMove",
        "DragLeave",
        "Drop",
        "DragResponse", // 64
        "*unknown*", "*unknown*", "*unknown*", "*unknown*", "*unknown*",
        "ChildInserted", // 70
        "ChildRemoved",
        "LayoutHint", // 72
        "*unknown*", "*unknown*", "*unknown*", "*unknown*", "*unknown*",
        "*unknown*", "*unknown*",
        "ActivateControl", // 80
        "DeactivateControl"
};

#if 0
bool
SoWwGLWidgetP::eventFilter(QObject * obj, QEvent * e)
{
    if (SOWW_DEBUG && 0) { // debug
#if QT_VERSION >= 0x040000
        SbString w = obj->objectName().toLatin1().constData();
#else
        SbString w = obj->name();
#endif
        SbBool istoplevel = obj == this->currentglwidget->topLevelWidget();

        if (obj == this->glparent) { w = "glparent"; }
        else if (obj == this->currentglwidget) { w = "currentglwidget"; }
        else if (obj == this->borderwidget) { w = "borderwidget"; }
        else if (istoplevel) { w = "top-level"; }

        SoDebugError::postInfo("SoWwGLWidgetP::eventFilter",
                               "[invoked] QEvent==%p obj==%p==\"%s\"==%s (%s) %s (typecode==%d)",
                               e, obj, w.getString(), obj->metaObject()->className(),
                               istoplevel ? "TOPLEVEL" : "",
                               eventnaming[e->type()], e->type());
    }

    QEvent::Type etype = e->type();

    // FIXME: Under Qt 3.0.0 we got buggy mouse event handling, since
    // mouse events were routed to the gl widget, even if it was
    // supposed to go somewhere else. I'm not sure if this is the
    // correct fix though. pederb, 2001-10-16

    if ((etype == QEvent::MouseButtonPress ||
         etype == QEvent::MouseButtonRelease ||
         etype == QEvent::MouseButtonDblClick ||
         etype == QEvent::MouseMove) &&
        (obj != this->currentglarea)) {
        return false;
    }

    bool kbdevent = false;
    if ( QEvent::KeyPress == etype ) kbdevent = true;
    if ( QEvent::KeyRelease == etype ) kbdevent = true;

    if (kbdevent) {
        // Ignore keyboard-events, as they are caught directly by the
        // SoWwGLArea widget and forwarded through the
        // SoWwGLWidgetP::GLAreaKeyEvent() callback.
        return false;
    }

    if (obj == (QObject *) this->glparent) {
        // If this hits, the this->glparent wxFrame is a toplevelshell, so
        // we resize the GL widget along with it.
        if (e->type() == QEvent::Resize) {
            QResizeEvent * r = (QResizeEvent *)e;
            if (SOWW_DEBUG && 0) {  // debug
                SoDebugError::postInfo("SoWwGLWidgetP::eventFilter",
                                       "resize parent %p: (%d, %d)",
                                       this->glparent,
                                       r->size().width(), r->size().height());
            }

            this->borderwidget->resize(r->size());
//      int newwidth = r->size().width() - 2 * this->borderthickness;
//      int newheight = r->size().height() - 2 * this->borderthickness;

//      PUBLIC(this)->glwidget->setGeometry(this->borderthickness,
//                                   this->borderthickness,
//                                   newwidth - 1, newheight -1);
//      QRect glarea = this->borderwidget->contentsRect();
//      glarea =
//      PUBLIC(this)->glwidget->setGeometry(this->borderwidget->contentsRect());

/*
      int newwidth = r->size().width();
      int newheight = r->size().height();
      PUBLIC(this)->sizeChanged(SbVec2s(newwidth - 1, newheight - 1));
*/
#if 0 // debug
            SoDebugError::postInfo("SoWwGLWidgetP::eventFilter", "resize done");
#endif // debug
        }
    }
    else if (obj == (QObject *) this->currentglarea) {
        // We used to return ``true'' here if the event was a
        // QResizeEvent. The reason why we'd want to stop resize events
        // being passed on to the native Qt handler was not commented, and
        // I can't see why this should be necessary. Also, kyrah found out
        // that it causes a nasty problem with Qt/Mac, so it has been
        // removed.  <mortene@sim.no>.
    }

    // OBSOLETED: SoWwGLWidget does no longer inherit QObject
    // (indirectly through SoQtObject). I'm not sure whether or not
    // leaving this out causes any trouble..  If no problems are
    // detected, remove eventually. 20020613 mortene.
#if 0
    else {
    // Handle in superclass.
    bool stop = PUBLIC(this)->eventFilter(obj, e);
    if (stop) { return true; }
  }
#endif // OBSOLETED

    PUBLIC(this)->processEvent(e);
    return false;
}

// Registered callback on device events, set up by the
// So@Gui@RenderArea.
void
SoWwGLWidgetP::eventHandler(wxFrame * widget, void * closure, QEvent * event,
                            bool *)
{
    assert(closure != NULL);
    SoWwGLWidget * component = (SoWwGLWidget *) closure;
    component->processEvent(event);
}

#endif

// The GL widget rebuilding has been written to remember the previous
// GL widget, which might be swapped back in if it fits the wanted
// format.
//
// There are two reasons for keeping track of both the current and the
// previous widget:
//
//  1) efficiency; often one swaps back and forth between only two
//  different visuals -- like single and double buffering, or stereo
//  mode on and off
//
//  2) robustness; killing off the previous widget in the build-method
//  below has nasty sideeffects (like "random" coredumps), since the
//  Qt event loop might be using it
void
SoWwGLWidgetP::buildGLWidget(void)
{

    if (SOWW_DEBUG && 0) { // debug
        SoDebugError::postInfo("SoWwGLWidgetP::buildGLWidget",
                // TODO "%s, %s, %s, %s, %s",
                               "%s, %s, %s",
                               PUBLIC(this)->isDoubleBuffer() ? "double" : "single",
                // TODO: this->glformat->depth() ? "z-buffer" : "no z-buffer",
                               PUBLIC(this)->isRGBMode() ? "RGBA" : "colorindex",
                               PUBLIC(this)->isQuadBufferStereo() ? "stereo" : "mono"
                // TODO: ,QGLFormat_hasOverlay(this->glformat) ? "overlay" : "no overlay"
        );
    }

    wxFrame * wascurrent = dynamic_cast<wxFrame *>(this->currentglwidget);
    wxFrame * wasprevious = this->previousglwidget;
    SoWwGLArea * wascurrentarea = this->currentglarea;
    SoWwGLArea * waspreviousarea = this->previousglarea;

    void * display = NULL;
    void * screen = NULL;

    if (wascurrent) {
        // Do _not_ turn off mousetracking or remove the eventfilter, as
        // we'd loose events after the switch has happened if the user is
        // already interacting with the canvas (e.g. when starting a drag
        // in BUFFER_INTERACTIVE mode).
        //TODO: QObject::disconnect(wascurrentarea, SIGNAL(expose_sig()), this, SLOT(gl_exposed()));
        //TODO: QObject::disconnect(wascurrentarea, SIGNAL(init_sig()), this, SLOT(gl_init()));
        //TODO: QObject::disconnect(wascurrentarea, SIGNAL(screenChanged(QScreen*)), this, SLOT(gl_changed()));
        //    QObject::disconnect(wascurrentarea, SIGNAL(reshape_sig()), this, SLOT(gl_reshape()));
        this->previousglwidget = wascurrent;
        this->previousglarea = wascurrentarea;
    }

#if 0
    if (wasprevious
        // TODO: && QGLFormat_eq(*this->glformat, waspreviousarea->format())
            ) {
        // Reenable the previous widget.
        if (this->currentglwidget) SoAny::si()->unregisterGLContext((void *)PUBLIC(this));
        this->currentglwidget = wasprevious;
        this->currentglarea = waspreviousarea;
        SoAny::si()->registerGLContext((void *)PUBLIC(this), display, screen);
        if (SOWW_DEBUG && 0) { // debug
            SoDebugError::postInfo("SoWwGLWidgetP::buildGLWidget",
                                   "reused previously used GL widget");
        }
    }
    else {
        // Couldn't use the previous widget, make a new one.
        SoWwGLWidget * sharewidget = (SoWwGLWidget*) SoAny::si()->getSharedGLContext(display, screen);
        if (this->currentglwidget) SoAny::si()->unregisterGLContext((void *)PUBLIC(this));

        this->currentglarea = new SoWwGLArea(sharewidget ? (wxWindow*) sharewidget->getGLWidget() : NULL,
                                             glAttributes);
        this->currentglwidget = this->currentglarea;
        // TODO: this->currentglarea->registerQKeyEventHandler(SoWwGLWidgetP::GLAreaKeyEvent, PUBLIC(this));
        // TODO: this->currentglwidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
        SoAny::si()->registerGLContext((void *)PUBLIC(this), display, screen);
        // Send this one to the final hunting grounds.
        delete wasprevious;
    }
#else
    if (this->currentglwidget) SoAny::si()->unregisterGLContext((void *)PUBLIC(this));

    this->currentglarea = new SoWwGLArea(this,
                                         glAttributes);

    this->currentglwidget = this->currentglarea;
    // TODO: this->currentglarea->registerQKeyEventHandler(SoWwGLWidgetP::GLAreaKeyEvent, PUBLIC(this));
    // TODO: this->currentglwidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
    SoAny::si()->registerGLContext((void *)PUBLIC(this), display, screen);

#endif

    if (SOWW_DEBUG) { // Warn about requested features that we didn't get.
        // TODO: QSurfaceFormat * w = this->glformat; // w(anted)
        // TODO: QSurfaceFormat g = this->currentglarea->format(); // g(ot)

#define GLWIDGET_FEATURECMP(_glformatfunc_, _cmpvalue_, _truestr_, _falsestr_) \
  do { \
    if (w->_glformatfunc_() != g._glformatfunc_()) { \
      SoDebugError::postWarning("SoWwGLWidgetP::buildGLWidget", \
                                "wanted %s, but that is not supported " \
                                "by the OpenGL driver", \
                                w->_glformatfunc_() == _cmpvalue_ ? _truestr_ : _falsestr_); \
    } \
  } while (0)

        // TODO: GLWIDGET_FEATURECMP(swapBehavior, QSurfaceFormat::DoubleBuffer, "doublebuffer visual", "singlebuffer visual");
        // TODO: GLWIDGET_FEATURECMP(depthBufferSize, 0, "visual without depthbuffer", "visual with depthbuffer");
        // TODO: //GLWIDGET_FEATURECMP(rgba, true, "RGBA buffer", "colorindex buffer");
        // TODO: GLWIDGET_FEATURECMP(stereo, true, "stereo buffers", "mono buffer");

#if HAVE_QT_SAMPLE_BUFFERS
        // TODO:     GLWIDGET_FEATURECMP(samples, 0, "no sample buffers", "sample buffers");
#endif

// TODO:
#if 0
        if (QGLFormat_hasOverlay(w) != QGLFormat_hasOverlay(&g)) {
            SoDebugError::postWarning("SoWwGLWidgetP::buildGLWidget",
                                      "wanted %s, but that is not supported "
                                      "by the OpenGL driver",
                                      QGLFormat_hasOverlay(w) ?
                                      "overlay plane(s)" :
                                      "visual without overlay plane(s)");
        }
#endif
    }
#undef GLWIDGET_FEATURECMP

    //TODO: this->glformat = this->currentglarea->format();

    int frame = PUBLIC(this)->isBorder() ? this->borderthickness : 0;
    // TODO: this->currentglwidget->setGeometry(frame, frame,this->glSize[0] - 2*frame,this->glSize[1] - 2*frame);

    // TODO: QObject::connect(this->currentglarea, SIGNAL(init_sig()), this, SLOT(gl_init()));
    //  QObject::connect(this->currentglarea, SIGNAL(reshape_sig(int, int)),
    //                    this, SLOT(gl_reshape(int, int)));
    // TODO: QObject::connect(this->currentglarea, SIGNAL(screenChanged(QScreen*)), this, SLOT(gl_changed()));
    // TODO: QObject::connect(this->currentglarea, SIGNAL(expose_sig()), this, SLOT(gl_exposed()));

    // TODO: this->currentglwidget->setMouseTracking(true);
    // TODO: this->currentglarea->installEventFilter(this);

    // Reset to avoid unnecessary scenegraph redraws.
    //TODO: set to false for TEST
    // PUBLIC(this)->waitForExpose = true;
    PUBLIC(this)->waitForExpose = false;

    // We've changed to a new widget, so notify subclasses through this
    // virtual method.
    PUBLIC(this)->widgetChanged(dynamic_cast<wxFrame *>(this->currentglwidget));

    if (wascurrent) {
        // If we are rebuilding, we need to explicitly call show() here,
        // as no message to show will be given from an already visible
        // parent. (If the glwidget was built but not shown before the
        // rebuild, the call below doesn't do any harm, as the glwidget
        // still won't become visible until all parents are visible.)
        this->currentglwidget->Show();
        // TODO: this->currentglwidget->raise();
    }
    // TODO: this->currentglwidget->setFocus();
}

// Returns the normal GL context.
const wxGLContext *
SoWwGLWidgetP::getNormalContext(void)
{
    SoWwGLArea * w = this->currentglarea;
    if (w) return w->context();
    return NULL;
}

// Returns the overlay GL context.
const wxGLContext *
SoWwGLWidgetP::getOverlayContext(void)
{
    SoWwGLArea * w = this->currentglarea;
    // TODO: if (w) { return QGLWidget_overlayContext(w); }
    return NULL;
}

#if HAVE_GLX
// There is something in this header file that fools the g++ 2.95.4
// compiler to run into problems with some of the code in
// SoWwGLWidgetP::eventFilter() (it gives a completely non-sensical
// error message), so we just include it at the bottom like this.
#include <GL/glx.h> // For glXIsDirect().
#endif // HAVE_GLX

// Return a flag indicating whether or not OpenGL rendering is
// happening directly from the CPU(s) to the GPU(s), ie on a local
// display. With GLX on X11, it is possible to do remote rendering.
SbBool
SoWwGLWidgetP::isDirectRendering(void)
{
#if defined(Q_WS_X11)
    PUBLIC(this)->glLockNormal();
  GLXContext ctx = glXGetCurrentContext();
  if (!ctx) {
    SoDebugError::postWarning("SoWwGLWidgetP::isDirectRendering",
                              "Could not get hold of current context.");
    return true;
  }
  Display * d;
#if QT_VERSION < 0x040000 // pre Qt 4
  d = qt_xdisplay();
#else // Qt 4.0.0+
  d = QX11Info::display();
#endif
  Bool isdirect = glXIsDirect(d, ctx);
  PUBLIC(this)->glUnlockNormal();
  return isdirect ? true : false;
#else // ! X11
    return true; // Neither MSWindows nor Mac OS X is capable of remote display.
#endif // ! X11
}

void SoWwGLWidgetP::eventHandler(wxWindow * widget , void *closure, wxEvent &event, bool *) {
    assert(closure != NULL);
    SoWwGLWidget * component = ((SoWwGLWidgetP *) closure)->pub;
    component->processEvent(event);
}

// *************************************************************************

#undef PRIVATE
#undef PUBLIC




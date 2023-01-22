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
        : SoGuiGLWidgetP(o) {
    this->borderthickness = 0;
    this->oldcontext = NULL;
}

SoWwGLWidgetP::~SoWwGLWidgetP() {
}

/*// Gets called by the SoWwGLArea instance upon keyboard presses. These
// are then forwarded to subclasses for handling.
void
SoWwGLWidgetP::GLAreaKeyEvent(QKeyEvent * e, void * userdata)
{
    SoWwGLWidget * that = (SoWwGLWidget *)userdata;
    that->processEvent(e);
}*/


// slot invoked upon QGLWidget initialization
void
SoWwGLWidgetP::gl_init(wxCommandEvent& )
{
#if SOWW_DEBUG
    SoDebugError::postInfo("gl_init", "invoked");
#endif

    PUBLIC(this)->initGraphic();
}

void
SoWwGLWidgetP::gl_reshape(wxSizeEvent& event) {
#if SOWW_DEBUG
    SoDebugError::postInfo("SoWwGLWidgetP::gl_reshape",
                           "<%d, %d>",
                           event.GetSize().GetWidth(),
                           event.GetSize().GetHeight());
#endif

    this->glSize = SbVec2s((short) event.GetSize().GetWidth(), (short) event.GetSize().GetHeight());
    this->wasresized = true;
    pub->setSize(this->glSize);
}

void
SoWwGLWidgetP::gl_exposed(wxCommandEvent&) {
#if SOWW_DEBUG
    SoDebugError::postInfo("SoWwGLWidgetP::gl_exposed", "%f", SbTime::getTimeOfDay().getValue());
#endif

    if (PUBLIC(this)->waitForExpose) {
#if SOWW_DEBUG
        SoDebugError::postInfo("SoWwGLWidgetP::gl_exposed", "waitForExpose");
#endif
        PUBLIC(this)->waitForExpose = false; // Gets flipped from TRUE on first expose.
        PUBLIC(this)->setSize(PUBLIC(this)->getSize());
    }
    if (this->wasresized) {
#if SOWW_DEBUG
        SoDebugError::postInfo("SoWwGLWidgetP::gl_exposed", "wasresized");
#endif
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
//  event loop might be using it
SoWwGLArea*
SoWwGLWidgetP::buildGLWidget(void) {

    try {
#if SOWW_DEBUG
        SoDebugError::postInfo("SoWwGLWidgetP::buildGLWidget",
                "%s, %s, %s, %s, %s",
                               PUBLIC(this)->isDoubleBuffer() ? "double" : "single",
                               this->hasZBuffer() ? "z-buffer" : "no z-buffer",
                               PUBLIC(this)->isRGBMode() ? "RGBA" : "colorindex",
                               PUBLIC(this)->isQuadBufferStereo() ? "stereo" : "mono",
                               this->hasOverlay() ? "overlay" : "no overlay");
#endif

        wxWindow *wascurrent = this->currentglwidget;
        wxWindow *wasprevious = this->previousglwidget;
        SoWwGLArea *wascurrentarea = this->currentglarea;
        SoWwGLArea *waspreviousarea = this->previousglarea;

        void *display = NULL;
        void *screen = NULL;

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

        if (wasprevious
            // TODO: && QGLFormat_eq(*this->glformat, waspreviousarea->format())
                ) {
            // Reenable the previous widget.
            if (this->currentglwidget) SoAny::si()->unregisterGLContext((void *) PUBLIC(this));
            this->currentglwidget = wasprevious;
            this->currentglarea = waspreviousarea;
            SoAny::si()->registerGLContext((void *) PUBLIC(this), display, screen);
#if SOWW_DEBUG
                SoDebugError::postInfo("SoWwGLWidgetP::buildGLWidget",
                                       "reused previously used GL widget");
#endif
        } else {
            if (this->currentglwidget)
                SoAny::si()->unregisterGLContext((void *) PUBLIC(this));

            this->currentglarea = new SoWwGLArea(
                    glparent,
                    gl_attributes);

            // a wxPanel need a sizer, look at: https://forums.wxwidgets.org/viewtopic.php?t=44252
            if( SoWwGLWidgetP::isAPanel(glparent)) {
                // new sizer remove the previous one (I'm sorry)
                addSizer();
            }

            this->currentglwidget = this->currentglarea;
            // TODO: this->currentglarea->registerQKeyEventHandler(SoWwGLWidgetP::GLAreaKeyEvent, PUBLIC(this));
            // TODO: this->currentglwidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
            SoAny::si()->registerGLContext((void *) PUBLIC(this), display, screen);
            // Send this one to the final hunting grounds.
            delete wasprevious;
        }

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

        this->currentglarea->Bind(wxEVT_SIZE, &SoWwGLWidgetP::gl_reshape, this);
        this->currentglarea->Bind(SO_WW_GL_INIT, &SoWwGLWidgetP::gl_init, this);
        this->currentglarea->Bind(SO_WW_GL_DRAW, &SoWwGLWidgetP::gl_exposed, this);
        this->currentglarea->Bind(wxEVT_LEFT_DOWN, &SoWwGLWidgetP::onMouse, this);
        this->currentglarea->Bind(wxEVT_RIGHT_DOWN, &SoWwGLWidgetP::onMouse, this);
        this->currentglarea->Bind(wxEVT_LEFT_UP, &SoWwGLWidgetP::onMouse, this);
        this->currentglarea->Bind(wxEVT_RIGHT_UP, &SoWwGLWidgetP::onMouse, this);
        this->currentglarea->Bind(wxEVT_MOTION, &SoWwGLWidgetP::onMouse, this);
        this->currentglarea->Bind(wxEVT_MOUSEWHEEL, &SoWwGLWidgetP::onMouse, this);
        this->currentglarea->Bind(wxEVT_KEY_DOWN, &SoWwGLWidgetP::onKey, this);
        this->currentglarea->Bind(wxEVT_KEY_UP, &SoWwGLWidgetP::onKey, this);

        // Reset to avoid unnecessary scenegraph redraws.
        PUBLIC(this)->waitForExpose = true;

        // We've changed to a new widget, so notify subclasses through this
        // virtual method.
        PUBLIC(this)->widgetChanged(this->currentglwidget);

        if (wascurrent) {
            // If we are rebuilding, we need to explicitly call show() here,
            // as no message to show will be given from an already visible
            // parent. (If the glwidget was built but not shown before the
            // rebuild, the call below doesn't do any harm, as the glwidget
            // still won't become visible until all parents are visible.)
            this->currentglwidget->Show();
        }
    }
    catch (std::exception& e) {
        SoDebugError::postWarning("SoWwGLWidgetP::buildGLWidget",
                                  "exception: %s",
                                  e.what());
    }
    catch(...) {
        SoDebugError::postWarning("SoWwGLWidgetP::buildGLWidget",
                                  "unknown exception");
    }
    return (this->currentglarea);
}

// Returns the normal GL context.
const wxGLContext *
SoWwGLWidgetP::getNormalContext(void) {
    SoWwGLArea * w = this->currentglarea;
    if (w) return w->context();
    return NULL;
}

// Returns the overlay GL context.
const wxGLContext *
SoWwGLWidgetP::getOverlayContext(void) {
    SoWwGLArea * w = this->currentglarea;
    // TODO: if (w) { return QGLWidget_overlayContext(w); }
    return NULL;
}

SbBool
SoWwGLWidgetP::isDirectRendering(void) {
    SbBool res = FALSE;
    if(this->currentglarea && this->currentglarea->GetGLCTXAttrs().x11Direct)
        res = TRUE;
    return (res);
}

void SoWwGLWidgetP::initGLModes(int glmodes) {

    gl_attributes.clear();
    if(glmodes & SO_GL_DOUBLE) {
        gl_attributes.push_back(WX_GL_DOUBLEBUFFER);
    }
    if(glmodes & SO_GL_ZBUFFER) {
        // 24 bit seems to be ok also on Windows
        gl_attributes.push_back(WX_GL_DEPTH_SIZE);
        gl_attributes.push_back(24);
    }
    if(glmodes & SO_GL_RGB) {
        gl_attributes.push_back(WX_GL_RGBA);
    }
    if(glmodes & SO_GL_STEREO) {
        gl_attributes.push_back(WX_GL_STEREO);
    }
    gl_attributes.push_back(0);

    /*
    if(!SoWwGLArea::IsDisplaySupported(&gl_attributes[0])) {
        SoDebugError::postInfo("SoWwGLWidget::SoWwGLWidget",
                               "required GL modes are not supported!");
    }
     */
}

void
SoWwGLWidgetP::eventHandler(wxWindow * /*widget*/ , void *closure, wxEvent &event, bool *) {
#if SOWW_DEBUG
    SoDebugError::postInfo("SoWwGLWidgetP::eventHandler",
                           "");
#endif
    assert(closure != NULL);
    SoWwGLWidget * component = ((SoWwGLWidgetP *) closure)->pub;
    component->processEvent(event);
}

void
SoWwGLWidgetP::onMouse(wxMouseEvent &event) {
#if SOWW_DEBUG && 0
    SoDebugError::postInfo("SoWwGLWidgetP::onMouse",
                           "mouse event");
#endif
    PUBLIC(this)->processEvent(event);
}

void
SoWwGLWidgetP::onKey(wxKeyEvent &event) {
#if SOWW_DEBUG
    SoDebugError::postInfo("SoWwGLWidgetP::onKey",
                           "key event");
#endif
    PUBLIC(this)->processEvent(event);
}

bool
SoWwGLWidgetP::isAPanel(wxWindow* window) {
    return (window->IsKindOf(wxCLASSINFO(wxPanel)));
}

void
SoWwGLWidgetP::addSizer() {

    if(glparent->GetSizer()) {
        SoDebugError::postWarning("SoWwGLWidgetP::addSizer",
                                  "panel holds a sizer, the old one will be removed");
    }

    wxFlexGridSizer* sizer = new wxFlexGridSizer(0);
    sizer->AddGrowableCol(0);
    sizer->AddGrowableRow(0);
    glparent->SetSizer(sizer);
    sizer->Add(this->currentglarea, 0, wxEXPAND | wxALL, 0);
    sizer->Layout();
}

bool
SoWwGLWidgetP::hasZBuffer() const {
    const bool z_buffer = SoWwGLArea::isGLFeatureAvailable(gl_attributes,
                                                           WX_GL_DEPTH_SIZE);
    return (z_buffer);
}

bool
SoWwGLWidgetP::hasOverlay() const {
    SOWW_STUB();
    return (false);
}

#undef PRIVATE
#undef PUBLIC

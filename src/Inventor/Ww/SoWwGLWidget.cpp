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

#include "Inventor/Ww/SoWwGLWidget.h"
#include "Inventor/Ww/SoWwGLWidgetP.h"

SOWW_OBJECT_ABSTRACT_SOURCE(SoWwGLWidget);

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

SoWwGLWidget::SoWwGLWidget(wxWindow* const parent ,
                           const char * const name ,
                           const SbBool embed ,
                           const int glmodes ,
                           const SbBool build):
        inherited(parent, name, embed),
        waitForExpose(true),
        drawToFrontBuffer(false)
{
    PRIVATE(this) = new SoWwGLWidgetP(this);

    wxGLAttributes &dispAttrs = PRIVATE(this)->glAttributes;
    dispAttrs.PlatformDefaults();//.DoubleBuffer().Depth(32).MinRGBA(8,8,8,8).EndList();
    if(glmodes & SO_GL_DOUBLE)
        dispAttrs.DoubleBuffer();
    if(glmodes & SO_GL_ZBUFFER)
        dispAttrs.Depth(32);
    if(glmodes & SO_GL_RGB)
        dispAttrs.MinRGBA(8, 8, 8, 8);
    dispAttrs.EndList();

    PRIVATE(this)->glSize = SbVec2s(0, 0);
    PRIVATE(this)->glSizeUnscaled = SbVec2s(0, 0);
    PRIVATE(this)->wasresized = false;

    //TODO: PRIVATE(this)->glformat = new QSurfaceFormat(QSurfaceFormat::defaultFormat());
    //TODO: PRIVATE(this)->glformat->setSwapBehavior((glmodes & SO_GL_DOUBLE) ? QSurfaceFormat::DoubleBuffer : QSurfaceFormat::SingleBuffer);
    //TODO: PRIVATE(this)->glformat->setDepthBufferSize((glmodes & SO_GL_ZBUFFER) ? 32 : 0);
    //TODO: //PRIVATE(this)->glformat->setRgba((glmodes & SO_GL_RGB) ? true : false);
    //TODO: PRIVATE(this)->glformat->setStereo((glmodes & SO_GL_STEREO) ? true : false);
    //TODO: bool enableoverlay = (glmodes & SO_GL_OVERLAY) ? true : false;
    //TODO: QGLFormat_setOverlay(PRIVATE(this)->glformat, enableoverlay);

    PRIVATE(this)->glparent = NULL;
    PRIVATE(this)->currentglwidget = NULL;
    PRIVATE(this)->previousglwidget = NULL;
    PRIVATE(this)->currentglarea = NULL;
    PRIVATE(this)->previousglarea = NULL;
    PRIVATE(this)->borderwidget = NULL;

    // TODO: if (! build) { return; }

    this->setClassName("SoWwGLWidget");
    //wxWindow* parentwidget = this->getParentWidget();
    //wxFrame* widget = dynamic_cast<wxFrame *>(this->buildWidget(parent));
    //this->setBaseWidget(widget);
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


wxWindow* SoWwGLWidget::getGLWidget(void) const{ }
wxWindow* SoWwGLWidget::getNormalWidget(void) const{ }
wxWindow* SoWwGLWidget::getOverlayWidget(void) const{ }

SbBool SoWwGLWidget::hasOverlayGLArea(void) const{ }
SbBool SoWwGLWidget::hasNormalGLArea(void) const{ }

unsigned long SoWwGLWidget::getOverlayTransparentPixel(void){ }

void SoWwGLWidget::processEvent(wxEvent* event){ }

wxWindow * SoWwGLWidget::buildWidget(wxWindow* parent){

    if (parent != NULL && this->isTopLevelShell()) {
        // TODO: parent->installEventFilter(PRIVATE(this));
    }

    // TODO:PRIVATE(this)->borderwidget = new QFrame(parent);
    // TODO:this->registerWidget(PRIVATE(this)->borderwidget);

    // TODO:PRIVATE(this)->borderwidget->setFrameStyle(QFrame::Panel | QFrame::Raised);
    // TODO:PRIVATE(this)->borderwidget->setLineWidth(PRIVATE(this)->borderthickness);
    // TODO:PRIVATE(this)->borderwidget->move(0, 0);

    // Remember our parent widget so we can use it in tests in the
    // eventFilter().
    PRIVATE(this)->glparent = parent;

    PRIVATE(this)->buildGLWidget();

    //return PRIVATE(this)->currentglwidget;
    //return PRIVATE(this)->borderwidget;
    return PRIVATE(this)->currentglwidget;
}

void SoWwGLWidget::redrawOverlay(void){ }

void SoWwGLWidget::initGraphic(void){ }
void SoWwGLWidget::initOverlayGraphic(void){ }

void SoWwGLWidget::sizeChanged(const SbVec2s & size){ }
void SoWwGLWidget::widgetChanged(wxWindow* w){ }


// Private data

#include "SoWwGLWidgetP.cpp"

#undef PRIVATE
#undef PUBLIC
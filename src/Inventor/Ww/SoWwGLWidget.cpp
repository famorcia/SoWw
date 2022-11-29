//
// Created by fmorciano on 11/27/22.
//

#include "Inventor/Ww/SoWwGLWidget.h"

SOWW_OBJECT_ABSTRACT_SOURCE(SoWwGLWidget);

SoWwGLWidget::SoWwGLWidget(wxFrame* const parent ,
                           const char * const name ,
                           const SbBool embed ,
                           const int glmodes ,
                           const SbBool build) {

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

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
#include "Inventor/Ww/SoWwComponent.h"
#include "Inventor/Ww/SoWwComponentP.h"
#include "Inventor/Ww/SoAny.h"
#include "Inventor/Ww/SoWwGLWidget.h"
#include "Inventor/Ww/SoWwRenderArea.h"

#define SOWWCOMP_RESIZE_DEBUG 1

#undef PUBLIC
#undef PRIVATE

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

SOWW_OBJECT_ABSTRACT_SOURCE(SoWwComponent);

SoWwComponent::~SoWwComponent(){

}

SoWwComponent::SoWwComponent(wxWindow* const parent,
                             const char * const name,
                             const SbBool embed) {
    PRIVATE(this) = new SoWwComponentP(this);

    PRIVATE(this)->realized = false;
    PRIVATE(this)->shelled = false;
    PRIVATE(this)->widget = NULL;
    PRIVATE(this)->parent = parent;
    PRIVATE(this)->closeCB = NULL;
    PRIVATE(this)->closeCBdata = NULL;
    PRIVATE(this)->visibilitychangeCBs = NULL;
    PRIVATE(this)->fullscreen = false;

    this->setClassName("SoWwComponent");

    PRIVATE(this)->storesize.setValue(-1, -1);

    SoAny::si()->addInternalFatalErrorHandler(SoWwComponentP::fatalerrorHandler,
                                              PRIVATE(this));

    PRIVATE(this)->widgetname = (name ? name :
                                 this->getDefaultWidgetName());

    if (!parent || !embed) {
        // TODO: PRIVATE(this)->parent = (QWidget *) new QMainWindow(NULL, name);
        PRIVATE(this)->embedded = false;
        PRIVATE(this)->shelled = true;
    }
    else {
        PRIVATE(this)->parent = parent;
        PRIVATE(this)->embedded = true;
    }

    // TODO: PRIVATE(this)->parent->installEventFilter(PRIVATE(this));
}

void  SoWwComponent::initClasses(void) {
    SoWwComponent::initClass();
    SoWwGLWidget::initClass();
    SoWwRenderArea::initClass();
#if 0
    SoWwViewer::initClass();
    SoWwFullViewer::initClass();
    SoWwExaminerViewer::initClass();
    SoWwPlaneViewer::initClass();
    SoWwConstrainedViewer::initClass();
    SoWwWalkViewer::initClass();
    SoWwFlyViewer::initClass();
#endif
}

void SoWwComponent::afterRealizeHook(void) {

}

void SoWwComponent::setClassName(const char * const name) {
    PRIVATE(this)->classname = name;
}

void SoWwComponent::setBaseWidget(wxWindow* widget) {

}


void SoWwComponent::show(void) {
    if(SOWW_DEBUG && !PRIVATE(this)->widget) { // debug
        SoDebugError::postWarning("SoWwComponent::show",
                                  "Called while no wxWindow has been set.");
        return;
    }

    if (SOWWCOMP_RESIZE_DEBUG) {  // debug
        SoDebugError::postInfo("SoWwComponent::show-1",
                               "resizing %p: (%d, %d)",
                               PRIVATE(this)->widget,
                               PRIVATE(this)->storesize[0],
                               PRIVATE(this)->storesize[1]);
    }

    if (PRIVATE(this)->shelled) {
        PRIVATE(this)->parent->SetSize(PRIVATE(this)->storesize[0],
                                      PRIVATE(this)->storesize[1]);
    }
    else {
        PRIVATE(this)->widget->SetSize(PRIVATE(this)->storesize[0],
                                      PRIVATE(this)->storesize[1]);
    }

    if (SOWWCOMP_RESIZE_DEBUG) {  // debug
        SoDebugError::postInfo("SoWwComponent::show-2",
                               "resized %p: (%d, %d)",
                               PRIVATE(this)->widget,
                               PRIVATE(this)->widget->GetSize().GetX(),
                               PRIVATE(this)->widget->GetSize().GetY());
    }

    PRIVATE(this)->widget->GetGrandParent()->Show();

    if (SOWWCOMP_RESIZE_DEBUG) {  // debug
        SoDebugError::postInfo("SoWwComponent::show-3",
                               "showed %p: (%d, %d)",
                               PRIVATE(this)->widget,
                               PRIVATE(this)->widget->GetSize().GetX(),
                               PRIVATE(this)->widget->GetSize().GetY());
    }

    // TODO: PRIVATE(this)->widget->raise();

    if (SOWWCOMP_RESIZE_DEBUG) {  // debug
        SoDebugError::postInfo("SoWwComponent::show-4",
                               "raised %p: (%d, %d)",
                               PRIVATE(this)->widget,
                               PRIVATE(this)->widget->GetSize().GetX(),
                               PRIVATE(this)->widget->GetSize().GetY());
    }
    this->sizeChanged(PRIVATE(this)->storesize);
}

void SoWwComponent::hide(void) {

}

void SoWwComponent::setComponentCursor(const SoWwCursor & cursor) {

}

void SoWwComponent::setWidgetCursor(wxWindow* w, const SoWwCursor & cursor) {

}

SbBool SoWwComponent::isFullScreen(void) const {

}

SbBool SoWwComponent::setFullScreen(const SbBool onoff) {

}

SbBool SoWwComponent::isVisible(void) {

}

SbBool SoWwComponent::isTopLevelShell(void) const {

}

wxWindow* SoWwComponent::getWidget(void) const {
    return this->getBaseWidget();
}

wxWindow* SoWwComponent::getBaseWidget(void) const {
    return PRIVATE(this)->widget;
}


wxWindow* SoWwComponent::getParentWidget(void) const {
    return PRIVATE(this)->parent;
}

void SoWwComponent::setSize(const SbVec2s size) {
    if ( PRIVATE(this)->embedded ) {
        //TODO: SoWw::setWidgetSize(this->getBaseWidget(), size);
    }
    else {
        //TODO: SoWw::setWidgetSize(this->getShellWidget(), size);
    }
    PRIVATE(this)->size = size;
}

SbVec2s SoWwComponent::getSize(void) const {
    if ( PRIVATE(this)->embedded ) {
        //TODO: PRIVATE(this)->size = SoWw::getWidgetSize(this->getShellWidget());
    }
    else {
        //TODO: PRIVATE(this)->size = SoWw::getWidgetSize(this->getBaseWidget());
    }
    return PRIVATE(this)->size;
}

void SoWwComponent::setTitle(const char * const title) {

}
const char * SoWwComponent::getTitle(void) const {

}

void SoWwComponent::setIconTitle(const char * const title) {

}

const char * SoWwComponent::getIconTitle(void) const {

}

const char * SoWwComponent::getWidgetName(void) const{

}

const char * SoWwComponent::getClassName(void) const {

}

void SoWwComponent::setWindowCloseCallback(SoWwComponentCB * const func,
                                           void * const user ) {

}


void SoWwComponent::sizeChanged(const SbVec2s & size) {

}

void SoWwComponent::addVisibilityChangeCallback(SoWwComponentVisibilityCB * const func,
                                                void * const user) {

}

void SoWwComponent::removeVisibilityChangeCallback(SoWwComponentVisibilityCB * const func,
                                                   void * const user){

}

#include "Inventor/Ww/SoWwComponentP.cpp"
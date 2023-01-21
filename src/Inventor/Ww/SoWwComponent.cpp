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
#include "Inventor/Ww/SoWwCursor.h"

#include "Inventor/Ww/viewers/SoWwViewer.h"
#include "Inventor/Ww/viewers/SoWwFullViewer.h"
#include "Inventor/Ww/viewers/SoWwExaminerViewer.h"
#include "Inventor/Ww/viewers/SoWwPlaneViewer.h"
#include "Inventor/Ww/viewers/SoWwConstrainedViewer.h"
#include "Inventor/Ww/viewers/SoWwFlyViewer.h"

#include "sowwdefs.h"
#include "SoWwP.h"

#define SOWWCOMP_RESIZE_DEBUG 1

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
        PRIVATE(this)->parent = new wxFrame(NULL, wxID_ANY, name);
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
    SoWwViewer::initClass();
    SoWwExaminerViewer::initClass();
    SoWwPlaneViewer::initClass();
    SoWwConstrainedViewer::initClass();
    SoWwFullViewer::initClass();
    SoWwFlyViewer::initClass();
}

void
SoWwComponent::afterRealizeHook(void) {
    SOWW_STUB();
}

void
SoWwComponent::setClassName(const char * const name) {
    PRIVATE(this)->classname = name;
}

void
SoWwComponent::setBaseWidget(wxWindow* w) {

#ifdef SOWW_DEBUG
    SoDebugError::postInfo("SoWw::setBaseWidget",
                           "%s",
                           SoWwP::dumpWindowData(w).c_str());
#endif

    std::string iconText = this->getDefaultIconTitle();
    std::string widgetName = PRIVATE(this)->widgetname;

    assert(w);

    if (PRIVATE(this)->widget) {
        /* TODO: iconText = (PRIVATE(this)->widget-windowIconText().isEmpty() ?
		iconText :
		PRIVATE(this)->widget->windowIconText());
         */
        widgetName = (PRIVATE(this)->widget->GetName().IsEmpty() ?
                      widgetName :
                      PRIVATE(this)->widget->GetName());

        this->unregisterWidget(PRIVATE(this)->widget);
    }

    PRIVATE(this)->widget = w;

    this->registerWidget(PRIVATE(this)->widget);

#if SOWW_DEBUG && 0
    SoDebugError::postInfo("SoWwComponent::setBaseWidget",
                           "widget: %p, parent: %p", w, PRIVATE(this)->parent);
#endif

    if (!PRIVATE(this)->parent || PRIVATE(this)->parent->IsTopLevel()) {

        if (PRIVATE(this)->widget->GetName() == "") {
            this->setTitle(this->getDefaultTitle());
        }

        SoWw::getShellWidget(this->getWidget())->SetName(iconText);
    }
    PRIVATE(this)->widget->SetName(widgetName);
}

void
SoWwComponent::show(void) {
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
        PRIVATE(this)->parent->SetClientSize(PRIVATE(this)->storesize[0],
                                             PRIVATE(this)->storesize[1]);
    }
    else {
        PRIVATE(this)->widget->SetClientSize(PRIVATE(this)->storesize[0],
                                             PRIVATE(this)->storesize[1]);
    }

    if (SOWWCOMP_RESIZE_DEBUG) {  // debug
        SoDebugError::postInfo("SoWwComponent::show-2",
                               "resized %p: (%d, %d)",
                               PRIVATE(this)->widget,
                               PRIVATE(this)->widget->GetSize().GetX(),
                               PRIVATE(this)->widget->GetSize().GetY());
    }

    PRIVATE(this)->widget->Show();

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

void
SoWwComponent::hide(void) {
    SOWW_STUB();
    PRIVATE(this)->widget->Hide();
}

void
SoWwComponent::setComponentCursor(const SoWwCursor & cursor) {
    SOWW_STUB();
    SoWwComponent::setWidgetCursor(this->getWidget(), cursor);
}

void
SoWwComponent::setWidgetCursor(wxWindow* w, const SoWwCursor & cursor) {

    if(!w) {
        return;
    }
    // FIXME: as this function is called all the time when the cursor is
    // grabbed by the window under X11, we should really compare with
    // the previous cursor before doing anything, to avoid spending
    // unnecessary clockcycles during animation. 20011203 mortene.

    if (cursor.getShape() == SoWwCursor::CUSTOM_BITMAP) {
        const SoWwCursor::CustomCursor * cc = &cursor.getCustomCursor();
        w->SetCursor(*SoWwComponentP::getNativeCursor(cc));
    }
    else {
        switch (cursor.getShape()) {
            case SoWwCursor::DEFAULT:
                w->SetCursor(*wxSTANDARD_CURSOR);
                break;

            case SoWwCursor::BUSY:
                w->SetCursor(*wxHOURGLASS_CURSOR);
                break;

            case SoWwCursor::CROSSHAIR:
                w->SetCursor(*wxCROSS_CURSOR);
                break;

            case SoWwCursor::UPARROW:
                w->SetCursor(*wxCROSS_CURSOR);
                break;

            default:
                assert(false && "unsupported cursor shape type");
                break;
        }
    }
}

SbBool
SoWwComponent::isFullScreen(void) const {
    return (PRIVATE(this)->fullscreen);
}

SbBool
SoWwComponent::setFullScreen(const SbBool onoff) {
    wxWindow * w = this->getShellWidget();
    if (w == NULL) w = this->getParentWidget();
    if (w == NULL) w = this->getWidget();
    if (!w) { return false; }
    wxFrame* frame =  dynamic_cast<wxFrame*>(w);
    if(frame) {
        frame->ShowFullScreen(onoff);
    }
    else {
        return (false);
    }

    PRIVATE(this)->fullscreen = onoff;
    return (PRIVATE(this)->fullscreen);
}

SbBool
SoWwComponent::isVisible(void) {
    bool ret = false;
    if( PRIVATE(this)->widget ) {
        ret = PRIVATE(this)->widget->IsShownOnScreen();
    }
    return (ret);
}

SbBool
SoWwComponent::isTopLevelShell(void) const {
#if SOWW_DEBUG
    if (! PRIVATE(this)->widget) {
        SoDebugError::postWarning("SoWwComponent::isTopLevelShell",
                                  "Called while no wxWindow has been set.");
        return false;
    }
#endif // SOWW_DEBUG
    return PRIVATE(this)->embedded ? false : true;
}

wxWindow*
SoWwComponent::getWidget(void) const {
    return this->getBaseWidget();
}

wxWindow*
SoWwComponent::getBaseWidget(void) const {
    return PRIVATE(this)->widget;
}

wxWindow*
SoWwComponent::getParentWidget(void) const {
    return PRIVATE(this)->parent;
}

void
SoWwComponent::setSize(const SbVec2s size) {

#ifdef SOWW_DEBUG
    SoDebugError::postInfo("SoWw::setSize",
                           " baseWidget %s",
    SoWwP::dumpWindowData(this->getBaseWidget()).c_str());
    SoDebugError::postInfo("SoWw::setSize",
                           " shellWidget %s",
    SoWwP::dumpWindowData(this->getShellWidget()).c_str());
#endif

#if 0
    if ( PRIVATE(this)->embedded ) {
        SoWw::setWidgetSize(this->getBaseWidget(), size);
    }
    else {
        SoWw::setWidgetSize(this->getShellWidget(), size);
    }
#endif
#if 1
    #if SOWW_DEBUG
    if((size[0] <= 0) || (size[1] <= 0)) {
        SoDebugError::postWarning("SoWwComponent::setSize",
                                  "Invalid size setting: <%d, %d>.",
                                  size[0], size[1]);
        return;
    }
#endif // SOWW_DEBUG

#if SOWWCOMP_RESIZE_DEBUG  // debug
    SoDebugError::postInfo("SoWwComponent::setSize",
                           "resize %p: (%d, %d)",
                           PRIVATE(this)->widget,
                           size[0], size[1]);
#endif // debug
    const SbBool yetbuilt = (this->getWidget() != NULL);
    if (yetbuilt) {
        wxWindow * shell = this->getShellWidget();
        if (shell) {
            // shell->SetSize(size[0], size[1]);
            shell->SetClientSize(wxSize(size[0], size[1]));
        }
    }
#endif
    PRIVATE(this)->storesize = size;
    this->sizeChanged(size);
}

SbVec2s
SoWwComponent::getSize(void) const {
    return (PRIVATE(this)->storesize);
}

void
SoWwComponent::setTitle(const char * const title) {
    SoWw::getShellWidget(this->getWidget())->SetName(title);
}

const char *
SoWwComponent::getTitle(void) const {
    return (SoWw::getShellWidget(this->getWidget())->GetName());
}

void
SoWwComponent::setIconTitle(const char * const title) {
    SOWW_STUB();
}

const char *
SoWwComponent::getIconTitle(void) const {
    SOWW_STUB();
    return ("");
}

const char *
SoWwComponent::getWidgetName(void) const{
    SOWW_STUB();
    return ("");
}

const char *
SoWwComponent::getClassName(void) const {
    SOWW_STUB();
    return ("");
}

void
SoWwComponent::setWindowCloseCallback(SoWwComponentCB * const func,
                                      void * const user ) {
    SOWW_STUB();
}

void
SoWwComponent::sizeChanged(const SbVec2s & size) {
    SOWW_STUB();
}

void
SoWwComponent::addVisibilityChangeCallback(SoWwComponentVisibilityCB * const func,
                                           void * const user) {
    SOWW_STUB();
}

void
SoWwComponent::removeVisibilityChangeCallback(SoWwComponentVisibilityCB * const func,
                                              void * const user){
    SOWW_STUB();
}

#undef PUBLIC
#undef PRIVATE

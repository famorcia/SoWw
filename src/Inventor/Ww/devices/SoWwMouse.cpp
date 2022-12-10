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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <X11/X.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>

#include <Inventor/Ww/devices/SoWwMouse.h>
#include "Inventor/Ww/devices/SoGuiMouseP.h"
#include "sowwdefs.h"

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->pub)

class SoWwMouseP : public SoGuiMouseP {
public:
    SoWwMouseP(SoWwMouse * p) : SoGuiMouseP(p) { }

    //SoLocation2Event * makeLocationEvent(XMotionEvent * event);
    //SoMouseButtonEvent * makeButtonEvent(XButtonEvent * event, SoButtonEvent::State state);
};

SoWwMouse::SoWwMouse(int mask ) {
    PRIVATE(this) = new SoWwMouseP(this);
    PRIVATE(this)->eventmask = mask;
}

SoWwMouse::~SoWwMouse(void) {

}

void SoWwMouse::enable(wxWindow* widget, SoWwEventHandler * handler, void * closure)    {
    SOWW_STUB();
}

void SoWwMouse::disable(wxWindow* widget, SoWwEventHandler * handler, void * closure) {
    SOWW_STUB();
}

const SoEvent * SoWwMouse::translateEvent(wxEvent& event) {

    SoEvent * conv = NULL;

    wxMouseEvent* mouse_event = dynamic_cast<wxMouseEvent*>(&event);
    if(!mouse_event) {
        SoDebugError::postInfo("SoWwMouse::translateEvent",
                               "is not a mouse event!");

        return (conv);
    }

#if 1
#if 0
    QWheelEvent * wheelevent =
            (event->type() == QEvent::Wheel) ? (QWheelEvent *)event : NULL;

    QMouseEvent * mouseevent =
            ((event->type() == QEvent::MouseButtonDblClick) ||
             (event->type() == QEvent::MouseButtonPress) ||
             (event->type() == QEvent::MouseButtonRelease) ||
             (event->type() == QEvent::MouseMove)) ?
            (QMouseEvent *)event : NULL;

    if (!wheelevent && !mouseevent) return NULL;

    // Convert wheel mouse events to Coin SoMouseButtonEvents.
    //
    // FIXME: should consider adding an SoMouseWheel event to Coin?
    // 20020821 mortene. (idea mentioned by Florian Link on
    // coin-discuss.)

#ifdef HAVE_SOMOUSEBUTTONEVENT_BUTTON5
    if (wheelevent) {
#if QT_VERSION >= 0x050700
        if (wheelevent->angleDelta().y() > 0)
      PRIVATE(this)->buttonevent->setButton(wheelevent->inverted() ? SoMouseButtonEvent::BUTTON5 : SoMouseButtonEvent::BUTTON4);
    else if (wheelevent->angleDelta().y() < 0)
      PRIVATE(this)->buttonevent->setButton(wheelevent->inverted() ? SoMouseButtonEvent::BUTTON4 : SoMouseButtonEvent::BUTTON5);
#elif QT_VERSION >= 0x050000
        if (wheelevent->angleDelta().y() > 0)
      PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON4);
    else if (wheelevent->angleDelta().y() < 0)
      PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON5);
#else
        if (wheelevent->delta() > 0)
            PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON4);
        else if (wheelevent->delta() < 0)
            PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON5);
#endif // QT_VERSION

#if SoWw_DEBUG
            else {
      SoDebugError::postInfo("SoWwMouse::translateEvent",
                             "event, but no movement");
    }
#endif // SoWw_DEBUG
        PRIVATE(this)->buttonevent->setState(SoButtonEvent::DOWN);
        conv = PRIVATE(this)->buttonevent;
    }
#endif // HAVE_SOMOUSEBUTTONEVENT_BUTTON5

    // Check for mousebutton press/release. Note that mousebutton
    // doubleclick events are handled by converting them to two
    // press/release events. In other words: it's the user's
    // responsibility to translate pairs of singleclicks to
    // doubleclicks, if doubleclicks have a special meaning in the
    // application.

    // Qt actually sends this series of events upon dblclick:
    // QEvent::MouseButtonPress, QEvent::MouseButtonRelease,
    // QEvent::MouseButtonDblClick, QEvent::MouseButtonRelease.
    //
    // This was reported to Troll Tech as a possible bug, but was
    // confirmed by TT support to be the intended behavior.
#endif
    if (((mouse_event->ButtonDClick()) ||
         (mouse_event->ButtonDown()) ||
         (mouse_event->ButtonUp())) &&
        (PRIVATE(this)->eventmask & (BUTTON_PRESS | BUTTON_RELEASE))) {

        // Which button?
        switch (mouse_event->GetButton()) {
            case wxMOUSE_BTN_LEFT:
                PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON1);
                break;
            case wxMOUSE_BTN_RIGHT:
                PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON2);
                break;
            case wxMOUSE_BTN_MIDDLE:
                PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON3);
                break;
                // Not sure if this can actually happen.
            case wxMOUSE_BTN_ANY:
            default:
                PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::ANY);
                break;
        }

        // Press or release?
        if (mouse_event->ButtonUp())
            PRIVATE(this)->buttonevent->setState(SoButtonEvent::UP);
        else
            PRIVATE(this)->buttonevent->setState(SoButtonEvent::DOWN);

        conv = PRIVATE(this)->buttonevent;
    }

    // Check for mouse movement.
    if (mouse_event->Dragging() || mouse_event->Moving()) {
        conv = PRIVATE(this)->locationevent;
    }

    // Common settings for SoEvent superclass.
    if (conv) {
        // Modifiers
        if (mouse_event) {
#if 0
            conv->setShiftDown(mouseevent->state() & Qt::ShiftButton);
            conv->setCtrlDown(mouseevent->state() & Qt::ControlButton);
            conv->setAltDown(mouseevent->state() & Qt::AltButton);
#endif

            this->setEventPosition(conv,
                                   mouse_event->GetX(),
                                   mouse_event->GetY());
        }
        else { // wheelevent
#if 0
            conv->setShiftDown(wheelevent->state() & Qt::ShiftButton);
            conv->setCtrlDown(wheelevent->state() & Qt::ControlButton);
            conv->setAltDown(wheelevent->state() & Qt::AltButton);
            this->setEventPosition(conv, wheelevent->x(), wheelevent->y());
#endif
        }

        conv->setTime(SbTime::getTimeOfDay());
    }
#endif
    return conv;
}

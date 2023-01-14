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

#include "Inventor/Ww/SoWwP.h"

#include <Inventor/SoDB.h>
#include <Inventor/SbTime.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/sensors/SoSensorManager.h>

#include <iostream>
#include <wx/sizer.h>
#include <sstream>

class SoWxApp : public  wxApp {
public:

    virtual bool
    OnInit() wxOVERRIDE {
        if ( !wxApp::OnInit() )
            return false;
        return true;
    }
};

wxTimer * SoWwP::timerqueuetimer = 0;
wxTimer * SoWwP::delaytimeouttimer = 0;

SoWwP::SoWwP() {
    init = false;
    main_frame = 0;
    main_app = 0;
}

void
SoWwP::buildWxApp() {
    if(!main_app) {
        setWxApp( new SoWxApp);
    } else if (SOWW_DEBUG && 0){
        SoDebugError::postWarning("SoWwP::buildWxApp",
                               "wxApp already built");

    }
}

void
SoWwP::setWxApp(wxAppConsole* app) {
    main_app = app;
}

/**
 * if an app is not already available build and return
 */
wxAppConsole*
SoWwP::getWxApp() const {
    return (main_app);
}

void
SoGuiP::sensorQueueChanged(void *) {
    SoWwP::instance()->sensorQueueChanged();
}

class TimerQueueTimer : public wxTimer {
public:
    virtual void
    Notify() {
        if (SOWW_DEBUG && 0) { // debug
            SoDebugError::postInfo("TimerQueueTimer::Notify",
                                   "processing timer queue");
            SoDebugError::postInfo("TimerQueueTimer::Notify",
                                   "is %s",
                                   this->IsRunning() ?
                                   "active" : "inactive");
        }

        SoDB::getSensorManager()->processTimerQueue();

        // The change callback is _not_ called automatically from
        // SoSensorManager after the process methods, so we need to
        // explicitly trigger it ourselves here.
        SoGuiP::sensorQueueChanged(NULL);
    }
};

// The delay sensor timeout point has been reached, so process the
// delay queue even though the system is not idle (to avoid
// starvation).
class DelayTimeoutTimer : public wxTimer {
public:
    virtual void
    Notify() {
        if (SOWW_DEBUG && 0) { // debug
            SoDebugError::postInfo("DelayTimeoutTimer::Notify",
                                   "processing delay queue");
            SoDebugError::postInfo("DelayTimeoutTimer::Notify", "is %s",
                                   this->IsRunning() ?
                                   "active" : "inactive");
        }

        SoDB::getSensorManager()->processTimerQueue();
        SoDB::getSensorManager()->processDelayQueue(false);

        // The change callback is _not_ called automatically from
        // SoSensorManager after the process methods, so we need to
        // explicitly trigger it ourselves here.
        SoGuiP::sensorQueueChanged(NULL);
    }
};

void
SoWwP::sensorQueueChanged(void) {
    // We need three different mechanisms to interface Coin sensor
    // handling with Qt event handling, which are:
    //
    // 1. Detect when the application is idle and then empty the
    // delay-queue completely for delay-sensors -- handled by
    // SoWwP::onIdle.
    //
    // 2. Detect when one or more timer-sensors are ripe and trigger
    // those -- handled by SoWwP::timerqueuetimer.
    //
    // 3. On the "delay-sensor timeout interval", empty all highest
    // priority delay-queue sensors to avoid complete starvation in
    // continually busy applications -- handled by
    // SoWwP::delaytimeouttimer.


    // Allocate wx timers on first call.
    SoWwP::initTimers();

    SoSensorManager * sm = SoDB::getSensorManager();

    // Set up timer queue timeout if necessary.

    SbTime t;
    if (sm->isTimerSensorPending(t)) {
        SbTime interval = t - SbTime::getTimeOfDay();

        // We also want to avoid setting it to 0.0, as that has a special
        // semantic meaning: trigger only when the application is idle and
        // event queue is empty -- which is not what we want to do here.
        //
        // So we clamp it, to a small positive value:
        if (interval.getValue() <= 0.0) { interval.setValue(1.0/5000.0); }

        if (SOWW_DEBUG && 0) { // debug
            SoDebugError::postInfo("SoWwP::sensorQueueChanged",
                                   "timersensor pending, interval %f",
                                   interval.getValue());
        }

        // Change interval of timerqueuetimer when head node of the
        // timer-sensor queue of SoSensorManager changes.
        SoWwP::timerqueuetimer->Start((int)interval.getMsecValue(), true);
    }
        // Stop timerqueuetimer if queue is completely empty.
    else if (SoWwP::timerqueuetimer->IsRunning()) {
        SoWwP::timerqueuetimer->Stop();
    }


    // Set up idle notification for delay queue processing if necessary.

    if (sm->isDelaySensorPending()) {
        if (SOWW_DEBUG && 0) { // debug
            SoDebugError::postInfo("SoWwP::sensorQueueChanged",
                                   "delaysensor pending");
        }

        if (!SoWwP::delaytimeouttimer->IsRunning()) {
            const SbTime & delaySensorTimeout = SoDB::getDelaySensorTimeout();
            if (delaySensorTimeout != SbTime::zero()) {
                unsigned long timeout = delaySensorTimeout.getMsecValue();
                SoWwP::delaytimeouttimer->Start((int)timeout, true);
            }
        }
    }
    else {
        if (SoWwP::delaytimeouttimer->IsRunning())
            SoWwP::delaytimeouttimer->Stop();
    }
}

SoWwP *
SoWwP::instance() {
    static SoWwP singleton;
    return (&singleton);
}

bool
SoWwP::isInitialized() const {
    return (init);
}

void
SoWwP::setInitialize(bool i) {
    init = i;
}

wxWindow *
SoWwP::getMainFrame() const {
    return (main_frame);
}

void
SoWwP::setMainFrame(wxWindow * frame) {
    main_frame = frame;
}

#define INIT_TIMER(timer_name, timer_type)  \
    if (!timer_name) {                      \
        timer_name = new timer_type;        \
    }                                       \
    assert(timer_name != 0)

void
SoWwP::initTimers() {
    static bool are_initialized = false;

    if(!are_initialized) {
        INIT_TIMER(SoWwP::timerqueuetimer, TimerQueueTimer);
        INIT_TIMER(SoWwP::delaytimeouttimer, DelayTimeoutTimer);
        are_initialized = true;
    }
}

#undef INIT_TIMER

#define STOP_TIMER(timer_name) if(timer_name) timer_name->Stop()

void
SoWwP::stopTimers() {
    STOP_TIMER(SoWwP::timerqueuetimer);
    STOP_TIMER(SoWwP::delaytimeouttimer);
}

#undef STOP_TIMER

void
SoWwP::finish() {
    stopTimers();
}

std::string
nameToString(const wxWindow* w) {
    std::ostringstream  oss;
    oss<<"name:\""<<w->GetName()<<"\"";
    oss<<" ptr:"<<std::hex<<w;
    return (oss.str());
}

std::string
sizeToString(const wxSize& s) {
    std::ostringstream  oss;
    oss<<"size:("<<s.GetX()<<","<<s.GetY()<<")";
    return (oss.str());
}

std::string
dumpSizer(wxSizer* sizer,
          const std::string& prefix) {
    std::string out;
    out += prefix;
    out += " -> sizer:";
    out += sizeToString(sizer->GetSize());
    return (out);
}

std::string
dumpData(const wxWindow* w,
         const std::string& prefix="") {
    std::string out;
    out += prefix;
    out += nameToString(w);
    out += " ";
    out += sizeToString(w->GetSize());
    out += " minClientSize:";
    out += sizeToString(w->GetMinClientSize());
    if(w->GetSizer()) {
        out += dumpSizer(w->GetSizer(), prefix);
#if 0
        wxSizerItemList list = w->GetSizer()->GetChildren();
        wxSizerItemList::compatibility_iterator node = list.GetFirst();
        while(node) {
            if(node->GetData() && node->GetData()->GetSizer())
                dumpSizer(node->GetData()->GetSizer(), prefix);
           // if(node->GetData() && node->GetData()->GetWindow())
            //    dumpData(node->GetData()->GetWindow(), prefix);
            node = node->GetNext();
        }
#endif
    }
    return (out);
}

std::string
dumpWindowDataImpl(const wxWindow* window, int level) {
    if(window == 0) {
        return ("windows is null\n");
    }

    std::string out;
    std::string tabs;
    for(int i=0;i<level;++i)
        tabs+='-';

    out += "\n";
    out += dumpData(window, tabs);

    const wxWindowList& windows_list =  window->GetWindowChildren();
    wxWindowList::compatibility_iterator node = windows_list.GetFirst();
    while (node) {
        wxWindow *win = node->GetData();
        out += dumpWindowDataImpl(win, level+1);
        node = node->GetNext();
    }
    return (out);
}

std::string
SoWwP::dumpWindowData(const wxWindow* window) {
    return (dumpWindowDataImpl(window, 0));
}

void
SoWwP::onIdle(wxIdleEvent&) {
#if SOWW_DEBUG && 0
    SoDebugError::postInfo("SoWwP::onIdle",
                               "idlesensor pending");
#endif

    SoDB::getSensorManager()->processTimerQueue();
    SoDB::getSensorManager()->processDelayQueue(true);

    // The change callback is _not_ called automatically from
    // SoSensorManager after the process methods, so we need to
    // explicitly trigger it ourselves here.
    SoGuiP::sensorQueueChanged(NULL);
}

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

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SoDB.h>
#include <Inventor/SbPList.h>
#include <Inventor/sensors/SoSensorManager.h>
#include <Inventor/SbTime.h>

class SoWxApp : public  wxApp {
public:

    virtual bool OnInit() wxOVERRIDE {
        if ( !wxApp::OnInit() )
            return false;
        return true;
    }
};

wxTimer * SoWwP::timerqueuetimer = 0;
wxTimer * SoWwP::idletimer = 0;
wxTimer * SoWwP::delaytimeouttimer = 0;

wxApp* SoWwP::buildWxApp() {
    return (new SoWxApp);
}

void
SoGuiP::sensorQueueChanged(void *)
{
    SoWwP::instance()->sensorQueueChanged();
}

class TimerQueueTimer : public wxTimer {
public:
    virtual void Notify() {
        if (SOWW_DEBUG && 0) { // debug
            SoDebugError::postInfo("SoWw::timedOutSensor",
                                   "processing timer queue");
            SoDebugError::postInfo("SoWw::timedOutSensor",
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

class IdleTimer : public wxTimer {
public:
    virtual void Notify() {

        SoDB::getSensorManager()->processTimerQueue();
        SoDB::getSensorManager()->processDelayQueue(true);

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
    virtual void Notify() {
        if (SOWW_DEBUG && 0) { // debug
            SoDebugError::postInfo("SoWw::delaytimeoutSensor",
                                   "processing delay queue");
            SoDebugError::postInfo("SoWwP::delaytimeouttimer", "is %s",
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
SoWwP::sensorQueueChanged(void)
{
    // We need three different mechanisms to interface Coin sensor
    // handling with Qt event handling, which are:
    //
    // 1. Detect when the application is idle and then empty the
    // delay-queue completely for delay-sensors -- handled by
    // SoWwP::idletimer.
    //
    // 2. Detect when one or more timer-sensors are ripe and trigger
    // those -- handled by SoWwP::timerqueuetimer.
    //
    // 3. On the "delay-sensor timeout interval", empty all highest
    // priority delay-queue sensors to avoid complete starvation in
    // continually busy applications -- handled by
    // SoWwP::delaytimeouttimer.


    // Allocate wx timers on first call.

    if (!SoWwP::timerqueuetimer) {
        SoWwP::timerqueuetimer = new TimerQueueTimer;
        SoWwP::idletimer = new IdleTimer;
        SoWwP::delaytimeouttimer = new DelayTimeoutTimer;
    }

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
            SoDebugError::postInfo("SoWw::sensorQueueChanged",
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
            SoDebugError::postInfo("SoWw::sensorQueueChanged",
                                   "delaysensor pending");
        }

        // Start idletimer at 0 seconds in the future. -- That means it will
        // trigger when the Qt event queue has been run through, i.e. when
        // the application is idle.
        // TODO: use idle time of wxWidgets, now every 1/24 seconds
        if (!SoWwP::idletimer->IsRunning()) {
            SbTime idleTime;
            idleTime.setValue(1.0/24.0);
            SoWwP::idletimer->Start(idleTime.getMsecValue(), true);
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
        if (SoWwP::idletimer->IsRunning())
            SoWwP::idletimer->Stop();
        if (SoWwP::delaytimeouttimer->IsRunning())
            SoWwP::delaytimeouttimer->Stop();
    }
}

SoWwP *SoWwP::instance() {
    static SoWwP singleton;
    return (&singleton);
}

SoWwP::SoWwP() {
    init = false;
    main_frame = 0;
}

bool SoWwP::isInitialized() const {
    return (init);
}

void SoWwP::setInitialize(bool i) {
    init = i;
}

SoWwFrame *SoWwP::getMainFrame() const {
    return (main_frame);
}

void SoWwP::setMainFrame(SoWwFrame * frame) {
    main_frame = frame;
}

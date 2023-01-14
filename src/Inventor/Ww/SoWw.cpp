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

#include "Inventor/Ww/SoWw.h"
#include "Inventor/Ww/SoWwP.h"

#include "sowwdefs.h"

wxWindow*
SoWw::init(int & argc,
           char ** argv,
           const char * appname,
           const char * classname) {

    // retrieve the instance
    SoWwP::instance();

    if (SOWW_DEBUG && SoWwP::instance()->isInitialized()) {
        SoDebugError::postWarning("SoWw::init",
                                  "This method should be called only once.");
        return SoWwP::instance()->getMainFrame();
    }

    // Call all the code for initializing Coin data
    SoWwP::commonInit();

    // if wxApp is not already created
    if (wxApp::GetInstance() == NULL) {
        // Set up the QApplication instance which we have derived into a
        // subclass to catch spaceball events.
        SoWwP::instance()->buildWxApp();
        wxApp::SetInstance(SoWwP::instance()->getWxApp() );
        wxEntryStart( argc, argv );
        wxTheApp->CallOnInit();
    }
    else {
        // The user already set one up for us.
        // so nothing to do
        SoWwP::instance()->setWxApp(wxApp::GetInstance());
    }

    SoWwP::instance()->setMainFrame( new wxFrame(0,
                                                   wxID_ANY,
                                                   appname));

    assert(SoWwP::instance());
    assert(SoWwP::instance()->getWxApp());
    SoWwP::instance()->getWxApp()->Bind(wxEVT_IDLE, &SoWwP::onIdle,  SoWwP::instance());
    SoDB::getSensorManager()->setChangedCallback(SoGuiP::sensorQueueChanged,
                                                 NULL);

    SoWwP::instance()->setInitialize(true);
#ifdef SOWW_DEBUG
    SoDebugError::postInfo("SoWw::init",
                           "%s",
                           SoWwP::dumpWindowData(SoWwP::instance()->getMainFrame()).c_str());
#endif
    return (SoWwP::instance()->getMainFrame());
}

void
SoWw::init(wxWindow* toplevelwidget) {
#ifdef COIN_IV_EXTENSIONS
#define COIN_IV_EXTENSION(ext) ext::initClass();
    COIN_IV_EXTENSIONS
#undef COIN_IV_EXTENSION
#endif

    if (SOWW_DEBUG && SoWwP::instance()->isInitialized()) {
        SoDebugError::postWarning("SoWw::init",
                                  "This method should be called only once.");
        return;
    }

    // Call all the code for initializing Coin data
    SoWwP::commonInit();

    // if wxApp is not already created
    if (wxApp::GetInstance() == NULL) {
        // Set up the QApplication instance which we have derived into a
        // subclass to catch spaceball events.
        SoWwP::instance()->buildWxApp();
        wxApp::SetInstance(SoWwP::instance()->getWxApp() );
        static const char * dummyargv[1];
        dummyargv[0] = "SoWw";
        int argc = 1;
        wxEntryStart( argc, (char **)dummyargv );
        wxTheApp->CallOnInit();
    }
    else {
        // The user already set one up for us.
        // so nothing to do
        SoWwP::instance()->setWxApp(wxApp::GetInstance());
    }

    SoWwP::instance()->setMainFrame( toplevelwidget );
    SoWwP::instance()->getWxApp()->Bind(wxEVT_IDLE, &SoWwP::onIdle,  SoWwP::instance());
    SoDB::getSensorManager()->setChangedCallback(SoGuiP::sensorQueueChanged,
                                                 NULL);

    SoWwP::instance()->setInitialize(true);
#ifdef SOWW_DEBUG
    SoDebugError::postInfo("SoWw::init",
                           "%s",
                           SoWwP::dumpWindowData(SoWwP::instance()->getMainFrame()).c_str());
#endif
}

/**
 * mainLoop is required only if an external app is not available
 * (embedded in already existing window).
 */
void
SoWw::mainLoop(void) {
    wxTheApp->OnRun();

    // To avoid getting any further invocations of
    // SoGuiP::sensorQueueChanged() (which would re-allocate the timers
    // we destruct below). This could for instance happen when
    // de-coupling the scenegraph camera, triggering a notification
    // chain through the scenegraph.
    SoDB::getSensorManager()->setChangedCallback(NULL, NULL);
    // turn off timers
    SoWwP::instance()->finish();

    // only if app is built by SoWw perform exit and cleanup
    if(SoWwP::instance()->getWxApp()) {
        wxTheApp->OnExit();
        wxEntryCleanup();
    }
}

void
SoWw::done() {

#if 0 // FIXME: These methods exist in TGS Inventor. We should add
    // them, and then call them from here. 20060210 kyrah
  SoInteraction::finish();
  SoNodeKit::finish();
#endif
    SoDB::finish();
}

void
SoWw::show(wxWindow* const widget) {
    widget->Show();
}

void
SoWw::createSimpleErrorDialog(wxWindow* widget,
                              const char * title,
                              const char * string1,
                              const char * string2 ) {
    SOWW_STUB();
}

wxWindow*
SoWw::getShellWidget(const wxWindow* w) {
    return (wxGetTopLevelParent((wxWindowBase *) w));
#if 0
    wxWindow* p = const_cast<wxWindow*>(w);
    while (p !=  NULL) {
        wxFrame* top_frame = dynamic_cast<wxFrame*>(p);
        if ( top_frame ) {
            return p;
        }
        p = p->GetParent();
    }
#if SOWW_DEBUG && 0 // debug
    SoDebugError::postInfo("SoWw::getShellWidget",
                         "couldn't find shell for widget at %p", widget);
#endif // debug
    return (NULL);
#endif
}


void
SoWw::setWidgetSize(wxWindow* const widget, const SbVec2s size) {

    if ( widget ) {
        widget->SetSize(size[0], size[1]);
    }
#if SOWW_DEBUG
    else  {
        SoDebugError::postWarning("SoWw::setWidgetSize",
                                  "null widget on setting: <%d, %d>.",
                                  size[0], size[1]);
    }
#endif // SOWW_DEBUG
}

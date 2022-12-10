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

#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/SoInteraction.h>
#include "Inventor/Ww/SoWw.h"
#include "Inventor/Ww/SoWwP.h"
#include "Inventor/Ww/SoWwFrame.h"

#include "wx/wx.h"
#include "sowwdefs.h"

#include <wx/frame.h>
#include <wx/defs.h>

wxWindow* SoWw::init(int & argc,
                     char ** argv,
                     const char * appname,
                     const char * classname) {

    if (SOWW_DEBUG && SoWwP::instance()->isInitialized()) {
        SoDebugError::postWarning("SoWw::init",
                                  "This method should be called only once.");
        return SoWwP::instance()->getMainFrame();
    }

    // Call all the code for initializing Coin data
    SoWwP::commonInit();


    // "qApp" is a global variable from the Qt library, pointing to the
    // single QApplication instance in a Qt-application.
    if (wxApp::GetInstance() == NULL) {
        // Set up the QApplication instance which we have derived into a
        // subclass to catch spaceball events.
        wxApp::SetInstance( SoWwP::instance()->buildWxApp() );
        wxEntryStart( argc, argv );
        wxTheApp->CallOnInit();
    }
    else {
        // The user already set one up for us.
        //
        // FIXME: should somehow warn about the fact that spaceball events
        // will not be caught, if a spaceball is attempted used. 20020619 mortene.

    }

    SoWwP::instance()->setMainFrame( new SoWwFrame(0,
                                    appname,
                                    wxDefaultPosition,
                                    wxSize(400,400)));

    SoDB::getSensorManager()->setChangedCallback(SoGuiP::sensorQueueChanged, NULL);

    return SoWwP::instance()->getMainFrame();
}

void SoWw::init(wxWindow* toplevelwidget) {
    SOWW_STUB();
}

void SoWw::mainLoop(void) {
    wxTheApp->OnRun();
    wxTheApp->OnExit();
    wxEntryCleanup();
}

void  SoWw::show(wxWindow* const widget) {
    widget->Show();
}

void    SoWw::createSimpleErrorDialog(wxWindow* widget,
                                      const char * title,
                                      const char * string1,
                                      const char * string2 ) {

}

wxWindow*   SoWw::getShellWidget(const wxWindow* w) {

}

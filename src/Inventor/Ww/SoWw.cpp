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

#include "wx/wx.h"
#include "sowwdefs.h"

#include <wx/frame.h>
#include <wx/defs.h>

class MyFrame : public wxFrame
{
public:
    MyFrame(wxFrame *frame,
            const wxString& title,
            const wxPoint& pos,
            const wxSize& size,
            long style = wxDEFAULT_FRAME_STYLE);

    void OnPaint(wxPaintEvent& event) {
        std::clog<<__PRETTY_FUNCTION__ <<std::endl;
        event.Skip();
    }
    void OnSize(wxSizeEvent& event) {
        std::clog<<__PRETTY_FUNCTION__ <<std::endl;
        event.Skip();
    }

wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
                EVT_SIZE(MyFrame::OnSize)
                EVT_PAINT(MyFrame::OnPaint)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(wxFrame *frame,
                 const wxString& title,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style)
        : wxFrame(frame, wxID_ANY, title, pos, size, style)
{
}

wxWindow* SoWw::init(int & argc,
                     char ** argv,
                     const char * appname,
                     const char * classname) {

    // Must do this here so SoDebugError is initialized before it could
    // be attempted used.
    if (!SoDB::isInitialized()) { SoDB::init(); }
    // these aren't necessary - they are initialized in the SoWw::init method above
    // which is invoked below
    SoNodeKit::init();
    SoInteraction::init();

    /*
    if (SOWW_DEBUG && (SoWwP::appobject || SoWwP::mainwidget)) {
        SoDebugError::postWarning("SoWw::init",
                                  "This method should be called only once.");
        return SoWwP::mainwidget;
    }
     */

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

    wxFrame* aFrame = new MyFrame(0,
                                  appname,
                                  wxDefaultPosition,
                                  wxSize(400,400));

    SoDB::getSensorManager()->setChangedCallback(SoGuiP::sensorQueueChanged, NULL);

    return aFrame;
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

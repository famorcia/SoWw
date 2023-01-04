/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

/*
  Demonstrates embedding of an SoQtExaminerViewer within a simple
  widget hierarchy.
*/

/***********************************************************************/

#include "Inventor/Ww/SoWw.h"
#include "Inventor/Ww/viewers/SoWwExaminerViewer.h"
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSeparator.h>

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/splitter.h>
#include <wx/sizer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MyMainWindow
///////////////////////////////////////////////////////////////////////////////
class MyMainWindow : public wxFrame
{
private:

protected:
    wxSplitterWindow* m_splitter1;
    wxPanel* m_panel19;
    wxPanel* m_panel20;

public:

    MyMainWindow( wxWindow* parent,
                  wxWindowID id = wxID_ANY,
                  const wxString& title = wxEmptyString,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxSize( 500,300 ),
                  long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL )
                  : wxFrame( parent, id, title, pos, size, style )
    {
        this->SetSizeHints( wxDefaultSize, wxDefaultSize );

        wxBoxSizer* bSizer12;
        bSizer12 = new wxBoxSizer( wxHORIZONTAL );

        m_splitter1 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
        m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( MyMainWindow::m_splitter1OnIdle ), NULL, this );

        m_panel19 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        m_panel20 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        m_splitter1->SplitVertically( m_panel19, m_panel20, 0 );
        bSizer12->Add( m_splitter1, 1, wxEXPAND, 5 );


        this->SetSizer( bSizer12 );
        this->Layout();

        this->Centre( wxBOTH );
    }

    ~MyMainWindow()
    {
    }

    void m_splitter1OnIdle( wxIdleEvent& )
    {
        m_splitter1->SetSashPosition( 0 );
        m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MyMainWindow::m_splitter1OnIdle ), NULL, this );
    }

};

#if 0
class MyMainWindow : public QMainWindow {
public:
  MyMainWindow(void)
    {
      QSplitter * split = new QSplitter(this);

      QListView * listview = new QListView(split);
      QWidget * glw = new QWidget(split);

      // Construct a simple scenegraph.

      SoSeparator * root = new SoSeparator;

      SoCone * cone = new SoCone;
      root->addChild(cone);

      // Add the examinerviewer.

      SoWwExaminerViewer * examinerviewer = new SoWwExaminerViewer(glw);
      examinerviewer->setSceneGraph(root);

      split->resize(640, 480);
      this->resize(640, 480);
    }
};
#endif
/***********************************************************************/

int
main(int argc, char ** argv)
{
  // Initialize Ww and SoWw.

  // You should not create a QApplication instance if you want
  // to receive spaceball events.
  //  QApplication app(argc, argv);

  SoWw::init((wxWindow *)NULL);

  // Set up scrollview window.
  MyMainWindow * vp = new MyMainWindow(0);

  // Map window.
  vp->Show();
  // Set termination condition.
  // QObject::connect(qApp, SIGNAL(lastWindowClosed()), qApp, SLOT(quit()));
  // Start event loop.
  SoWw::mainLoop();

  return 0;
}

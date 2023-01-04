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

#include "wx/wx.h"
#include "wx/button.h"
#include "wx/gbsizer.h"

#include "common/SimpleFrame.h"

// Define a new application type
class MyApp : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE {
        if ( !wxApp::OnInit() )
            return false;

        SimpleFrame *aFrame = new SimpleFrame(NULL,
                                              "thumb wheel",
                                              wxDefaultPosition,
                                              wxSize(600   ,600));

#if 1
        wxGridBagSizer* gbSizer2;
        gbSizer2 = new wxGridBagSizer( 0, 0 );
        gbSizer2->SetFlexibleDirection( wxBOTH );
        gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );

        wxPanel* m_panel8;
        wxPanel* m_panel81;
        wxPanel* m_panel82;
        wxPanel* m_panel16;

        m_panel8 = new wxPanel( aFrame, wxID_ANY, wxDefaultPosition, wxSize( 10,200 ), wxTAB_TRAVERSAL );
        m_panel8->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
        m_panel8->SetMinSize( wxSize( 10,200 ) );
        m_panel8->SetMaxSize( wxSize( 10,200 ) );

        gbSizer2->Add( m_panel8, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND | wxALL, 5 );

        m_panel81 = new wxPanel( aFrame, wxID_ANY, wxDefaultPosition, wxSize( 10,200 ), wxTAB_TRAVERSAL );
        m_panel81->SetBackgroundColour( wxColour( 19, 229, 220 ) );
        m_panel81->SetMinSize( wxSize( 10,200 ) );
        m_panel81->SetMaxSize( wxSize( 10,200 ) );

        gbSizer2->Add( m_panel81, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxEXPAND | wxALL, 5 );

        m_panel82 = new wxPanel( aFrame, wxID_ANY, wxDefaultPosition, wxSize( 10,200 ), wxTAB_TRAVERSAL );
        m_panel82->SetBackgroundColour( wxColour( 171, 73, 73 ) );
        m_panel82->SetMinSize( wxSize( 10,200 ) );
        m_panel82->SetMaxSize( wxSize( 10,200 ) );

        gbSizer2->Add( m_panel82, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxEXPAND | wxALL, 5 );

        m_panel16 = new wxPanel( aFrame, wxID_ANY, wxDefaultPosition, wxSize( -1,20 ), wxTAB_TRAVERSAL );
        m_panel16->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVECAPTION ) );
        m_panel16->SetMinSize( wxSize( -1,20 ) );
        m_panel16->SetMaxSize( wxSize( -1,20 ) );

        gbSizer2->Add( m_panel16, wxGBPosition( 1, 0 ), wxGBSpan( 1, 3 ), wxALL|wxEXPAND, 5 );

        gbSizer2->AddGrowableCol( 1 );
        gbSizer2->AddGrowableRow( 0 );

        aFrame->SetSizer( gbSizer2 );
        aFrame->Layout();
#endif
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);

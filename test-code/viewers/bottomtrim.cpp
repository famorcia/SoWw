/*
 * wxCoin3D Porting on wxWidgets of Coin3D (a.k.a Open Inventor) examples
 * Copyright (C) 2022  Fabrizio Morciano

 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#include <Inventor/Ww/SoWw.h>

#define protected public
#include <Inventor/Ww/viewers/SoWwFullViewer.h>
#undef protected

#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSeparator.h>

#include "wx/wx.h"
#include "common/SimpleFrame.h"

// Define a new application type
class MyApp : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE {
        if ( !wxApp::OnInit() )
            return false;

        wxWindow* window = SoWw::init("renderarea");

        SoWwFullViewer * renderarea =
                new SoWwFullViewer(window,
                                   "Renderarea demonstration",
                                   FALSE,
                                   SoWwFullViewer::BUILD_ALL,
                                   SoWwViewer::BROWSER,
                                   FALSE);

        SimpleFrame* asimpleframe = new SimpleFrame(0,
                                                    "bottomtrim",
                                                    wxDefaultPosition,
                                                    wxSize(300,60));
        renderarea->buildBottomTrim(asimpleframe);
        asimpleframe->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);



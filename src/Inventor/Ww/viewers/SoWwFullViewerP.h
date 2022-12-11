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
#ifndef SOWW_SOWWFULLVIEWERP_H
#define SOWW_SOWWFULLVIEWERP_H

#include "Inventor/Ww/viewers/SoGuiFullViewerP.h"

#include <string>

class SoWwFullViewerP : public SoGuiFullViewerP {
public:


    SoWwFullViewerP(SoWwFullViewer *pViewer);

    std::string popupmenutitle;
    wxWindow * viewerwidget, * canvas;
    wxWindow * interactbutton, * viewbutton;
    SbBool decorations;
    SbString menutitle;
    SbBool menuenabled;
    wxBoxSizer* mainlayout;
    wxBoxSizer * appbuttonlayout;
    wxWindow* appbuttonform;
    SbPList * appbuttonlist;
    SbPList * viewerbuttons;

    void setLeftWheelValue(const float value);
    static void setThumbWheelValue(wxWindow*, float value);
    void showDecorationWidgets(SbBool onOff);

    // Thumbwheels.
    void leftWheelPressed(void);
    void leftWheelChanged(float value);
    void leftWheelReleased(void);
    void rightWheelPressed(void);
    void rightWheelChanged(float value);
    void rightWheelReleased(void);
    void bottomWheelPressed(void);
    void bottomWheelChanged(float value);
    void bottomWheelReleased(void);

    // Button row.
    void interactbuttonClicked();
    void viewbuttonClicked();
    void homebuttonClicked();
    void sethomebuttonClicked();
    void viewallbuttonClicked();
    void seekbuttonClicked();

    // Menu items.
    void selectedViewing();
    void selectedDecoration();
    void selectedHeadlight();

    // Generic slots.
    void increaseInteractiveCount();
    void decreaseInteractiveCount();
};


#endif //SOWW_SOWWFULLVIEWERP_H

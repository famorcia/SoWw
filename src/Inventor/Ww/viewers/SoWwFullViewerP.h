//
// Created by fmorciano on 11/27/22.
//

#ifndef SOWW_SOWWFULLVIEWERP_H
#define SOWW_SOWWFULLVIEWERP_H

#include <string>

#include "Inventor/Ww/viewers/SoGuiFullViewerP.h"

class SoWwFullViewerP : public SoGuiFullViewerP {
public:
    std::string popupmenutitle;
    void setLeftWheelValue(const float value);
    static void setThumbWheelValue(wxFrame*, float value);

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

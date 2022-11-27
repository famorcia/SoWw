//
// Created by fmorciano on 11/27/22.
//

#ifndef SOWW_SOWWDEVICEP_H
#define SOWW_SOWWDEVICEP_H

#include "Inventor/Ww/devices/SoGuiDeviceP.h"

class SoWwDeviceP {
public:
    SbVec2s widgetsize;
    void
    addEventHandler(wxFrame* widget,
                    SoWwEventHandler * handler,
                    void * closure);
    void
    removeEventHandler(wxFrame* widget,
                                   SoWwEventHandler * handler,
                                   void * closure);
};


#endif //SOWW_SOWWDEVICEP_H

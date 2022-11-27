//
// Created by fmorciano on 11/27/22.
//

#ifndef SOWW_SOWWPLANEVIEWERP_H
#define SOWW_SOWWPLANEVIEWERP_H

#include <Inventor/SbBasic.h>
#include "Inventor/Ww/viewers/SoGuiPlaneViewerP.h"

class SoWwPlaneViewerP : public SoGuiPlaneViewerP {
public:
    SoWwPlaneViewerP(SoWwPlaneViewer*);
    void constructor(SbBool buildnow);
};


#endif //SOWW_SOWWPLANEVIEWERP_H

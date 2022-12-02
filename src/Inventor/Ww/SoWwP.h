//
// Created by fmorciano on 12/2/22.
//

#ifndef SOWW_SOWWP_H
#define SOWW_SOWWP_H

#ifndef SOWW_INTERNAL
#error this is a private header file
#endif /* !SOWW_INTERNAL */

#include <Inventor/Ww/SoGuiP.h>
#include <Inventor/SbBasic.h>


class SoWwP : public wxApp, public SoGuiP {

public:
    virtual bool OnInit() wxOVERRIDE {
        if ( !wxApp::OnInit() )
            return false;
        return true;
    }
};

#endif //SOWW_SOWWP_H

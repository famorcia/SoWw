//
// Created by fmorciano on 11/27/22.
//

#ifndef SOWW_WWNATIVEPOPUPMENU_H
#define SOWW_WWNATIVEPOPUPMENU_H

#include <Inventor/Ww/widgets/SoWwPopupMenu.h>

class WwNativePopupMenu : public SoWwPopupMenu {
public:
    virtual int newMenu(const char * name, int menuid = -1) ;
    virtual int getMenu(const char * name) ;
    virtual void setMenuTitle(int id, const char * title) ;
    virtual const char * getMenuTitle(int id) ;

    virtual int newMenuItem(const char * name, int itemid = -1) ;
    virtual int getMenuItem(const char * name) ;
    virtual void setMenuItemTitle(int itemid, const char * title) ;
    virtual const char * getMenuItemTitle(int itemid) ;
    virtual void setMenuItemEnabled(int itemid, SbBool enabled) ;
    virtual SbBool getMenuItemEnabled(int itemid) ;
    void setMenuItemMarked(int itemid, SbBool marked);
    virtual SbBool getMenuItemMarked(int itemid) ;

    virtual void addMenu(int menuid, int submenuid, int pos = -1) ;
    virtual void addMenuItem(int menuid, int itemid, int pos = -1) ;
    virtual void addSeparator(int menuid, int pos = -1) ;
    virtual void removeMenu(int menuid) ;
    virtual void removeMenuItem(int itemid) ;

    virtual void popUp(wxFrame* inside, int x, int y) ;

protected:
    virtual void _setMenuItemMarked(int itemid, SbBool marked) ;

};


#endif //SOWW_WWNATIVEPOPUPMENU_H

//
// Created by fmorciano on 11/27/22.
//

#include "WwNativePopupMenu.h"


int WwNativePopupMenu::newMenu(const char * name, int menuid) {

}
int WwNativePopupMenu::getMenu(const char * name){ }
void WwNativePopupMenu::setMenuTitle(int id, const char * title){ }
const char * WwNativePopupMenu::getMenuTitle(int id){ }

int WwNativePopupMenu::newMenuItem(const char * name, int itemid){ }
int WwNativePopupMenu::getMenuItem(const char * name){ }
void WwNativePopupMenu::setMenuItemTitle(int itemid, const char * title){ }
const char * WwNativePopupMenu::getMenuItemTitle(int itemid){ }
void WwNativePopupMenu::setMenuItemEnabled(int itemid, SbBool enabled){ }
SbBool WwNativePopupMenu::getMenuItemEnabled(int itemid){ }
void WwNativePopupMenu::setMenuItemMarked(int itemid, SbBool marked) {

}
SbBool WwNativePopupMenu::getMenuItemMarked(int itemid){ }

void WwNativePopupMenu::addMenu(int menuid, int submenuid, int pos){ }
void WwNativePopupMenu::addMenuItem(int menuid, int itemid, int pos){ }
void WwNativePopupMenu::addSeparator(int menuid, int pos){ }
void WwNativePopupMenu::removeMenu(int menuid){ }
void WwNativePopupMenu::removeMenuItem(int itemid){ }

void WwNativePopupMenu::popUp(wxFrame* inside, int x, int y){ }

void WwNativePopupMenu::_setMenuItemMarked(int itemid, SbBool marked){ }
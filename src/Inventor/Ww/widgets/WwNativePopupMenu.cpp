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

/*!
  \class WwNativePopupMenu Inventor/Ww/widgets/WwNativePopupMenu.h
  \brief The WwNativePopupMenu class implements a common interface for pop-up
  menu management for all the Coin GUI toolkit libraries.
*/

#include "WwNativePopupMenu.h"
#include "sowwdefs.h"

struct MenuRecord {
    int menuid;
    std::string name;
    std::string title;
    wxMenu* menu;
    wxWindow* parent;
    //QAction * action;
    //QPOPUPMENU_CLASS * menu;
    //QPOPUPMENU_CLASS * parent;
}; // struct MenuRecord

struct ItemRecord {
    int itemid;
    int flags;
    std::string name;
    std::string title;
    wxWindow* parent;
    //QAction * action;
    //QPOPUPMENU_CLASS * parent;
}; // struct ItemRecord

#define ITEM_MARKED       0x0001
#define ITEM_SEPARATOR    0x0002
#define ITEM_ENABLED      0x0004

/*!
  The constructor.
*/

WwNativePopupMenu::WwNativePopupMenu(void) {
    SOWW_STUB();
    this->menus = new SbPList;
    this->items = new SbPList;
} // WwNativePopupMenu()

/*!
  Destructor.
*/
WwNativePopupMenu::~WwNativePopupMenu() {
    SOWW_STUB();
    const int numMenus = this->menus->getLength();

    int i;
    for (i = 0; i < numMenus; i++) {
        MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
        // We used to delete only the root menu, and trust that Qt cleaned
        // up all submenus, but after doing some testing with huge menus,
        // we found that Qt does _not_ automatically delete submenus when
        // the parent is destructed. We therefore delete all menus
        // manually here. pederb, 2005-12-05
        // TODO: delete rec->menu;
        delete rec;
    }

    const int numItems = this->items->getLength();
    for (i = 0; i < numItems; i++) {
        ItemRecord * rec = (ItemRecord *) (*this->items)[i];
        delete rec;
    }

    delete this->menus;
    delete this->items;

} // ~WwNativePopupMenu()

// *************************************************************************

/*!
*/

int
WwNativePopupMenu::newMenu(const char * name,
                           int menuid) {
    SOWW_STUB();
    // FIXME: this function is the same in the other So-libraries --
    // move to common abstraction layer SoGuiPopupMenu of
    // possible. 20031012 mortene.

    int id = menuid;
    if (id == -1) {
        id = 1;
        while (this->getMenuRecord(id) != NULL) id++;
    } else {
        assert(this->getMenuRecord(id) == NULL &&
               "requested menuid already taken");
    }

    // id contains ok ID
    MenuRecord * rec = this->createMenuRecord(name);
    rec->menuid = id;
    this->menus->append((void *) rec);
    return id;
} // newMenu()

/*!
*/

int
WwNativePopupMenu::getMenu(const char * name) {
    SOWW_STUB();
    const int numMenus = this->menus->getLength();
    int i;
    for (i = 0; i < numMenus; i++) {
        MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
        if (rec->name == name)
            return rec->menuid;
    }
    return -1;
} // getMenu()

/*!
*/

void
WwNativePopupMenu::setMenuTitle(int menuid,
                                const char * title) {
    SOWW_STUB();
    MenuRecord * rec = this->getMenuRecord(menuid);
    assert(rec && "no such menu");
    rec->title = title;
    if (rec->parent) {
        //rec->parent->actionAt(QPoint(rec->menuid,0))->setText(rec->title);
        // TODO: getMenuRecord(rec->menuid)->action->setText(rec->title);
    }
} // setMenuTitle()

/*!
*/

const char *
WwNativePopupMenu::getMenuTitle(int menuid) {
    SOWW_STUB();
    MenuRecord * rec = this->getMenuRecord(menuid);
    assert(rec && "no such menu");
    return rec->title.c_str();
} // getMenuTitle()

// *************************************************************************

/*!
*/

int
WwNativePopupMenu::newMenuItem(const char * name,
                               int itemid) {
    SOWW_STUB();
    int id = itemid;
    if (id == -1) {
        id = 1;
        while (this->getItemRecord(id) != NULL) id++;
    } else {
        assert(this->getItemRecord(itemid) == NULL &&
               "requested itemid already taken");
    }
    ItemRecord * rec = createItemRecord(name);
    rec->itemid = id;
    this->items->append(rec);
    return id;
} // newMenuItem()

/*!
*/

int
WwNativePopupMenu::getMenuItem(const char * name) {
    SOWW_STUB();
    const int numItems = this->items->getLength();
    int i;
    for (i = 0; i < numItems; i++) {
        ItemRecord * rec = (ItemRecord *) (*this->items)[i];
        if (rec->name == name)
            return rec->itemid;
    }
    return -1;
} // getMenuItem()

/*!
*/

void
WwNativePopupMenu::setMenuItemTitle(int itemid,
                                    const char * title) {
    SOWW_STUB();
    ItemRecord * rec = this->getItemRecord(itemid);
    assert(rec && "no such menu");
    rec->title = title;
    if (rec->parent) {
        // TODO: rec->parent->actionAt(QPoint(rec->itemid,0))->setText(rec->title);
    }
} // setMenuItemTitle()

/*!
*/

const char *
WwNativePopupMenu::getMenuItemTitle(int itemid) {
    SOWW_STUB();
    ItemRecord * rec = this->getItemRecord(itemid);
    assert(rec && "no such menu");
    return rec->title.c_str();
} // getMenuItemTitle()

/*!
*/

void
WwNativePopupMenu::setMenuItemEnabled(int itemid,
                                      SbBool enabled) {
    SOWW_STUB();
    ItemRecord * rec = this->getItemRecord(itemid);
    if (rec) {
        // TODO: rec->parent->actionAt(QPoint(rec->itemid,0))->setEnabled(enabled ? true : false);
        return;
    }
    MenuRecord * mrec = this->getMenuRecord(itemid);
    assert(mrec && "no such menu");
    assert(mrec->parent && "a menuitem must have a parent to be enabled/disabled");

    // TODO: mrec->parent->actionAt(QPoint(mrec->menuid,0))->setEnabled(enabled ? true : false);
} // setMenuItemEnabled()

/*!
*/

SbBool
WwNativePopupMenu::getMenuItemEnabled(int itemid) {
    SOWW_STUB();
    ItemRecord * rec = this->getItemRecord(itemid);

    if (rec) {
        // TODO return rec->parent->actionAt(QPoint(rec->itemid,0))->isEnabled();
        return (FALSE);
    }

    MenuRecord * mrec = this->getMenuRecord(itemid);
    assert(mrec && "no such menu");
    assert(mrec->parent && "a menuitem must have a parent to be enabled/disabled");

    // TODO return mrec->parent->actionAt(QPoint(mrec->menuid,0))->isEnabled();
    return (FALSE);

} // getMenuItemEnabled()

/*!
*/

void
WwNativePopupMenu::_setMenuItemMarked(int itemid, SbBool marked) {
    SOWW_STUB();
    ItemRecord * rec = this->getItemRecord(itemid);
    if (rec == NULL)
        return;
    if (marked)
        rec->flags |= ITEM_MARKED;
    else
        rec->flags &= ~ITEM_MARKED;

    if (rec->parent != NULL) {
#if TODO
        QAction * action = rec->action;
        if (action) {
            //FIXME: Could this be done on creation? - BFG 20090910
            action->setCheckable(true);
            action->setChecked(marked ? true : false);
        }
#endif
    }
}

/*!
*/

SbBool
WwNativePopupMenu::getMenuItemMarked( int itemid) {
    SOWW_STUB();
    ItemRecord * rec = this->getItemRecord(itemid);
    assert(rec && "no such menu");
    if (rec->parent == NULL)
        return (rec->flags & ITEM_MARKED) ? TRUE : FALSE;
#if TODO
    QAction * action = rec->action;
    if (!action) return false;
    return action->isChecked();
#endif
    return (FALSE);
} // getMenuItemMarked()

// *************************************************************************

/*!
*/

void
WwNativePopupMenu::addMenu(int menuid,
                           int submenuid,
                           int pos) {
    SOWW_STUB();
    MenuRecord * super = this->getMenuRecord(menuid);
    MenuRecord * sub = this->getMenuRecord(submenuid);
    assert(super && sub && "no such menu");

    // disconnect the submenu from the activated() signal. In Qt 4.0
    // only the parent menu should be connected, since both the parent
    // menu and the submenu will generate a call to
    // itemActivation(). Multiple calls to itemActivation() causes a
    // segfault when selecting Quadbuffer stereo, at least when it's not
    // supported. (20050726 frodo)
#if TODO
    // this was changed/fixed again in Qt 4.4.0, so now we shouldn't
    // disconnect menu items if this version is detected...
    // (20070530 pederb)
    QObject::disconnect(sub->menu, SIGNAL(activated(int)),
                        this, SLOT(itemActivation(int)));

    QAction * action;
    if (pos == -1) {
        action = super->menu->addMenu(sub->menu);
    }
    else {
        QAction * before = super->menu->actionAt(QPoint(pos,0));
        action = super->menu->insertMenu(before,sub->menu);
    }
    action->setText(QString(sub->title));
    sub->parent = super->menu;
#endif
} // addMenu()

/*!
*/

void
WwNativePopupMenu::addMenuItem(int menuid,
                               int itemid,
                               int pos) {
    SOWW_STUB();
    MenuRecord * menu = this->getMenuRecord(menuid);
    assert(menu && "invalid parent menu id");
    ItemRecord * item = this->getItemRecord(itemid);
    assert(item && "invalid child menu id");

#if TODO
    item->action = new QAction(menu->menu);
    item->action->setText(item->title);
    if (pos == -1) {
        menu->menu->addAction(item->action);
    }
    else {
        menu->menu->insertAction(menu->menu->actionAt(QPoint(pos,0)),item->action);
    }
    item->parent = menu->menu;

    // FIXME: is this really safe? (20050726 frodo)
    QAction * action = (QAction *) item->parent->actionAt(QPoint(itemid,0));
    if (action) action->setCheckable(true);

    if (item->flags & ITEM_MARKED) {
        if (action) action->setChecked(true);
    }
#endif
} // addMenuItem()

void
WwNativePopupMenu::addSeparator(int menuid,
                                int pos) {
    SOWW_STUB();
    MenuRecord * menu = this->getMenuRecord(menuid);
    assert(menu && "no such menu");

    ItemRecord * rec = createItemRecord("separator");
    // TODO: menu->menu->insertSeparator(menu->menu->actionAt(QPoint(pos,0)));
    rec->flags |= ITEM_SEPARATOR;
    this->items->append(rec);
} // addSeparator()

/*!
  This method removes the submenu with the given \a menuid.

  A removed menu can be attached again later - its menu id will still be
  allocated.
*/

void
WwNativePopupMenu::removeMenu(int menuid) {
    SOWW_STUB();
    MenuRecord * rec = this->getMenuRecord(menuid);
    assert(rec && "no such menu");

    if (rec->menuid == 0) {
#if SOWW_DEBUG
        SoDebugError::postInfo("WwNativePopupMenu::RemoveMenu", "can't remove root");
#endif // SOWW_DEBUG
        return;
    }
    if (rec->parent == NULL) {
#if SOWW_DEBUG
        SoDebugError::postInfo("WwNativePopupMenu::RemoveMenu", "menu not attached");
#endif // SOWW_DEBUG
        return;
    }
    // TODO: rec->parent->removeAction(rec->parent->actionAt(QPoint(rec->menuid,0)));
    rec->parent = NULL;
} // removeMenu()

/*!
  This method removes the menu item with the given \a itemid.

  A removed menu item can be attached again later - its item id will still
  be allocated.
*/

void
WwNativePopupMenu::removeMenuItem(int itemid) {
    SOWW_STUB();
    ItemRecord * rec = this->getItemRecord(itemid);
    assert(rec && "no such item");

    if (rec->parent == NULL) {
#if SOWW_DEBUG
        SoDebugError::postInfo("WwNativePopupMenu::RemoveMenu", "item not attached");
#endif // SOWW_DEBUG
        return;
    }
    // TODO: rec->parent->removeAction(rec->parent->actionAt(QPoint(rec->itemid,0)));
    rec->parent = NULL;
} // removeMenuItem()

// *************************************************************************

// Doc in superclass.
void
WwNativePopupMenu::popUp(wxWindow * inside, int x, int y) {
    SOWW_STUB();
    MenuRecord * rec = this->getMenuRecord(0);

    // Use exec() and not popup(). popup() doesn't seem to work properly
    // with a GL widget (the GL-redraw seems to overwrite the popup
    // widget or something). pederb, 2003-10-01
    //
    // An additional piece of information: the popup-menu misbehavior is
    // that at first attempt just a "shadow" comes up, and then on the
    // second RMB-press the menu actually comes up. This problem has
    // only been seen on a few Windows 2000 platforms, though, so in
    // case we ever need to revert to using popup() again, make sure the
    // code is properly tested over a wide range of platforms.
    // mortene, 2003-10-01
    //
    // Ignore return value. We use callbacks.
    // TODO: (void) rec->menu->exec(inside->mapToGlobal(QPoint(x, y)));
}

// *************************************************************************

/*!
*/

MenuRecord *
WwNativePopupMenu::getMenuRecord(int menuid) {
    SOWW_STUB();
    const int numMenus = this->menus->getLength();
    int i;
    for (i = 0; i < numMenus; i++)
        if (((MenuRecord *) (*this->menus)[i])->menuid == menuid)
            return (MenuRecord *) (*this->menus)[i];
    return (MenuRecord *) NULL;
} // getMenuRecord()

/*!
*/
ItemRecord *
WwNativePopupMenu::getItemRecord(int itemid) {
    SOWW_STUB();
    const int numItems = this->items->getLength();
    for (int i = 0; i < numItems; i++) {
        const int recid = ((ItemRecord *) (*this->items)[i])->itemid;
        if (recid == itemid) { return (ItemRecord *) (*this->items)[i]; }
    }

    return (ItemRecord *) NULL;
}

// *************************************************************************
#if 0
/*!
*/
ItemRecord *
WwNativePopupMenu::getItemRecordFromAction(QAction * action) {
    const int numItems = this->items->getLength();
    for (int i = 0; i < numItems; i++) {
        const ItemRecord * rec = static_cast<ItemRecord *>((*this->items)[i]);
        // TODO: if (rec->action == action) { return (ItemRecord *) (*this->items)[i]; }
    }

    return (ItemRecord *) NULL;
}
#endif

// *************************************************************************

/*!
*/

MenuRecord *
WwNativePopupMenu::createMenuRecord(const char * name) {
    SOWW_STUB();
    MenuRecord * rec = new MenuRecord;
    rec->menuid = -1;
    rec->name =  name;
    rec->title = name;

    rec->menu = new  wxMenu(name);
    // TODO: QObject::connect(rec->menu, SIGNAL(triggered(QAction *)),                     this, SLOT(itemActivation(QAction *)));

    rec->parent = NULL;
    // TODO: rec->action = NULL;
    return rec;
} // create()

/*!
*/

ItemRecord *
WwNativePopupMenu::createItemRecord(const char * name) {
    SOWW_STUB();
    ItemRecord * rec = new ItemRecord;
    rec->itemid = -1;
    rec->flags = 0;
    rec->name = name;
    rec->title =  name;
    rec->parent = NULL;
    // TODO: rec->action = NULL;
    return rec;
} // create()

// *************************************************************************
#if 0
void
WwNativePopupMenu::itemActivation(QAction * action)
{
    ItemRecord * rec = getItemRecordFromAction(action);
    assert(rec);
    inherited::invokeMenuSelection(rec->itemid);
} // menuSelection()

void
WwNativePopupMenu::itemActivation(int itemid
)
{
    inherited::invokeMenuSelection(itemid);
}
#endif
// *************************************************************************

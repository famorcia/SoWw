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

#include "WwNativePopupMenu.h"
#include "sowwdefs.h"


int 
WwNativePopupMenu::newMenu(const char * name, int menuid) {
    SOWW_STUB();
    return (0);
}

int 
WwNativePopupMenu::getMenu(const char * name) { 
    SOWW_STUB();
    return (0);
}

void 
WwNativePopupMenu::setMenuTitle(int id, const char * title) {
    SOWW_STUB();
}

const char * 
WwNativePopupMenu::getMenuTitle(int id) { 
    SOWW_STUB();
    return ("");
}

int 
WwNativePopupMenu::newMenuItem(const char * name, int itemid) { 
    SOWW_STUB();
    return (0);
}

int 
WwNativePopupMenu::getMenuItem(const char * name) { 
    SOWW_STUB();
    return (0);
}

void 
WwNativePopupMenu::setMenuItemTitle(int itemid, const char * title) { 
    SOWW_STUB();
}

const char * 
WwNativePopupMenu::getMenuItemTitle(int itemid) {
    SOWW_STUB();
    return ("");
}

void 
WwNativePopupMenu::setMenuItemEnabled(int itemid, SbBool enabled) {
    SOWW_STUB();
}

SbBool 
WwNativePopupMenu::getMenuItemEnabled(int itemid) {
    SOWW_STUB();
    return (FALSE);
}

void 
WwNativePopupMenu::setMenuItemMarked(int itemid, SbBool marked) {
    SOWW_STUB();
}

SbBool 
WwNativePopupMenu::getMenuItemMarked(int itemid) {
    SOWW_STUB();
    return (FALSE);
}

void 
WwNativePopupMenu::addMenu(int menuid, int submenuid, int pos) {
    SOWW_STUB();
}

void 
WwNativePopupMenu::addMenuItem(int menuid, int itemid, int pos) {
    SOWW_STUB();
}

void 
WwNativePopupMenu::addSeparator(int menuid, int pos) {
    SOWW_STUB();
}

void 
WwNativePopupMenu::removeMenu(int menuid) {
    SOWW_STUB();
}

void WwNativePopupMenu::removeMenuItem(int itemid) {
    SOWW_STUB();
}

void 
WwNativePopupMenu::popUp(wxWindow* inside, int x, int y) {
    SOWW_STUB();
}

void 
WwNativePopupMenu::_setMenuItemMarked(int itemid, SbBool marked) {
    SOWW_STUB();
}
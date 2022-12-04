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
#include <Inventor/Ww/viewers/SoWwFullViewer.h>
#include "Inventor/Ww/viewers/SoWwFullViewerP.h"

SOWW_OBJECT_ABSTRACT_SOURCE(SoWwFullViewer);



SoWwFullViewer::SoWwFullViewer(wxWindow* parent,
                               const char * name,
                               SbBool embed,
                               BuildFlag flag,
                               Type type,
                               SbBool build): SoWwViewer(parent, name, embed, type, build){

}

wxWindow* SoWwFullViewer::buildWidget(wxWindow* parent) {

}


void SoWwFullViewer::setDecoration(const SbBool on){ }
SbBool SoWwFullViewer::isDecoration(void) const{ }

void SoWwFullViewer::setPopupMenuEnabled(const SbBool on){ }
SbBool SoWwFullViewer::isPopupMenuEnabled(void) const{ }

wxWindow* SoWwFullViewer::getAppPushButtonParent(void) const{ }
void SoWwFullViewer::addAppPushButton(wxWindow* newButton){ }
void SoWwFullViewer::insertAppPushButton(wxWindow* newButton, int index){ }
void SoWwFullViewer::removeAppPushButton(wxWindow* oldButton){ }
int SoWwFullViewer::findAppPushButton(wxWindow* oldButton) const{ }
int SoWwFullViewer::lengthAppPushButton(void) const{ }

wxWindow* SoWwFullViewer::getRenderAreaWidget(void) const{ }

void SoWwFullViewer::setViewing(SbBool on){ }

/*
SoWwFullViewer::SoWwFullViewer(wxWindow* parent,
                               const char * name,
                               SbBool embed,
                               BuildFlag flag,
                               Type type,
                               SbBool build){

}*/

SoWwFullViewer::~SoWwFullViewer(){ }

void SoWwFullViewer::buildDecoration(wxWindow* parent){ }
wxWindow* SoWwFullViewer::buildLeftTrim(wxWindow* parent){ }
wxWindow* SoWwFullViewer::buildBottomTrim(wxWindow* parent){ }
wxWindow* SoWwFullViewer::buildRightTrim(wxWindow* parent){ }
wxWindow* SoWwFullViewer::buildAppButtons(wxWindow* parent){ }
wxWindow* SoWwFullViewer::buildViewerButtons(wxWindow* parent){ }
void SoWwFullViewer::createViewerButtons(wxWindow* parent, SbPList * buttonlist){ }

void SoWwFullViewer::buildPopupMenu(void){ }
void SoWwFullViewer::openPopupMenu(const SbVec2s position){ }

void SoWwFullViewer::setLeftWheelString(const char * const name){ }
void SoWwFullViewer::setBottomWheelString(const char * const name){ }

void SoWwFullViewer::setRightWheelString(const char * const name){ }
const char * SoWwFullViewer::getRightWheelString() const{ }

void SoWwFullViewer::sizeChanged(const SbVec2s & size) {

}

void  SoWwFullViewerP::setThumbWheelValue(wxWindow*, float value) {

}

void  SoWwFullViewerP::seekbuttonClicked()  {

}
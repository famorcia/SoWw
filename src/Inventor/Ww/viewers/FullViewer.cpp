//
// Created by fmorciano on 11/27/22.
//

#include <Inventor/Ww/viewers/SoWwFullViewer.h>

SOWW_OBJECT_ABSTRACT_SOURCE(SoWwFullViewer);


SoWwFullViewer::SoWwFullViewer(wxFrame* parent,
                               const char * name,
                               SbBool embed,
                               BuildFlag flag,
                               Type type,
                               SbBool build): SoWwViewer(parent, name, embed, type, build){

}

wxFrame* SoWwFullViewer::buildWidget(wxFrame* parent) {

}


void SoWwFullViewer::setDecoration(const SbBool on){ }
SbBool SoWwFullViewer::isDecoration(void) const{ }

void SoWwFullViewer::setPopupMenuEnabled(const SbBool on){ }
SbBool SoWwFullViewer::isPopupMenuEnabled(void) const{ }

wxFrame* SoWwFullViewer::getAppPushButtonParent(void) const{ }
void SoWwFullViewer::addAppPushButton(wxFrame* newButton){ }
void SoWwFullViewer::insertAppPushButton(wxFrame* newButton, int index){ }
void SoWwFullViewer::removeAppPushButton(wxFrame* oldButton){ }
int SoWwFullViewer::findAppPushButton(wxFrame* oldButton) const{ }
int SoWwFullViewer::lengthAppPushButton(void) const{ }

wxFrame* SoWwFullViewer::getRenderAreaWidget(void) const{ }

void SoWwFullViewer::setViewing(SbBool on){ }

/*
SoWwFullViewer::SoWwFullViewer(wxFrame* parent,
                               const char * name,
                               SbBool embed,
                               BuildFlag flag,
                               Type type,
                               SbBool build){

}*/

SoWwFullViewer::~SoWwFullViewer(){ }

void SoWwFullViewer::buildDecoration(wxFrame* parent){ }
wxFrame* SoWwFullViewer::buildLeftTrim(wxFrame* parent){ }
wxFrame* SoWwFullViewer::buildBottomTrim(wxFrame* parent){ }
wxFrame* SoWwFullViewer::buildRightTrim(wxFrame* parent){ }
wxFrame* SoWwFullViewer::buildAppButtons(wxFrame* parent){ }
wxFrame* SoWwFullViewer::buildViewerButtons(wxFrame* parent){ }
void SoWwFullViewer::createViewerButtons(wxFrame* parent, SbPList * buttonlist){ }

void SoWwFullViewer::buildPopupMenu(void){ }
void SoWwFullViewer::openPopupMenu(const SbVec2s position){ }

void SoWwFullViewer::setLeftWheelString(const char * const name){ }
void SoWwFullViewer::setBottomWheelString(const char * const name){ }

void SoWwFullViewer::setRightWheelString(const char * const name){ }
const char * SoWwFullViewer::getRightWheelString() const{ }

void SoWwFullViewer::sizeChanged(const SbVec2s & size) {

}

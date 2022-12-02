//
// Created by fmorciano on 11/27/22.
//

#include "Inventor/Ww/SoWwComponent.h"

SOWW_OBJECT_ABSTRACT_SOURCE(SoWwComponent);

SoWwComponent::~SoWwComponent(){

}

SoWwComponent::SoWwComponent(wxFrame* const parent,
                             const char * const name,
                             const SbBool embed) {

}

void  SoWwComponent::initClasses(void) {

}

void SoWwComponent::afterRealizeHook(void) {

}

void SoWwComponent::setClassName(const char * const name) {

}

void SoWwComponent::setBaseWidget(wxFrame* widget) {

}


void SoWwComponent::show(void) {

}

void SoWwComponent::hide(void) {

}

void SoWwComponent::setComponentCursor(const SoWwCursor & cursor) {

}

void SoWwComponent::setWidgetCursor(wxFrame* w, const SoWwCursor & cursor) {

}

SbBool SoWwComponent::isFullScreen(void) const {

}

SbBool SoWwComponent::setFullScreen(const SbBool onoff) {

}

SbBool SoWwComponent::isVisible(void) {

}

SbBool SoWwComponent::isTopLevelShell(void) const {

}

wxFrame* SoWwComponent::getWidget(void) const {

}

wxFrame* SoWwComponent::getBaseWidget(void) const {

}


wxFrame* SoWwComponent::getParentWidget(void) const {

}

void SoWwComponent::setSize(const SbVec2s size) {

}

SbVec2s SoWwComponent::getSize(void) const {

}

void SoWwComponent::setTitle(const char * const title) {

}
const char * SoWwComponent::getTitle(void) const {

}

void SoWwComponent::setIconTitle(const char * const title) {

}

const char * SoWwComponent::getIconTitle(void) const {

}

const char * SoWwComponent::getWidgetName(void) const{

}

const char * SoWwComponent::getClassName(void) const {

}

void SoWwComponent::setWindowCloseCallback(SoWwComponentCB * const func,
                                           void * const user ) {

}


void SoWwComponent::sizeChanged(const SbVec2s & size) {

}

void SoWwComponent::addVisibilityChangeCallback(SoWwComponentVisibilityCB * const func,
                                                void * const user) {

}

void SoWwComponent::removeVisibilityChangeCallback(SoWwComponentVisibilityCB * const func,
                                                   void * const user){

}
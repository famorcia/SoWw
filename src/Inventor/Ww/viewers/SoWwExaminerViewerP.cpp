//
// Created by fmorciano on 12/9/22.
//

#include "SoWwExaminerViewerP.h"

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

SoWwExaminerViewerP::SoWwExaminerViewerP(SoWwExaminerViewer *publ)
: SoGuiExaminerViewerP(publ) {

}

void SoWwExaminerViewerP::constructor(const SbBool build) {
    this->genericConstructor();
#if 0
    PUBLIC(this)->setClassName(PUBLIC(this)->getWidgetName());
    this->camerabutton = (Widget) NULL;

    if (build) {
        Widget viewer = PUBLIC(this)->buildWidget(PUBLIC(this)->getParentWidget());
        PUBLIC(this)->setBaseWidget(viewer);
        PUBLIC(this)->fitSize(SbVec2s(500, 300));

        char * dollyString = NULL;
        SoXtResource rsc(PUBLIC(this)->getRightWheelLabelWidget());
        if (rsc.getResource("dollyString", XmRString, dollyString) &&
            dollyString != NULL)
            PUBLIC(this)->setRightWheelString(dollyString);
    }
#endif
}

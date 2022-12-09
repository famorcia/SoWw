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
#if 0 // TODO
    this->cameratogglebutton = NULL;

    this->orthopixmap = new QPixmap((const char **) ortho_xpm);
    this->perspectivepixmap = new QPixmap((const char **) perspective_xpm);
    assert(this->orthopixmap->size() == this->perspectivepixmap->size());

    PUBLIC(this)->setClassName("SoQtExaminerViewer");

    PUBLIC(this)->setPopupMenuString("Examiner Viewer");
    PUBLIC(this)->setLeftWheelString("RotX");
    PUBLIC(this)->setBottomWheelString("RotY");

    if (build) {
        QWidget *widget = PUBLIC(this)->buildWidget(PUBLIC(this)->getParentWidget());
        PUBLIC(this)->setBaseWidget(widget);
    }
#endif
}
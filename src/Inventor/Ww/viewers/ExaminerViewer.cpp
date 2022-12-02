//
// Created by fmorciano on 11/27/22.
//

#include "Inventor/Ww/viewers/SoWwExaminerViewer.h"
#include "Inventor/Ww/viewers/SoWwViewer.h"
#include "Inventor/Ww/viewers/SoWwFullViewer.h"

SOWW_OBJECT_SOURCE(SoWwExaminerViewer);

/**
 * wxFrame* parent,
                    const char * name,
                    SbBool embed,
                    BuildFlag flag,
                    Type type,
                    SbBool build
 * @param parent
 */
SoWwExaminerViewer::SoWwExaminerViewer(wxFrame* parent,
                                       char const* name,
                                       int ,
                                       SoWwFullViewer::BuildFlag,
                                       SoWwViewer::Type)
                                       : SoWwFullViewer(parent,
                                                        name,
                                                        false,
                                                        BUILD_NONE,
                                                        BROWSER,
                                                        true) {

}

void SoWwExaminerViewer::setCamera(SoCamera * camera) {

}

void SoWwExaminerViewer::createViewerButtons(wxFrame* parent, SbPList * buttonlist) {

}


SoWwExaminerViewer::~SoWwExaminerViewer() {

}
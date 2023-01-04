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

#include "Inventor/Ww/viewers/SoWwExaminerViewer.h"
#include "Inventor/Ww/viewers/SoWwExaminerViewerP.h"
#include "Inventor/Ww/viewers/SoWwViewer.h"
#include "Inventor/Ww/viewers/SoWwFullViewer.h"
#include "Inventor/nodes/SoOrthographicCamera.h"
#include "sowwdefs.h"
#include "ButtonIndexValues.h"

#ifdef HAVE_CONFIG_H
#include <config.h> // for HAVE_LIBXPM
#include <Inventor/nodes/SoPerspectiveCamera.h>

#endif // HAVE_CONFIG_H

#if HAVE_LIBXPM
#include <Inventor/Ww/common/pixmaps/ortho.xpm>
#include <Inventor/We/common/pixmaps/perspective.xpm>
#endif // HAVE_LIBXPM

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

SOWW_OBJECT_SOURCE(SoWwExaminerViewer);

SoWwExaminerViewer::SoWwExaminerViewer(wxWindow* parent,
                                       char const* name,
                                       int embed,
                                       SoWwFullViewer::BuildFlag flag,
                                       SoWwViewer::Type type)
        : inherited(parent, name, embed, flag, type, FALSE)
{
    PRIVATE(this) = new SoWwExaminerViewerP(this);
    PRIVATE(this)->constructor(TRUE);
}


void SoWwExaminerViewer::setCamera(SoCamera * newCamera) {
    // This method overridden from parent class to toggle the camera
    // type selection button pixmap and string of the zoom/dolly
    // thumbwheel.

    if (newCamera) {
        SoType camtype = newCamera->getTypeId();
        SbBool orthogonal =
                camtype.isDerivedFrom(SoOrthographicCamera::getClassTypeId());

        const char * oldLabel = this->getRightWheelString();
        if (oldLabel) {
            if (orthogonal) {
                if (strcmp("Dolly",oldLabel) == 0)
                    this->setRightWheelString("Zoom");
            }
            else if (strcmp("Zoom",oldLabel) == 0)
                this->setRightWheelString("Dolly");
        }
        if (PRIVATE(this)->cameratogglebutton) {
            PRIVATE(this)->cameratogglebutton->SetBitmap(
                    orthogonal ?
                    * (PRIVATE(this)->orthopixmap) :
                    * (PRIVATE(this)->perspectivepixmap));
        }
    }

    inherited::setCamera(newCamera);
}

void SoWwExaminerViewer::createViewerButtons(wxWindow* parent, SbPList * buttonlist) {

    inherited::createViewerButtons(parent, buttonlist);
    PRIVATE(this)->cameratogglebutton = new wxButton(parent, CAMERA_BUTTON, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

    assert(PRIVATE(this)->perspectivepixmap);
    assert(PRIVATE(this)->orthopixmap);

    wxImage * p = NULL;
    SoType t = this->getCameraType();
    if (t.isDerivedFrom(SoOrthographicCamera::getClassTypeId()))
        p = PRIVATE(this)->orthopixmap;
    else if (t.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()))
        p = PRIVATE(this)->perspectivepixmap;
    else assert(0 && "unsupported cameratype");

    PRIVATE(this)->cameratogglebutton->SetBitmap(*p);

    buttonlist->append(PRIVATE(this)->cameratogglebutton);
}


SoWwExaminerViewer::~SoWwExaminerViewer() {

}
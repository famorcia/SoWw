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
#ifndef SOWW_SOWWTHUMBWHEEL_H
#define SOWW_SOWWTHUMBWHEEL_H

#include <wx/frame.h>
#include "Inventor/Ww/SoWwBasic.h"
#include "Inventor/Ww/widgets/SoAnyThumbWheel.h"

class SOWW_DLL_API SoWwThumbWheel : public wxFrame
{

public:
    enum Orientation { Horizontal, Vertical };

    SoWwThumbWheel(wxWindow * parent = 0, const char * name = 0);
    SoWwThumbWheel(Orientation, wxWindow * parent = 0, const char * name = 0);
    ~SoWwThumbWheel(void);

    void setOrientation(Orientation);
    Orientation orientation(void) const;

    void setValue(float value);
    float value(void) const;

    void setEnabled(bool enable);
    bool isEnabled(void) const;

    enum boundaryHandling {
        CLAMP,
        MODULATE,
        ACCUMULATE
    };
    void setRangeBoundaryHandling(boundaryHandling handling);
    boundaryHandling getRangeBoundaryHandling(void) const;

    wxSize sizeHint(void) const;

    void paintEvent(wxPaintEvent& );
    void mousePressEvent(wxMouseEvent& );
    void mouseReleaseEvent(wxMouseEvent& );
    void mouseMoveEvent(wxMouseEvent& );

private:
    void constructor(Orientation);

    SoWwThumbWheel(const SoWwThumbWheel & wheel);
    SoWwThumbWheel & operator = (const SoWwThumbWheel & wheel);

    enum State { Idle, Dragging, Disabled } state;

    Orientation orient;
    float wheelValue, tempWheelValue;
    int mouseDownPos, mouseLastPos;

    void initWheel(int diameter, int width);

    SoAnyThumbWheel * wheel;
    wxBitmap**   pixmaps;
    int numPixmaps;
    int currentPixmap;

wxDECLARE_EVENT_TABLE();

}; // class SoWwThumbWheel

// *************************************************************************


#endif //SOWW_SOWWTHUMBWHEEL_H

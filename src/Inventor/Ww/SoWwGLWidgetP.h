//
// Created by fmorciano on 11/27/22.
//

#ifndef SOWW_SOWWGLWIDGETP_H
#define SOWW_SOWWGLWIDGETP_H

#include "Inventor/Ww/SoGuiGLWidgetP.h"
#include <wx/glcanvas.h>
#include <wx/wx.h>
#include <wx/timer.h>

class SoWwGLWidgetP : public wxGLCanvas
{
public:
    static void eventHandler(wxFrame*, void*, wxEvent*, bool*);

    SoWwGLWidgetP(wxWindow *parent,
                 wxGLAttributes&,
                 wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxString& name = "SoWwGLWidgetP"
    );

    virtual ~SoWwGLWidgetP();

protected:
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnTimer(wxTimerEvent& event);

private:
    enum {
        TIMER_ID = 3000
    };
    void InitGL();

    bool isGLInitialized;
    wxTimer timer;
    wxGLContext* glRealContext;
    int W;
    int H;
wxDECLARE_NO_COPY_CLASS(SoWwGLWidgetP);
wxDECLARE_EVENT_TABLE();
};


#endif //SOWW_SOWWGLWIDGETP_H

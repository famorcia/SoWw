//
// Created by fmorciano on 11/27/22.
//

#ifndef SOWW_SOWWGLAREA_H
#define SOWW_SOWWGLAREA_H

#include <wx/glcanvas.h>
#include <wx/wx.h>
#include <wx/timer.h>

class SoWwGLArea : public wxGLCanvas
{
public:
    SoWwGLArea(wxWindow *parent,
               wxGLAttributes&,
               wxWindowID id = wxID_ANY,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxString& name = "SoWwGLArea"
    );

    virtual ~SoWwGLArea();

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
wxDECLARE_NO_COPY_CLASS(SoWwGLArea);
wxDECLARE_EVENT_TABLE();
};


#endif //SOWW_SOWWGLAREA_H

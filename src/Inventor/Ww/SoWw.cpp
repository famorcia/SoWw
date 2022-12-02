//
// Created by fmorciano on 11/27/22.
//

#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/SoInteraction.h>
#include "Inventor/Ww/SoWw.h"
#include "Inventor/Ww/SoWwP.h"

#include "wx/wx.h"

//wxIMPLEMENT_APP(SoWwP);

#include <wx/frame.h>
#include <wx/defs.h>

class MyFrame : public wxFrame
{
public:
    MyFrame(wxFrame *frame,
            const wxString& title,
            const wxPoint& pos,
            const wxSize& size,
            long style = wxDEFAULT_FRAME_STYLE);

wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(wxFrame *frame,
                 const wxString& title,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style)
        : wxFrame(frame, wxID_ANY, title, pos, size, style)
{
}

wxFrame* SoWw::init(int & argc,
                    char ** argv,
                    const char * appname,
                    const char * classname) {

    // Must do this here so SoDebugError is initialized before it could
    // be attempted used.
    if (!SoDB::isInitialized()) { SoDB::init(); }
    // these aren't necessary - they are initialized in the SoWw::init method above
    // which is invoked below
    SoNodeKit::init();
    SoInteraction::init();

    /*
    if (SOWW_DEBUG && (SoWwP::appobject || SoWwP::mainwidget)) {
        SoDebugError::postWarning("SoWw::init",
                                  "This method should be called only once.");
        return SoWwP::mainwidget;
    }
     */

    // "qApp" is a global variable from the Qt library, pointing to the
    // single QApplication instance in a Qt-application.
    if (wxApp::GetInstance() == NULL) {
        // Set up the QApplication instance which we have derived into a
        // subclass to catch spaceball events.
        // wxApp::SetInstance( new SoWwP );
        // wxAppInitializer wxTheAppInitializer((wxAppInitializerFunction) wxCreateApp);
        wxApp::SetInstance( new SoWwP() );
        wxEntryStart( argc, argv );
        wxTheApp->CallOnInit();
    }
    else {
        // The user already set one up for us.
        //
        // FIXME: should somehow warn about the fact that spaceball events
        // will not be caught, if a spaceball is attempted used. 20020619 mortene.

    }

    wxFrame* aFrame = new MyFrame(0,
                                  appname,
                                  wxDefaultPosition,
                                  wxDefaultSize);
    return aFrame;
}

void SoWw::init(wxFrame* toplevelwidget) {

}

void SoWw::mainLoop(void) {
    wxTheApp->OnRun();
    wxTheApp->OnExit();
    wxEntryCleanup();
}

void  SoWw::show(wxFrame* const widget) {

}

void    SoWw::createSimpleErrorDialog(wxFrame* widget,
                                      const char * title,
                                      const char * string1,
                                      const char * string2 ) {

}

wxFrame*   SoWw::getShellWidget(const wxFrame* w) {

}

#if 0

static void mainLoop(void);
    static void exitMainLoop(void);
    static void done(void);

    static wxFrame* getTopLevelWidget(void);
    static wxFrame* getShellWidget(const wxFrame* w);

    static void show(wxFrame* const widget);
    static void hide(wxFrame* const widget);

    static void setWidgetSize(wxFrame* const widget, const SbVec2s size);
    static SbVec2s getWidgetSize(const wxFrame* widget);


    static void getVersionInfo(int * major = NULL,
                               int * minor = NULL,
                               int * micro = NULL);
    static const char * getVersionString(void);
    static const char * getVersionToolkitString(void);

    enum FatalErrors {
        UNSPECIFIED_ERROR = 0,
        NO_OPENGL_CANVAS,
        INTERNAL_ASSERT
    };
    typedef void FatalErrorCB(const SbString errmsg, SoWw::FatalErrors errcode,
                              void * userdata);
    static FatalErrorCB * setFatalErrorHandler(SoWw::FatalErrorCB * cb,
                                               void * userdata);

    static SbBool isDebugLibrary(void);
    static SbBool isCompatible(unsigned int major, unsigned int minor);

    enum ABIType { DLL, LIB, UNKNOWN };
    static ABIType getABIType(void);

    static void lockGL(void);
    static void unlockGL(void);

private:
    // Since the class consists solely of static functions, hide the
    // default constructor and the destructor so nobody can instantiate
    // it.
    SoWw(void);
    virtual ~SoWw();

    friend class SoGuiP;
    friend class SoWwP;


    // FIXME!: audit and remove as much as possible of the remaining
    // toolkit specific parts below. 20020117 mortene.

#ifdef __COIN_SOWIN__
    public:
  static void doIdleTasks(void);
#endif // __COIN_SOWIN__

#ifdef __COIN_SOXT__
    public:
  static void nextEvent(XtAppContext, XEvent *);
  static Boolean dispatchEvent(XEvent * event);
  static XtAppContext getAppContext(void);
  static Display * getDisplay(void);
  static XmString encodeString(const char * const str);
  static char * decodeString(XmString xstring);
  static void getPopupArgs(Display * display, int screen,
                           ArgList args, int * n);

  static void registerColormapLoad(Widget widget, Widget shell);
  static void addColormapToShell(Widget widget, Widget shell);
  static void removeColormapFromShell(Widget widget, Widget shell);

  static void addExtensionEventHandler(Widget widget,
                                       int eventType, XtEventHandler proc,
                                       XtPointer clientData);
  static void removeExtensionEventHandler(Widget widget,
                                          int eventType, XtEventHandler proc,
                                          XtPointer clientData);

protected:
  static void getExtensionEventHandler(XEvent * event, Widget & widget,
                                       XtEventHandler & proc,
                                       XtPointer & clientData);
#endif // __COIN_SOXT__

#ifdef __COIN_SOGTK__
    public:
  friend class SoGtkComponent;
  enum SoGtkComponentAction { CREATION, DESTRUCTION, CHANGE };
  typedef void SoGtkComponentActionCallback(SoGtkComponent *, SoGtk::SoGtkComponentAction, void *);

  static void addComponentActionCallback(SoGtkComponentActionCallback *, void *);
  static void removeComponentActionCallback(SoGtkComponentActionCallback *, void *);

  static int getComponents(SbPList & components);

protected:
  static void invokeComponentActionCallbacks(SoGtkComponent * component,
                                             SoGtkComponentAction action);

  static gint componentCreation(SoGtkComponent * component);
  static gint componentDestruction(SoGtkComponent * component);
  static gint componentChange(SoGtkComponent * component);

private:
  static gint timerSensorCB(gpointer data);
  static gint idleSensorCB(gpointer data);
  static gint delaySensorCB(gpointer data);

  static GtkWidget * mainWidget;
  static SbPList * components;
  static SbPList * component_callbacks;
#endif // __COIN_SOGTK__
};
#endif
/*
  The BasicGLPane sections are covered by -
  
                    GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007
                       
   The remainder of the code is covered by -
                     
                       MIT License
			Copyright (c) 2023 RightHalfPlane
*/


//c++ -std=c++11 -o wxNotesSplit.x wxNotesSplit.cpp -lGLEW `/usr/local/bin/wx-config --cxxflags --libs --gl-libs` -lGLU -lGL

//c++ -std=c++11 -o wxNotesSplit.x wxNotesSplit.cpp -lGLEW `/usr/bin/wx-config --cxxflags --libs --gl-libs` -lGLU -lGL

//c++ -std=c++11 -o wxNotesSplit.x wxNotesSplit.cpp -lGLEW `/opt/local/Library/Frameworks/wxWidgets.framework/Versions/wxWidgets/3.1/bin/wx-config --cxxflags --libs --gl-libs` -Wno-deprecated-declarations


#define GLEW_IN
#ifdef GLEW_IN
#include <GL/glew.h>
#endif

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/radiobox.h>
#include <wx/textctrl.h>
#include <wx/glcanvas.h>
#include <wx/splitter.h>
#include <wx/arrstr.h>

#include <string.h>

#if !wxUSE_GLCANVAS
    #error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif


// include OpenGL
#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif
 
#include <sys/timeb.h>


double rtime(void);


 enum {
    ID_INSERT = 10004,
    ID_DELETE,
    ID_RENDER,
    TEXT_CLIPBOARD_COPY,
    TEXT_CLIPBOARD_PASTE,
    but1,
    but2,
    but3,
    but4,
    but5,
};


class BasicGLPane : public wxGLCanvas
{
    wxGLContext*	m_context;

public:
	BasicGLPane(wxFrame* parent, int* args);
	virtual ~BasicGLPane();
    
	void resized(wxSizeEvent& evt);
    
	int getWidth();
	int getHeight();
	float angle;
    
	void render(wxPaintEvent& evt);
	void render2();
	void InitOpenGl();
	int dotriangles(long n);
	int doTestSpeed(struct Scene *scene);
    
	// events
	void mouseMoved(wxMouseEvent& event);
	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);
	void OnChar(wxKeyEvent& event);
    void DeleteRow( wxCommandEvent& event );
    
	DECLARE_EVENT_TABLE()
};

class BasicGLPane2 : public wxScrolledWindow
{
    wxGLContext*	m_context;

public:
	BasicGLPane2(wxWindow *frame, const wxString& title);
	virtual ~BasicGLPane2();
    
	void resized(wxSizeEvent& evt);
    
	int getWidth();
	int getHeight();
	float angle;
	
	int noteCheckBox;
	
	wxRadioBox *radioBoxp;
	wxTextCtrl *text;
    
	void render(wxPaintEvent& evt);
	
	// events
	void mouseMoved(wxMouseEvent& event);
	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);
	void OnChar(wxKeyEvent& event);
    void DeleteRow( wxCommandEvent& event );
    void OnButton( wxCommandEvent& event );
    void OnButton2( wxCommandEvent& event );
    void radioBox( wxCommandEvent& event );
    
	DECLARE_EVENT_TABLE()
}; 
 
 
 
 enum
{
    ButtonPage_Button = wxID_HIGHEST,
    ButtonPage_ChangeNote,
    ID_RADIOBOX,
    ID_TEXTCTRL
};

char WarningBuff[256];


BasicGLPane *glPane;
 
class MyApp: public wxApp
{
    virtual bool OnInit();
    
    wxFrame *frame;
    wxFrame *frame2;
    BasicGLPane2 *glPane2;
public:
    
};

// Declare our main frame class
class MyFrame : public wxFrame
{
public:
    // Constructor
    MyFrame(const wxString& title);
};

 
bool MyApp::OnInit()
{

    if ( !wxApp::OnInit() )
        return false;
        
    MyFrame *frame = new MyFrame(wxT("Splitter-Sizer App"));

    // Show it
    frame->Show(true);

    return true;
} 

MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxPoint(-1, -1), wxSize(500, 520))
{
  wxBoxSizer *sizermain = new wxBoxSizer(wxVERTICAL);
  wxSplitterWindow *splittermain = new wxSplitterWindow(this, wxID_ANY);
  splittermain->SetSashGravity(0.5);
  splittermain->SetMinimumPaneSize(20); // Smalest size the
  sizermain->Add(splittermain, 1, wxEXPAND,0);


  BasicGLPane2 *pnl1 = new BasicGLPane2(splittermain, "Controls");
  

  wxBoxSizer *txt1sizer = new wxBoxSizer(wxVERTICAL);

  
  pnl1->SetSizer(txt1sizer);


  
    int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};

  
    glPane = new BasicGLPane((wxFrame *)splittermain, args);

  splittermain->SplitVertically(pnl1, glPane);

  this->SetSizer(sizermain);
  sizermain->SetSizeHints(this);
  
      glPane->InitOpenGl();

}

IMPLEMENT_APP(MyApp)

BEGIN_EVENT_TABLE(BasicGLPane2, wxScrolledWindow)
EVT_MOTION(BasicGLPane2::mouseMoved)
EVT_LEFT_DOWN(BasicGLPane2::mouseDown)
EVT_BUTTON(ButtonPage_Button, BasicGLPane2::OnButton)
EVT_BUTTON(ButtonPage_ChangeNote, BasicGLPane2::OnButton2)
EVT_RADIOBOX(ID_RADIOBOX, BasicGLPane2::radioBox)

END_EVENT_TABLE()
 
void BasicGLPane2::mouseMoved(wxMouseEvent& event) {}
void BasicGLPane2::mouseDown(wxMouseEvent& event) {/*printf("mouseDown 33\n");*/}
void BasicGLPane2::OnButton(wxCommandEvent& event) 
{
	exit(0);
}
void BasicGLPane2::OnButton2(wxCommandEvent& event) 
{    

}
void BasicGLPane2::radioBox(wxCommandEvent& event) 
{
}

BasicGLPane2::BasicGLPane2(wxWindow *parent, const wxString &title)
    : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                           wxHSCROLL | wxVSCROLL)
{

     
 	wxButton *about = new wxButton(this, but1, wxT("About"), wxPoint(10, 20));
 	wxButton *start = new wxButton(this, but2, wxT("Start Note"), wxPoint(10, 50));
 	wxButton *stop = new wxButton(this, but5, wxT("Stop Note"), wxPoint(10, 80));
    wxButton *info = new wxButton(this, but4, wxT("Information"), wxPoint(10, 110));

    text=new wxTextCtrl(this,ID_TEXTCTRL,wxT("A3"),
         wxDefaultPosition, wxDefaultSize);

     wxStaticText *stext=new wxStaticText(this,wxID_STATIC,wxT("Notes: (A0-C8)"),
          wxDefaultPosition, wxDefaultSize,wxALIGN_LEFT);
          
          
    wxCheckBox *check=new wxCheckBox(this,ID_TEXTCTRL,wxT("Show Note"),
         wxDefaultPosition, wxDefaultSize);
	check->SetValue(noteCheckBox);
	
	 this->SetSize(wxSize(500, 520));
}
BasicGLPane2::~BasicGLPane2()
{
	//delete m_context;
}


BEGIN_EVENT_TABLE(BasicGLPane, wxGLCanvas)
EVT_MOTION(BasicGLPane::mouseMoved)
EVT_LEFT_DOWN(BasicGLPane::mouseDown)
EVT_LEFT_UP(BasicGLPane::mouseReleased)
EVT_RIGHT_DOWN(BasicGLPane::rightClick)
EVT_LEAVE_WINDOW(BasicGLPane::mouseLeftWindow)
EVT_SIZE(BasicGLPane::resized)
EVT_KEY_DOWN(BasicGLPane::keyPressed)
EVT_KEY_UP(BasicGLPane::keyReleased)
EVT_MOUSEWHEEL(BasicGLPane::mouseWheelMoved)
EVT_PAINT(BasicGLPane::render)
EVT_CHAR(BasicGLPane::OnChar)    
EVT_BUTTON(ID_DELETE,BasicGLPane::DeleteRow )
END_EVENT_TABLE()
 
 
// some useful events to use
void BasicGLPane::mouseMoved(wxMouseEvent& event) {}
void BasicGLPane::mouseDown(wxMouseEvent& event) {/*printf("mouseDown\n");*/}
void BasicGLPane::mouseWheelMoved(wxMouseEvent& event) {}
void BasicGLPane::mouseReleased(wxMouseEvent& event) {}
void BasicGLPane::rightClick(wxMouseEvent& event) {}
void BasicGLPane::mouseLeftWindow(wxMouseEvent& event) {}
void BasicGLPane::keyReleased(wxKeyEvent& event) {}
void BasicGLPane::OnChar(wxKeyEvent& event) {}
  
int InitGL=0;



void BasicGLPane::InitOpenGl()
{

}

BasicGLPane::BasicGLPane(wxFrame* parent, int* args) :
    wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
    
}
void BasicGLPane::DeleteRow( wxCommandEvent& WXUNUSED(ev) )
{
    printf("DeleteRow\n");
}


void BasicGLPane::keyPressed(wxKeyEvent& event) 
{
    wxString key;
    long keycode = event.GetKeyCode();
    
    //printf("keycode %ld\n",keycode);
    
    if(keycode == 84){
        //buildList(&sceneG);
    	//doTestSpeed(&sceneG);
    }
  

}
int BasicGLPane::doTestSpeed(struct Scene *scene)
{
	return 0;
}

BasicGLPane::~BasicGLPane()
{
	delete m_context;
}

void BasicGLPane::resized(wxSizeEvent& evt)
{
//	wxGLCanvas::OnSize(evt);
   	 Refresh();
}
 
 
int BasicGLPane::getWidth()
{
    return GetSize().x;
}
 
int BasicGLPane::getHeight()
{
    return GetSize().y;
}
 


 
void BasicGLPane::render2( )
{
    if(!IsShown()) return;
    
}
 
 
void BasicGLPane::render( wxPaintEvent& evt )
{
 	render2( );
}
int BasicGLPane::dotriangles(long size)
{

	return 0;	
}

int GetTime(long *Seconds,long *milliseconds)
{
	struct timeb t;
	
	if(!Seconds || !milliseconds)return 1;
	

	ftime(&t);

	*Seconds=(long)t.time;
	*milliseconds=t.millitm;
	
	return 0;
}
double rtime(void)
{
	long milliseconds;
	long Seconds;
	double ret;
	
	
	GetTime(&Seconds,&milliseconds);
	
	ret=(double)Seconds+(double)milliseconds/1000.;
	
	return ret;

}



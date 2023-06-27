#include "main.h"
#include "notes.h"
#include <fftw3.h>


//c++ -std=c++11 -o FindNotes.x main.cpp notes.cpp Poly.cpp -lrtaudio  -lfftw3  -lfftw3f `/opt/local/Library/Frameworks/wxWidgets.framework/Versions/wxWidgets/3.1/bin/wx-config --cxxflags --libs --gl-libs`
//c++ -std=c++11 -o FindNotes.x main.cpp notes.cpp Poly.cpp -lrtaudio  -lfftw3  -lfftw3f `/usr/local/bin/wx-config --cxxflags --libs --gl-libs`
//c++ -std=c++11 -o FindNotes.x main.cpp notes.cpp Poly.cpp -lrtaudio  -lfftw3  -lfftw3f `wx-config --cxxflags --libs --gl-libs`


char *ProgramVersion=(char *)"FindNotes-107";

char WarningBuff[256];

IMPLEMENT_APP(MyApp)

Buttons *buttons;

bool MyApp::OnInit()
{

    buttons = new Buttons(wxT("FindNotes"));
    buttons->Show(true);

    return true;
}


#ifdef WIN64
int main(int, char**)                                           
{                                                                
    wxDISABLE_DEBUG_SUPPORT();                                   
    return wxEntry();                                            
}
#endif


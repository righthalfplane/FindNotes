#include <wx/wx.h>

#if __has_include(<rtaudio/RtAudio.h>)
#include <rtaudio/RtAudio.h>
#else
#include <RtAudio.h>
#endif

#include <fftw3.h>

#include <wx/textctrl.h>

#include <iostream>
#include <vector>

#include <string.h>

#include "Poly.h"


enum{
	but1=wxID_HIGHEST+1,
	but2,
	but3,
	but4,
	but5,
	ID_TEXTCTRL,
	CHECK_BOX,
	ID_COMBOBOX,
	INPUT_MENU,
	OUTPUT_MENU=INPUT_MENU+100,
	};


class sTone{
public:
    sTone(double frequency,double sampleRate);
    int doTone(short int *buffer,int frames);
    int doTone(float *buffer,int frames);
	~sTone();

	float dt;
	float sino;
	float coso;
	float sindt;
	float cosdt;
};

struct names{
	std::string name;
	std::vector<int> sampleRate;
	int deviceID;
};


class Buttons : public wxFrame
{
public:
	
	
    int input( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *datain );

	int printAudioInfo(int flag);

    Buttons(const wxString& title);
    void bOnQuit(wxCommandEvent& event);
    void bOnSine(wxCommandEvent& event);
    void bOnAbout(wxCommandEvent& event);
    void StartTone(wxCommandEvent& event);
    void StopTone(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnSine(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void Information(wxCommandEvent& event);
    void OnIdle(wxIdleEvent& event);
    void OnPaint( wxPaintEvent &event );
    void OnPaint2();
    void OnInputSelect(wxCommandEvent& event);
    void OnOuputSelect(wxCommandEvent& event);
    void OnCheckSelect(wxCommandEvent& event);
    void OnCombo(wxCommandEvent& event);
    int DoFTT(float *BufferIn,float *BufferOut);

    wxDECLARE_EVENT_TABLE();
    
    
	int startAudio();
        
	RtAudio *adc;

	int fsample;
	
	float *buf1;
	
	float *buf2;
	
	float *buf3;
	
	float *buf4;
	
	wxTextCtrl *text;

	class sTone *tone;
	
	wxRect *plotLocation;
	
	int bcount;
	
	std::vector<names> inputNames;
	std::vector<names> outputNames;
	
	fftwf_plan p1,p2;
	
	int noteListSave;
	
	double noteListDiff;
	
	int itWas;
	
	
	double xmin;
	double xmax;
	double dx;
	
	double ymin;
	double ymax;
	double dy;
	
	int inputID;
	int outputID;
	int SampleFrequency;
	
	struct NoteList *notelist;
	int notelistCount;
	int nodeGroup;
	
	class Poly *p[100];
	int polyCount;

	int noteCheckBox;
	
	double noteWas;
	
	wxComboBox *combo;
	
	int idebug;
	
};


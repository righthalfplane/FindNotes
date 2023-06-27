#include "notes.h"

#include <wx/init.h>

#include <wx/combobox.h>

#include <ctype.h>

struct NoteList{
	char *note;
	double frequency;
};

struct NodeGroups{
	struct NoteList *list;
	int listCount;
	char *name;
};

struct NoteList notelist0[]={

(char *)"A0",27.50,
(char *)"B0",30.50,
(char *)"C1",32.70,
(char *)"D1",36.71,
(char *)"E1",41.21,
(char *)"F1",43.65,
(char *)"G1",49.0,

(char *)"A1",55.00,
(char *)"B1",61.74,
(char *)"C2",65.41,
(char *)"D2",73.42,
(char *)"E2",82.41,
(char *)"F2",87.31,
(char *)"G2",98.00,

(char *)"A2",110.00,
(char *)"B2",123.74,
(char *)"C3",130.81,
(char *)"D3",146.83,
(char *)"E3",164.81,
(char *)"F3",174.61,
(char *)"G3",196.00,

(char *)"A3",220.00,
(char *)"B3",246.94,
(char *)"C4",261.63,
(char *)"D4",293.66,
(char *)"E4",329.63,
(char *)"F4",349.23,
(char *)"G4",392.00,

(char *)"A4",444.00,
(char *)"B4",493.88,
(char *)"C5",523.25,
(char *)"D5",587.33,
(char *)"E5",659.26,
(char *)"F5",698.46,
(char *)"G5",783.99,
   
(char *)"A5",880.00,
(char *)"B5",987.77,
(char *)"C6",1046.50,
(char *)"D6",1174.70,
(char *)"E6",1318.50,
(char *)"F6",1396.90,
(char *)"G6",1568.90,

(char *)"A6",1760.00,
(char *)"B6",1975.50,
(char *)"C7",2093.00,
(char *)"D7",2349.30,
(char *)"E7",2637.00,
(char *)"F7",2793.80,
(char *)"G7",3126.00,

(char *)"A7",3520.00,
(char *)"B7",3951.10,
(char *)"C8",4186.00,

};

struct NoteList notelist1[]={
(char *)"E2",82.41,
(char *)"A2",110.00,
(char *)"D3",146.83,
(char *)"G3",196.00,
(char *)"B3",246.94,
(char *)"E4",329.63,
};


struct NoteList notelist2[]={
(char *)"C5",523.25,
(char *)"D5",587.33,
(char *)"E5",659.26,
(char *)"F5",698.46,
(char *)"G5",783.99, 
(char *)"A5",880.00,
(char *)"B5",987.77,
};

struct NodeGroups group[]=
{
	notelist0,
	sizeof(notelist0)/sizeof(struct NoteList),
	(char *)"Piano",
	notelist1,
	sizeof(notelist1)/sizeof(struct NoteList),
	(char *)"Guitar",
	notelist2,
	sizeof(notelist2)/sizeof(struct NoteList),
	(char *)"Whistle",
};

int groupCount=sizeof(group)/sizeof(struct NodeGroups);

extern char *ProgramVersion;

FILE *out11;

void doQuit();

sTone::sTone(double frequency,double sampleRate)
{
	double pi;
    pi=4.0*atan(1.0);
    dt=1.0/((double)sampleRate);
    sino=0;
    coso=1;
    double w=2.0*pi*(frequency);
    sindt=sin(w*dt);
    cosdt=cos(w*dt);
}

int sTone::doTone(short int *buffer,int frames)
{

	if(frames <= 0)return 1;

	for(int k=0;k<frames;++k){
		double sint=sino*cosdt+coso*sindt;
		double cost=coso*cosdt-sino*sindt;
		coso=cost;
		sino=sint;
		double v=32000*cost;
		if(v > 32000)v=32000;
		if(v < -32000)v=-32000;
		buffer[k]=(short)(v);
	}

	double r=sqrt(coso*coso+sino*sino);
	coso /= r;
	sino /= r;

	return 0;
}

int sTone::doTone(float *buffer,int frames)
{

	if(frames <= 0)return 1;

	for(int k=0;k<frames;++k){
		double sint=sino*cosdt+coso*sindt;
		double cost=coso*cosdt-sino*sindt;
		coso=cost;
		sino=sint;
		double v=cost;
		buffer[k]=(v);
	}

	double r=sqrt(coso*coso+sino*sino);
	coso /= r;
	sino /= r;

	return 0;
}

sTone::~sTone()
{
	//printf("~sTone\n");
}

int fft(float *data,int nn,int isign);

int doSource(float *buffer,int frames)
{
	float out[2*32768];

	int d_nfft=32768;
	int d_nend=0;
	
	int nout=d_nfft;
		  
      	      
	int nw=0;
	int nend=d_nend;
	for(int n = 0; n < nout; n++)
	 {
		out[2*n]=0;
		out[2*n+1]=0;
		if(nw++ >= nend){
			nw=0;
			out[2*n]=1;
			out[2*n+1]=1;
		}
	 }
         
	 fft((float *)out,d_nfft,-1);
	 
	 
	double amin=1e33;
	double amax=-1e33;
		
	for(int n=0;n<frames;++n){
	    double v=out[2*n];
	    if(v > amax)amax=v;
	    if(v < amin)amin=v;
		buffer[n]=(short int)v;
	}

	//printf("amin %g amax %g\n",amin,amax);
	
	return frames;
}

int fft(float *data,int nn,int isign)
{
	double twopi,tempr,tempi,wstpr,wstpi;
	double wr,wi,theta,sinth,fni;
	int i,j,n,m,mmax,istep;

	  data -= 1;
	  j=1;
	  n=2*nn;
	  twopi=8.*atan(1.);
	   for(i=1;i<=n;i += 2){
	   if(i-j >= 0)goto L200;
	   tempr=data[j];
	   tempi=data[j+1];
	   data[j]=data[i];
	   data[j+1]=data[i+1];
	   data[i]=tempr;
	   data[i+1]=tempi;
L200:    m=n/2;
L300:    if(j-m > 0)goto L400;
		goto L500;
L400:    j=j-m;
	   m=m/2;
	   if(m-2 >= 0)goto L300;
L500:  j=j+m;
	   }
	  mmax=2;
L600:   if(mmax-n >= 0)goto L1000;
	  istep=2*mmax;
	  theta=twopi/(double)(isign*mmax);
	  sinth=sin(theta/2.);
	  wstpr=-2.*sinth*sinth;
	  wstpi=sin(theta);
	  wr=1.;
	  wi=0.;
		for(m=1;m<=mmax;m+=2){
			for( i=m;i<=n;i+=istep){
				j=i+mmax;
				tempr=wr*data[j]-wi*data[j+1];
				tempi=wr*data[j+1]+wi*data[j];
				data[j]=data[i]-tempr;
				data[j+1]=data[i+1]-tempi;
				data[i]=data[i]+tempr;
				data[i+1]=data[i+1]+tempi;
			}
			tempr=wr;
			wr=wr*wstpr-wi*wstpi+wr;
			wi=wi*wstpr+tempr*wstpi+wi;
		}
	  mmax=istep;
	  goto L600;
L1000: 

	if(isign > 0){
		fni=2.0/(double)nn;
	}else{
		fni=0.5;
	}
	double amax=-1e33;
	double amin=1e33;
	for( i=1;i<=2*nn;++i){
		data[i]=data[i]*fni;
		double v=data[i];
		if(v > amax)amax=v;
		if(v < amin)amin=v;
	}
/*		
	static int p=0;
	
	if(++p > 20){
	   fprintf(stderr,"amin %g amax %g\n",amin,amax);
	   p=0;
	}
*/		
	
	return 0;
}

wxBEGIN_EVENT_TABLE(Buttons, wxFrame)
EVT_MENU(wxID_EXIT,Buttons::OnQuit)
EVT_MENU(wxID_ABOUT,Buttons::OnAbout)

EVT_CHECKBOX(wxID_ANY,Buttons::OnCheckSelect)

EVT_COMBOBOX(ID_COMBOBOX,Buttons::OnCombo)

EVT_MENU_RANGE(INPUT_MENU,INPUT_MENU+99,Buttons::OnInputSelect)
EVT_MENU_RANGE(OUTPUT_MENU,OUTPUT_MENU+99,Buttons::OnOuputSelect)


EVT_BUTTON(but1,Buttons::bOnAbout)
EVT_BUTTON(but2,Buttons::StartTone)
EVT_BUTTON(but3,Buttons::bOnQuit)
EVT_BUTTON(but4,Buttons::Information)
EVT_BUTTON(but5,Buttons::StopTone)

EVT_IDLE(Buttons::OnIdle)
EVT_PAINT(Buttons::OnPaint)
wxEND_EVENT_TABLE()



void Buttons::OnCombo(wxCommandEvent& event){
	int ngroup=event.GetSelection();
//	printf("OnCombo %d IsChecked %d\n",item,event.IsChecked());
//	noteCheckBox=event.IsChecked();

	nodeGroup=ngroup;
	
	notelist=group[nodeGroup].list;
	
	notelistCount=group[nodeGroup].listCount;

}

void Buttons::OnCheckSelect(wxCommandEvent& event){
	//int item=event.GetId();
	//printf("OnCheckSelect %d IsChecked %d\n",item,event.IsChecked());
	noteCheckBox=event.IsChecked();
}

void Buttons::OnInputSelect(wxCommandEvent& event){
	int item=event.GetId()-INPUT_MENU;
	//printf("OnInputSelect %d INPUT_MENU %d\n",item,INPUT_MENU);
	int n=item/10;
	inputID=inputNames[n].deviceID;
	int sound=item-10*n;
	fprintf(stderr,"%s sampleRate %d\n",inputNames[n].name.c_str(),inputNames[n].sampleRate[sound]);
	SampleFrequency=inputNames[n].sampleRate[sound];
	startAudio();
}
void Buttons::OnOuputSelect(wxCommandEvent& event){
	int item=event.GetId()-OUTPUT_MENU;
	fprintf(stderr,"%s sampleRate %d\n",outputNames[item].name.c_str(),fsample);
	//printf("OnOuputSelect %d OUTPUT_MENU %d\n",item,OUTPUT_MENU);
	outputID=item;
	startAudio();
}



int input( void * /*outputBuffer*/, void *inputBuffer, unsigned int nBufferFrames,
           double /*streamTime*/, RtAudioStreamStatus /*status*/, void *datain );


void Buttons::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
	wxPaintDC dc(this);

	wxFont font1(50,wxFONTFAMILY_ROMAN,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD);
	wxRect rs(15,10,130,250);
	
	dc.SetFont(font1);
	dc.SetTextForeground(*wxBLACK);
	dc.SetBackgroundMode(wxBRUSHSTYLE_TRANSPARENT);
	
	dc.SetBrush(*wxLIGHT_GREY_BRUSH);
	
	dc.DrawRectangle(rs);
	
	dc.SetBrush(*wxWHITE);

	wxPoint rp(300,130);
	dc.DrawCircle(rp,220/2);
	
	
	dc.SetTextForeground(*wxBLACK);
	
	//printf("OnPaint noteListSave %d\n",noteListSave);
	if(noteListSave < 0){
		dc.DrawText("00", rp.x-25, rp.y+20);
	}else{
		dc.DrawText(notelist[noteListSave].note, rp.x-25, rp.y+20);	
	}
	
	
	double rBig=200/2;
	double rSmall=10/2;
	
	wxPoint Triangle[3];
	
	double pi=4.0*atan(1.0);
	double angle=noteListDiff;
	if(angle < -45)angle = -45;
	if(angle >  45)angle =  45;

	if(fabs(angle) <= 1.5){
		dc.SetPen(wxPen(*wxGREEN));
		dc.SetBrush(*wxGREEN);
	}else{
		dc.SetPen(wxPen(*wxRED));
		dc.SetBrush(*wxRED);
	}	
	
	//fprintf(stderr,"angle %g\n",angle);
	
	angle=angle*pi/180.0;
	
	Triangle[0].x=-rSmall*cos((angle-pi/4.0));
	Triangle[0].y=-rSmall*sin((angle-pi/4.0));
	Triangle[1].x=rSmall*cos((angle+pi/4.0));
	Triangle[1].y=rSmall*sin((angle+pi/4.0));
	
	Triangle[2].y = -rBig*cos(angle);
	Triangle[2].x =  rBig*sin(angle);


	//fprintf(stderr,"angle %g x %d y %d\n",angle,Triangle[2].x,Triangle[2].y);
	

	dc.DrawPolygon(WXSIZEOF(Triangle),Triangle,	rp.x,rp.y);
	
	dc.DrawCircle(rp,10/2);
	
	
	dc.SetPen(wxPen(*wxBLACK));
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	
	double xs=rBig*sin(pi/4)+rp.x;
	double ys=-rBig*cos(pi/4)+rp.y;
	
	double xss=(rBig-10)*sin(pi/4)+rp.x;
	double yss=-(rBig-10)*cos(pi/4)+rp.y;
	
	double xe=rBig*sin(-pi/4)+rp.x;
	double ye=-rBig*cos(-pi/4)+rp.y;
	
	double xee=(rBig-10)*sin(-pi/4)+rp.x;
	double yee=-(rBig-10)*cos(-pi/4)+rp.y;
	
	dc.DrawArc(xs,ys,xe,ye,rp.x,rp.y);
	
	dc.DrawLine(xss, yss, xs, ys);

	dc.DrawLine(xee, yee, xe, ye);
	
	
	xe=rBig*sin(0)+rp.x;
	ye=-rBig*cos(0)+rp.y;
	
	xee=(rBig-10)*sin(0)+rp.x;
	yee=-(rBig-10)*cos(0)+rp.y;

	dc.DrawLine(xee, yee, xe, ye);

	
	
	dc.SetPen(wxPen(*wxWHITE_PEN));
	dc.SetBrush(*wxWHITE);
	dc.DrawRectangle(*plotLocation);
	
	dc.SetPen(wxPen(*wxBLACK_PEN));
	
	if(noteListSave < 0)return;

	//dc.DrawLine(plotLocation->x, plotLocation->y, plotLocation->x+plotLocation->width-1,plotLocation->y+plotLocation->height-1);
	
	double ixmin=plotLocation->x;
	double ixmax=plotLocation->x+plotLocation->width-1;
	double idx=ixmax-ixmin;
	//idx = idx*2.0;
	idx = idx*fsample/5000;
	
	double iymin=plotLocation->y;
	double iymax=plotLocation->y+plotLocation->height-1;
	double idy=iymin-iymax;
		
	int ixxmin,ixxmax,iyymin,iyymax;
	
	ixxmin=100000000;
	ixxmax= -100000000;
	iyymin=100000000;
	iyymax= -100000000;
	
	
	for(int n=0;n<5000;++n){
		float sum=buf2[2*n]*buf2[2*n]+buf2[2*n+1]*buf2[2*n+1];
		if(sum > 0.0)sum=sqrt(sum);
		double x=n;
		double y=sum;
		int ixold;
		int iyold;
		int ix;
		int iy;
		ix=(int)((x-xmin)*idx/dx+ixmin);
		if(ix < ixxmin)ixxmin=ix;
		if(ix > ixxmax)ixxmax=ix;
		iy=(int)((y-ymin)*idy/dy+iymax);
		if(iy < iyymin)iyymin=iy;
		if(iy > iyymax)iyymax=iy;
		if(n == 0){
		  ixold=ix;
		  iyold=iy;
		}
		dc.DrawLine(ixold, iyold, ix, iy);
		ixold=ix;
		iyold=iy;
	}
	
	if(noteCheckBox){
		dc.SetPen(wxPen(*wxRED,1,wxPENSTYLE_DOT));
		int iymin=plotLocation->y;
	    int iymax=plotLocation->y+plotLocation->height-1;
	    double ixmin=plotLocation->x;
		double ixmax=plotLocation->x+plotLocation->width-1;
		double idx=ixmax-ixmin;
		idx = idx*fsample/5000;
		int ix=(int)((noteWas-xmin)*idx/dx+ixmin);
		//fprintf(stderr,"ix %d iymin %d iymax %d\n",ix,iymin,iymax);
		dc.DrawLine(ix, iymin, ix, iymax);	
	}
/*
	fprintf(stderr,"fsample %d ixxmin %d ixxmax %d iyymin %d iyymax %d\n",fsample,ixxmin,ixxmax,iyymin,iyymax);
	fprintf(stderr,"fsample %d ixmin %g ixmax %g iymin %g iymax %g\n",fsample,ixmin,ixmax,iymin,iymax);
	fprintf(stderr,"fsample %d xmin %g xmax %g ymin %g ymax %g\n\n",fsample,xmin,xmax,ymin,ymax);
*/
    //printf("this %d %d\n",this->GetSize().x,this->GetSize().y);

}
void Buttons::OnIdle(wxIdleEvent& event){
	static long int count;
	
	if(noteListSave >= 0){
	   if(itWas == noteListSave)return;
	  // fprintf(stderr,"OnIdle noteListSave %d\n",noteListSave);
	    Refresh();
	    Update();
	    itWas=noteListSave;
	}else{
		itWas=-1;
	}
	
    //fprintf(stderr,"OnSine count %ld\n",count++);
}

int input( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *datain )
{
	Buttons *in=(Buttons *)datain;
  	
  	in->input(outputBuffer,inputBuffer,nBufferFrames,streamTime,status,datain);
  	
    return 0;
}

int Buttons::input( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *datain )
 {
 	float *output=(float  *)outputBuffer;
 	float *input=(float  *)inputBuffer;
 	
	//printf("Buttons::input nBufferFrames %u\n",nBufferFrames);
	
	for(unsigned int n=0;n<nBufferFrames;++n){
		output[n]=input[n];
		output[n]=0;
		buf1[2*bcount]=input[n];
		buf1[2*bcount+1]=0;
		//buf2[2*bcount]=0;
		//buf2[2*bcount+1]=0;
		//buf3[n]=input[n];
		//buf3[2*bcount]=1;
		//buf3[2*bcount+1]=1;
		bcount++;
	}
	
	if(tone)tone->doTone(output,nBufferFrames);
	
	//fwrite(&buf1[2*(bcount-nBufferFrames)],8,(long)(nBufferFrames),out11);

	if(bcount >= fsample){
		bcount=0;
		noteListSave = -1;
		noteListDiff =  0;

/*
		fftwf_execute(p2);
		
		for(int n=0;n<fsample;++n){
			buf1[2*n] = buf4[2*n]/fsample;
			buf1[2*n+1] = buf4[2*n+1]/fsample;
			
			buf3[n]=input[n];
		}
		
		p[5]->forceCascadeRun(buf1,buf1,fsample,0);
*/

		fftwf_execute(p1);
		
		//DoFTT(buf1,buf2);
		
		float fmax = -1e33;
		int imax=-1;
		float sum=0;
		for(int n=0;n<fsample;++n){
			sum=buf2[2*n]*buf2[2*n]+buf2[2*n+1]*buf2[2*n+1];
			if(sum > 0.0)sum=sqrt(sum);
			buf1[n]=sum;
			if(sum > fmax){
				fmax=sum;
				imax=n;
			}
			
		}
		
		
		//fprintf(stderr,"fsample %d fmax %g sum %g\n",fsample,fmax,sum);
		
		ymax=fmax;
		dy=ymax-ymin;
		
				
		if(imax < 27 || imax > 4186){
			//fprintf(stderr,"bcount %d fsample %d nBufferFrames %d fmax %g imax %d\n",bcount,fsample,nBufferFrames,fmax,imax);
		}else{
			double min=10000;
			int imin=-1;
			for(int n=0;n<notelistCount;++n)
			{
				double diff=fabs(imax-notelist[n].frequency);
			      if(diff < min){
			         noteListDiff=imax-notelist[n].frequency;
			         min=diff;
			         imin=n;
			      }
			}
			
			if(imin >= 0){
				noteListSave=imin;
				if(idebug)fprintf(stderr,"Frequency %d note %s diff %g fmax %g",
				       imax,notelist[imin].note,noteListDiff,fmax);				       
				double v=0;
				int nn=-10;
				for(int n=0;n<(int)(0.5*(imax+0.5*imax));++n)
				{
					if(buf1[n] > v){
					    v=buf1[n];
					    nn=n;
					}
				}
				if(idebug)fprintf(stderr," v %g nn %d ",v,nn);
				
			}
			
	
			if(nodeGroup > 0){
				for(int n=0;n<notelistCount;++n){
					p[n]->forceCascadeRun(buf3,buf4,fsample,0);
					if(idebug)fprintf(stderr,"(%d %g)",n,(p[n]->amax-p[n]->amin));
				}
				
			}
			
			//p[0]->forceCascadeRun(buf2,buf2,fsample,0);
			
			//fprintf(stderr," amin %g amax %g\n",p[0]->amin,p[0]->amax);

			if(idebug)fprintf(stderr,"\n");
			
		}
		wxWakeUpIdle();

	    //fwrite(buf2,8,(long)(bcount),out11);

	}
	
 	return 1;
 }       



Buttons::Buttons(const wxString& title)
       : wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(500, 520))
{

	adc=NULL;
	
	buf1=NULL;
	
	buf2=NULL;
	
	buf3=NULL;
	
	buf4=NULL;
	
	text=NULL;
	
	tone=NULL;
	
	plotLocation=NULL;
	
	idebug=0;
	
	inputID  = -1;
	outputID = -1;
	SampleFrequency = -1;
	
	nodeGroup=0;
	
	notelist=group[nodeGroup].list;
	
	notelistCount=group[nodeGroup].listCount;
	
	noteCheckBox=0;
	
	noteWas=-10;
	
	for(int n=0;n<100;++n){
		p[n]=NULL;
	}
	
 	wxButton *about = new wxButton(this, but1, wxT("About"), wxPoint(30, 20),wxSize(100, 25));
    wxButton *info = new wxButton(this, but4, wxT("Information"), wxPoint(30, 50),wxSize(100, 25));
	

     wxStaticText *stext=new wxStaticText(this,wxID_STATIC,wxT("Notes: (A0-C8)"),
          wxPoint(30, 80), wxSize(100, 25),wxALIGN_LEFT);
          
          
    wxCheckBox *check=new wxCheckBox(this,ID_TEXTCTRL,wxT("Show Note"),
         wxPoint(30, 110), wxDefaultSize);
	check->SetValue(noteCheckBox);
	
    text=new wxTextCtrl(this,ID_TEXTCTRL,wxT("A3"),
         wxPoint(30, 140), wxSize(100, 25));
	
	
	wxArrayString strings;
	

	for(int n=0;n<groupCount;++n){
		strings.Add(group[n].name);
	}

 	wxButton *start = new wxButton(this, but2, wxT("Start Note"), wxPoint(30, 170),wxSize(100, 25));
 	
 	wxButton *stop = new wxButton(this, but5, wxT("Stop Note"), wxPoint(30, 200),wxSize(100, 25));
	
	combo = new wxComboBox(this,ID_COMBOBOX,wxT("Instrument"),
	 			wxPoint(30, 230),wxSize(100, 25),strings,wxITEM_DROPDOWN);
	 			
	combo->SetSelection(0);
	

  	printAudioInfo(1);
  	
    if(idebug)printf("inputNames %lu outputNames %lu\n",inputNames.size(),outputNames.size());

    wxMenu *menuFile = new wxMenu(_T(""), wxMENU_TEAROFF);
    menuFile->Append(wxID_OPEN, _T("&Information Audio...\tCtrl-O"), _T("Infomation"));
    
    menuFile->Append(wxID_EXIT, _T("E&xit\tAlt-X"), _T("Quit this program"));
    
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, _T("&About...\tCtrl-A"), _T("Show about dialog"));

    wxMenu *inputMenu = new wxMenu;
    
    for(long int n=0;n<inputNames.size();++n){
        wxMenu *subMenu = new wxMenu;
        inputMenu->AppendSubMenu(subMenu, inputNames[n].name, wxT("Description?"));
        //int deviceID=inputNames[n].deviceID;
        for(long int k=0;k<inputNames[n].sampleRate.size();++k){
            std::stringstream srateName;
            srateName << ((float) (inputNames[n].sampleRate[k]) / 1000.0f) << "kHz";
            wxMenuItem *itm = subMenu->AppendRadioItem(INPUT_MENU + 10*n + k, srateName.str(), wxT("Description?"));
        }
    }
   
    wxMenu *outputMenu = new wxMenu;
    for(long int n=0;n<outputNames.size();++n){
        int deviceID=outputNames[n].deviceID;
    	outputMenu->Append(OUTPUT_MENU+deviceID, outputNames[n].name, _T("Show about dialog"));
  	}
  	
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, _T("&File"));
    
    menuBar->Append(inputMenu, _T("Input Devices"));
    
    menuBar->Append(outputMenu, _T("Output Devices"));
    
    menuBar->Append(helpMenu, _T("&Help"));
    
    SetMenuBar(menuBar);
    
    startAudio();
    
    this->SetSize(wxSize(500, 560));
/*
    printf("panel %d %d\n",panel->GetSize().x,panel->GetSize().y);
    printf("midPan %d %d\n",midPan->GetSize().x,midPan->GetSize().y);
    printf("this %d %d\n",this->GetSize().x,this->GetSize().y);
*/

    
}

int Buttons::startAudio()
{
	
	if(adc){
	   delete adc;
	}
	
	adc=new RtAudio;
	if ( adc->getDeviceCount() < 1 ) {
		printf("\nNo audio devices found!\n");
    	exit( 1 );
  	}
  	
	if(idebug)printf("startAudio getDeviceCount %d inputID %d outputID %d\n",(int)adc->getDeviceCount(),inputID,outputID);
	
	
	RtAudio::StreamParameters parameters;
	parameters.nChannels = 1;
	parameters.firstChannel = 0;
	
	if(outputID < 0){
		parameters.deviceId = adc->getDefaultOutputDevice();
	}else{
		parameters.deviceId = outputID;
	}
		
	RtAudio::StreamParameters ParamsIn;
	ParamsIn.nChannels = 1;
	ParamsIn.firstChannel = 0;
	
	if(inputID < 0){
		ParamsIn.deviceId = adc->getDefaultInputDevice();
	}else{
		ParamsIn.deviceId = inputID;
	}

	if(SampleFrequency < 0){
		fsample=44100;
	}else{
		fsample=SampleFrequency;
	}
	
	
	if(buf1)delete buf1;
    buf1 = new float[(int)(sizeof(float)*4*fsample)];
    
	if(buf2)delete buf2;
    buf2 = new float[(int)(sizeof(float)*4*fsample)];
    		
	if(buf3)delete buf3;
    buf3 = new float[(int)(sizeof(float)*4*fsample)];
    		
	if(buf4)delete buf4;
    buf4 = new float[(int)(sizeof(float)*4*fsample)];
    		
	p1 = fftwf_plan_dft_1d(fsample,(fftwf_complex *)buf1, (fftwf_complex *)buf2, FFTW_FORWARD, FFTW_ESTIMATE);
	
	p2 = fftwf_plan_dft_1d(fsample,(fftwf_complex *)buf3, (fftwf_complex *)buf4, FFTW_BACKWARD, FFTW_ESTIMATE);

	bcount=0;
	
	//out11=fopen("test1_IQ_162400000_44100_fc.raw","wb");
	//out11=fopen("test1_IQ_162400000_882000_fc.raw","wb");
	
	noteListSave = -1;
	
	if(plotLocation)delete plotLocation;
	
	plotLocation=new wxRect(40,280,400,200);

	xmin=0;
	xmax=fsample;
	dx=xmax-xmin;
	
	ymin=0;
	ymax=0;
	dy=ymax-ymin;
	
	if(tone)delete tone;
	
	tone=NULL;
	
	class Poly *pold[100];
    for(int n=0;n<100;++n){
        pold[n]=p[n];
    }

	
	for(int n=0;n<notelistCount;++n){
		p[n]=new Poly((float)fsample);
	}
	
	//p[0]->Clowpass("butter",16,1.0,0.5*(notelist[0].frequency));
	
	p[0]->Cbandpass("butter",16,1.0,0.5*(notelist[0].frequency),
				 0.5*(notelist[0].frequency+notelist[1].frequency));


	for(int n=1;n<notelistCount-1;++n){
		p[n]->Cbandpass("butter",16,1.0,0.5*(notelist[n-1].frequency+notelist[n].frequency),
						 0.5*(notelist[n].frequency+notelist[n+1].frequency));
	}

	//p[notelistCount-1]->Chighpass("butter",16,1.0,notelist[notelistCount-1].frequency);
	
	int nn=notelistCount-1;
	p[nn]->Cbandpass("butter",16,1.0,0.5*(notelist[nn-1].frequency+notelist[nn].frequency),
						                 (notelist[nn].frequency+0.2*notelist[nn].frequency));
	
	
	for(int n=0;n<notelistCount;++n){
       p[n]->forceCascadeStart();
    }

    for(int n=0;n<notelistCount;++n){
       p[n]->forceCascadeStart();
       if(pold[n])p[n]->gain=pold[n]->gain;
    }

    for(int n=0;n<100;++n){
        if(pold[n])delete pold[n];
        pold[n]=NULL;
    }
	

	unsigned int bufferFrames = fsample/20;
	
  	try {
    	adc->openStream( &parameters, &ParamsIn, RTAUDIO_FLOAT32, fsample, &bufferFrames, &::input, (void *)this );
    	adc->startStream();
  	}
  	catch ( RtAudioError& e ) {
    	std::cout << '\n' << e.getMessage() << '\n' << std::endl;
    	goto cleanup;
    }
    
cleanup:


	return 0;
}


void Buttons::bOnAbout(wxCommandEvent& event)
{
    wxString msg;
    
    msg.Printf("%s\n",ProgramVersion);
    
    wxMessageBox(msg, _T("About Notes"), wxOK | wxICON_INFORMATION, this);
}

void Buttons::StartTone(wxCommandEvent& event)
{

   wxString value=text->GetValue();
   
   const char *note=value.mb_str().data();
   
   char noteUpper[256];
   
   strncpy(noteUpper,note,sizeof(noteUpper));
   
   char *p = noteUpper;

	while (*p) {
    	*p = toupper(*p);
    	p++;
	}
   
   int find = -1;
	for(int n=0;n<sizeof(notelist0)/sizeof(struct NoteList);++n)
	{
		if(!strcmp(noteUpper,notelist0[n].note)){
			find=n;
		}
	}

	if(find < 0){
		fprintf(stderr,"Piano Note %s not found \n",noteUpper);
		return;
	}

	if(idebug)printf("Start Tone note %s find %d frequency %g\n",note,find,notelist0[find].frequency);
	
	if(tone)delete tone;
	
	tone= new sTone(notelist0[find].frequency,fsample);
	
	noteWas=notelist0[find].frequency;


}
void Buttons::StopTone(wxCommandEvent& event){

	if(idebug)printf("Stop Tone\n");
	
	if(tone)delete tone;
	tone=NULL;
	
}


void Buttons::bOnSine(wxCommandEvent& event){printf("OnSine\n");}
void Buttons::bOnQuit(wxCommandEvent& event){printf("bOnQuit\n");doQuit();}

void Buttons::OnAbout(wxCommandEvent& event){printf("OnAbout\n");Buttons::bOnAbout(event);}
void Buttons::OnSine(wxCommandEvent& event){printf("OnSine\n");}
void Buttons::OnQuit(wxCommandEvent& event){doQuit();}

void Buttons::Information(wxCommandEvent& event){
	printAudioInfo(0);
	
	fprintf(stderr,"%s\n",group[nodeGroup].name);
	
	for(int n=0;n<notelistCount;++n){
		fprintf(stderr,"(%s %g) ",group[nodeGroup].list[n].note,group[nodeGroup].list[n].frequency);	
		if((n+1) % 10 == 0)fprintf(stderr,"\n");
	}		
	fprintf(stderr,"\n\n");
	
	fprintf(stderr,"Sample Rate %d\n\n",fsample);
	
    fprintf(stderr,"DeviceCount %d inputID %d outputID %d\n",(int)adc->getDeviceCount(),inputID,outputID);


}


int Buttons::DoFTT(float *BufferIn,float *BufferOut)
{

	int n2=2;
	int ifound=FALSE;
	for(int n=0;n<29;++n){
		if(n2 >= fsample){
			ifound=TRUE;
			break;
		}
		n2 *= 2;
	}
	
	if(!ifound){
	    fprintf(stderr,"DoFTT Did not find power of 2 fsample %d\n",fsample);
	    return 1;
	}

	//fprintf(stderr,"n2 %d fsample %d\n",n2,fsample);

	for(int n=0;n<n2;++n){
		if(n < fsample){
			BufferOut[2*n]=BufferIn[2*n];
			BufferOut[2*n+1]=BufferIn[2*n+1];
		}else{
			BufferOut[2*n]=0;
			BufferOut[2*n+1]=0;
		}
		
	}
	
	fft(BufferOut,n2,-1);

	return 0;
}

void doQuit()
{
    wxWindowList::Node *node = wxTopLevelWindows.GetFirst();
    while ( node )
    {
        wxWindow *win = node->GetData();
        node = node->GetNext();
        if (win->IsKindOf(CLASSINFO(wxFrame)) )
        {
            wxFrame *frame = (wxFrame *)win;
            frame->Close(TRUE);
        }
    }
    
    return;
}
int Buttons::printAudioInfo(int flag)
{
	RtAudio dac;
	
	
	
	
	int deviceCount=dac.getDeviceCount();

	if (deviceCount < 1 ) {
		printf("\nNo audio devices found!\n");
		exit(0);
	}
	
	struct names name;

	inputNames.clear();
	outputNames.clear();

	if(flag == 0)printf("\nAudio device Count %d default output device %d \n",deviceCount,dac.getDefaultOutputDevice());

	RtAudio::DeviceInfo info;
	for (int i=0; i<deviceCount; i++) {

		try {
			info=dac.getDeviceInfo(i);
			if(info.outputChannels > 0){
			// Print, for example, the maximum number of output channels for each device
				if(flag == 0)printf("audio device = %d : output  channels = %d Device Name = %s",i,info.outputChannels,info.name.c_str());
				name.name=info.name;
				name.deviceID=i;
				name.sampleRate.clear();
				if(info.sampleRates.size()){
					if(flag == 0)printf(" sampleRates = ");
					for (int ii = 0; ii < (int)info.sampleRates.size(); ++ii){
						if(flag == 0)printf(" %d ",info.sampleRates[ii]);
						name.sampleRate.push_back(info.sampleRates[ii]);
				   }
				}
				if(flag == 0)printf("\n");
				outputNames.push_back(name);
			 }
 
			if(info.inputChannels > 0){
			// Print, for example, the maximum number of output channels for each device
				if(flag == 0)printf("audio device = %d : input   channels = %d Device Name = %s",i,info.inputChannels,info.name.c_str());
				name.name=info.name;
				name.deviceID=i;
				name.sampleRate.clear();
				if(info.sampleRates.size()){
					if(flag == 0)printf(" sampleRates = ");
					for (int ii = 0; ii < (int)info.sampleRates.size(); ++ii){
						if(flag == 0)printf(" %d ",info.sampleRates[ii]);
						name.sampleRate.push_back(info.sampleRates[ii]);
			        }
				}
				if(flag == 0)printf("\n");
				inputNames.push_back(name);
		   }

		}
		catch (RtAudioError &error) {
			error.printMessage();
			break;
		}

	}

	if(flag == 0)printf("\n");

	return 0;
}


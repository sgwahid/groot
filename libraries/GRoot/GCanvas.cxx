

#include <GCanvas.h>

#include <string>

#include <globals.h>
#include <GCommands.h>
#include <GMarker.h>
#include <GH1D.h>

#include <KeySymbols.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TClass.h>
#include <TFrame.h>
#include <TAxis.h>

#include <TRootCanvas.h>
#include <TVirtualX.h>


int GCanvas::fCanvasNumber = 0;


GCanvas::GCanvas(bool build) : 
  TCanvas(build) { Init(); }

GCanvas::GCanvas(const char* name, const char* title, int wtopx, int wtopy, int ww, int wh) :
  TCanvas(name,title,wtopx,wtopy,ww,wh) { Init(name,title); }

GCanvas::GCanvas(const char* name, const char* title, int ww, int wh) : 
  TCanvas(name,title,ww,wh) { Init(name,title); }

GCanvas::GCanvas(const char* name, const char* title,int form) :
  TCanvas(name,title,form) { Init(name,title); }

GCanvas::GCanvas(const char* name, int ww, int wh, int winid) :
  TCanvas(name,ww,wh,winid) { Init(name); }

GCanvas::~GCanvas() { }  

void GCanvas::Init(const char* name, const char* title) {
  std::string temp   = Form("canvas_%i",fCanvasNumber++); 
  std::string sname  = name;  
  std::string stitle = title;

  if(!sname.length())  this->SetName(temp.c_str());
  if(!stitle.length()) this->SetTitle(temp.c_str());

  gClient->Connect("ProcessedEvent(Event_t *,Window_t)","GCanvas",this,"EventProcessed(Event_t*)");

}

void GCanvas::EventProcessed(Event_t *event) {
//ProcessedEvent(int event, int x, int y, TObject *selected) {
/*
  printf("-------------\n");
  printf("Event Processed\n");
  printf("\tevent  = %i\n",event->fCode);
  printf("\tevent  = 0x%08x\n",event->fCode);
  printf("\tstate  = 0x%08x\n",event->fState);
  printf("\ttype   = 0x%08x\n",event->fType);   // 0 down, 1 up
  printf("\twindow = %lu\n",event->fWindow);
  printf("\tx      = 0x%08x\n",event->fX);
  printf("\ty      = 0x%08x\n",event->fY);
*/
  if(static_cast<unsigned long>(event->fWindow) != 
     static_cast<unsigned long>(gVirtualX->GetWindowID(this->GetCanvasID()))) 
    return;
  //switch(event->fCode) {

  if(event->fType!=0) return; // only look for key down...

  unsigned int keysym;
  char str[2];
  gVirtualX->LookupString(event, str, sizeof(str), keysym);


  //bool GCanvas::HandleArrowPress(EEventType event, int px, int py) {
  //switch(event->fCode) {
  switch(keysym) {
    case kKey_Left:
    case kKey_Up:
    case kKey_Right:
    case kKey_Down:
      //printf("arrow key pressed.\n");
      //printf("keysym = 0x%08x\n",keysym);
      HandleArrowPress(kArrowKeyPress,keysym,keysym);
      break;
    defualt:
      break;
  }
  
  //printf("\tobject = %s\n",selected ? selected->GetName() : "");
  
  
  return;
}



void GCanvas::HandleInput(EEventType event, int px, int py) {
  bool handled = false;


  //check the event,
  switch(event) {
    case kKeyPress:
      handled = HandleKeyPress(event,px,py);
      break;
    case kArrowKeyPress:
    case kArrowKeyRelease:
      handled = HandleArrowPress(event,px,py);
      break;
    case kButton1Down:
    case kButton1Shift:
    case kButton1Up:
    case kButton1Double:
      printf("clicked on %s\n",GetSelected()->IsA()->GetName());
      if(GetSelected()->InheritsFrom(TH1::Class()) ||
         GetSelected()->InheritsFrom(TFrame::Class()) )
        handled = HandleMouseButton1(event,px,py);
      break;
    case kButton1Motion:
    case kButton1ShiftMotion: 
      //handled = true;
      break;
    default:
      break;
  }
  //printf(RED "handled = %i" RESET_COLOR  "\n",handled);
  if(!handled) 
    TCanvas::HandleInput(event,px,py);
}


bool GCanvas::HandleMouseButton1(EEventType event, int px, int py) { 
  bool handled = false;
  TH1 *gHist = 0;
  switch(event) {
    case kButton1Down:
      gHist = GrabHist();
      //TVirtualPad *sPad = TCanvas::GetSelectedPad();
      if(gPad != TCanvas::GetSelectedPad()) {
        //switch the gPad to the clicked pad... 
        TCanvas::HandleInput(kButton2Down,px,py);
      } else if(gHist && gPad) {
        GMarker *m = new GMarker();
        //printf(RED "adding marker\n" RESET_COLOR "\n"); fflush(stdout);
        double xx = gPad->AbsPixeltoX(px);
        double x  = gPad->PadtoX(xx);
        //int  binx = h->GetXaxis()->FindBin(x);
        m->AddTo(gHist,x);
        handled = true;
        
        gPad->Modified();
        gPad->Update();
      }
      break;
    case kButton1Shift:
      break;
    case kButton1Up:
      break;
    case kButton1Double:
      break;
    default:
      break; 
  }

  return handled;
}

bool GCanvas::HandleArrowPress(EEventType event, int px, int py) {
  bool handled  = false;
  bool doUpdate = false;
  TH1 *gHist = 0;
  //printf("HandleArrowPress()\tevent = %i\tpx = %i\tpy = %i\n",event,px,py); fflush(stdout);
  gHist = GrabHist();
  switch(px) {
    //gHist->GetXaxis()->GetXmin() // minimum x
    //gHist->GetXaxis()->GetXmax() // maximum x
    //gPad->GetUxmin() // current min
    //gPAd->GetUxmax() // current max
    case kKey_Left:
      if(gHist && gHist->GetDimension()==1) {
        double halfWindow = fabs(0.5*(gPad->GetUxmax() - gPad->GetUxmin()));
        if((gPad->GetUxmin() - halfWindow)<gHist->GetXaxis()->GetXmin()) 
          halfWindow = fabs(gPad->GetUxmin()-gHist->GetXaxis()->GetXmin());
        gHist->GetXaxis()->SetRangeUser(gPad->GetUxmin()-halfWindow,gPad->GetUxmax()-halfWindow);
      } 
      handled  = true;
      doUpdate = true;
      break;
    case kKey_Up:
      break;
    case kKey_Right:
      if(gHist && gHist->GetDimension()==1) {
        double halfWindow = fabs(0.5*(gPad->GetUxmax() - gPad->GetUxmin()));
        if((gPad->GetUxmax() + halfWindow)>gHist->GetXaxis()->GetXmax()) 
          halfWindow = fabs(gHist->GetXaxis()->GetXmax() - gPad->GetUxmax());
        gHist->GetXaxis()->SetRangeUser(gPad->GetUxmin()+halfWindow,gPad->GetUxmax()+halfWindow);
      } 
      handled  = true;
      doUpdate = true;
      break;
    case kKey_Down:
      //printf("ARROW!!\n"); fflush(stdout);
      break;
    default:
      break;
  }
  if(doUpdate) {
    gPad->Modified();
    gPad->Update();
  }
  return handled;
}


bool GCanvas::HandleKeyPress(EEventType event, int px, int py) {
  bool handled  = false;
  bool doUpdate = false;
  TH1 *gHist = 0;
  //printf("HandleKey()\tevent = %i\tpx = %i\tpy = %i\n",event,px,py); fflush(stdout);
  //printf("key: %i  %i  %i\n",event,px,py);
  switch(py) {
    case kKey_Space:
      printf("--space--\n");
      //printf("this->GetName() = %s\n",this->GetName());
      TCanvas::HandleInput(kButton2Down,px,py);
      handled = true;
      break;
    case kKey_e:
      gHist = GrabHist();
      if(gHist) {
        std::vector<GMarker*> markers = GMarker::GetAll(gHist);
        if(markers.size()>1) {
          double xlow  = markers.at(0)->X();
          double xhigh = markers.at(1)->X();
          if(xlow>xhigh) std::swap(xlow,xhigh);
          gHist->GetXaxis()->SetRangeUser(xlow,xhigh);
          GMarker::RemoveAll(gHist);
          doUpdate=true;
          handled=true;
        }
      }
      break;
    case kKey_f:
      gHist=GrabHist();
      if(gHist) {
        std::vector<GMarker*> markers = GMarker::GetAll(gHist);
        if(markers.size()>1) {
          if(PhotoPeakFit(gHist,markers.at(0)->X(),markers.at(1)->X())) {
            doUpdate=true;
            handled=true;
            GMarker::RemoveAll(gHist);
          }
        }
      }
      break;
    case kKey_g:
      gHist=GrabHist();
      if(gHist) {
        std::vector<GMarker*> markers = GMarker::GetAll(gHist);
        if(markers.size()>1) {
          if(GausFit(gHist,markers.at(0)->X(),markers.at(1)->X())) {
            doUpdate=true;
            handled=true;
            GMarker::RemoveAll(gHist);
          }
        }
      }
      break;
    case kKey_n:
      gHist=GrabHist();
      if(gHist) {
        TListIter iter(gHist->GetListOfFunctions());
        std::vector<TF1*> funcs;
        while(TObject *obj=iter.Next()) {
          if(obj->InheritsFrom(TF1::Class()))
            funcs.push_back(((TF1*)obj));
        }
        for(auto i=funcs.begin();i!=funcs.end();i++)
          gHist->GetListOfFunctions()->Remove(*i);
        GMarker::RemoveAll(gHist);
        gHist->Sumw2(false);

      }
      doUpdate = true;
      handled  = true;
      break;
    case kKey_o:
      //printf("\tkey: %i  %i  %i\n",event,px,py);
      gHist = GrabHist();
      if(gHist && gHist->GetDimension()==1) {    
        gHist->GetXaxis()->UnZoom();
      } else if(gHist && gHist->GetDimension()==2) {  
        ((TH2*)gHist)->GetXaxis()->UnZoom();
        ((TH2*)gHist)->GetYaxis()->UnZoom();
      }
      doUpdate = true;
      handled  = true;
      break;
    case kKey_w:
      gHist = GrabHist();
      if(gHist && gHist->InheritsFrom(GH1D::Class())) {
        double xlow = gHist->GetXaxis()->GetBinLowEdge(gHist->GetXaxis()->GetFirst());
        double xup  = gHist->GetXaxis()->GetBinUpEdge(gHist->GetXaxis()->GetLast());
        gHist->Rebin(2);
        gHist->GetXaxis()->SetRangeUser(xlow,xup);
        doUpdate = true;
        handled  = true;
      }
      break;
    case kKey_q:
      gHist = GrabHist();
      if(gHist && gHist->InheritsFrom(GH1D::Class())) {
        GH1D *ggHist = dynamic_cast<GH1D*>(gHist);
        double xlow = ggHist->GetXaxis()->GetBinLowEdge(ggHist->GetXaxis()->GetFirst());
        double xup  = ggHist->GetXaxis()->GetBinUpEdge(ggHist->GetXaxis()->GetLast());
        ggHist->Unbin(2);
        ggHist->GetXaxis()->SetRangeUser(xlow,xup);
        doUpdate = true;
        handled  = true;
      }
      break;
    case kKey_l:
    case kKey_z:
      gHist = GrabHist();
      if(gHist && gHist->GetDimension()==1) {
        if(GetLogy()){
          // Show full y range, not restricted to positive values.
          gHist->GetYaxis()->UnZoom();
          SetLogy(0);
        } else {
          // Only show plot from 0 up when in log scale.
          if(gPad->GetUymin()<0) {
            gHist->GetYaxis()->SetRangeUser(0,gPad->GetUymax());
          }
          SetLogy(1);
        }
      } else if(gHist && gHist->GetDimension()==2) {
        if(GetLogz()){
          // Show full y range, not restricted to positive values.
          gHist->GetZaxis()->UnZoom();
          SetLogz(0);
        } else {
          // Only show plot from 0 up when in log scale.
          if(gHist->GetMinimum()<0) {
            gHist->GetZaxis()->SetRangeUser(0,gHist->GetMaximum());
          }
          SetLogz(1);
        }
      }
      doUpdate = true;
      handled  = true;
      break;
    default:
      break;


  }
  if(gPad && doUpdate) {
    gPad->Modified();
    gPad->Update();
  }
  return handled;
}


TVirtualPad* GCanvas::GetSelectedPad() const {
  TVirtualPad *sPad = TCanvas::GetSelectedPad();
  //printf("gPad = 0x%p\n",gPad);
  //printf("sPad = 0x%p\n",sPad);
  return sPad;
}





/*
void GCanvas::doSelected(TVirtualPad *pad, TObject *obj, Int_t event) {
  printf("--------------\n");
  printf("gPad    = 0x%p\n",gPad);
  printf("pad     = 0x%p\n",pad);
  printf("obj     = 0x%p\n",obj);
  if(obj) 
  printf("name    = %s\n",obj->GetName());
  if(obj->InheritsFrom(TAxis::Class())) 
  printf("parent  = %s\n",((TAxis*)obj)->GetParent()->GetName());
  printf("event   = %i\n",event);
  printf("\n\n");
}
*/

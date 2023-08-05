#ifndef __HISTOMATIC_H__
#define __HISTOMATIC_H__


#include "TGClient.h"
#include "TGFrame.h"
#include "TGTab.h"
#include "TGMenu.h"
#include "TGLayout.h"
#include "TGSplitter.h"
#include "TGStatusBar.h"
#include "TGButton.h"
#include "TGListTree.h"

#include "TFile.h"
#include "TKey.h"
#include "TList.h"

#include<exception>
#include<iostream>

class Histomatic;

extern Histomatic *gHistomatic;

class HistList : public TGCanvas {
  public: 
    HistList(TGWindow *parent=0,UInt_t w=1,UInt_t h=1, UInt_t options=kSunkenFrame|kDoubleBorder,Pixel_t back=GetDefaultFrameBackground());
    ~HistList();

  TGListTree *GetListTree();
  
  TList *GetAllActive(TGListTreeItem *item=0);  
  void ClearActive();

  void InsertObject(TObject *obj,TGListTreeItem *parent=0);
  const TGPicture *GetIcon(TClass *cls);
  void OnDoubleClicked(TGListTreeItem *item, Int_t btn); 

  private:
    TGListTree *fListTree;
    TList      *fActive;


  
  ClassDef(HistList,0)
};

class Histomatic { //: public TGMainFrame {
  //RQ_OBJECT("Histomatic")
  //Histomatic(const Histomatic&) = delete;
  //Histomatic& operator=(const Histomatic&) = delete;

  public:
    Histomatic();
    virtual ~Histomatic();    

    TGListTree *GetListTree() { return fHistList->GetListTree(); }

    void buttonAction();

  protected:
    TGLayoutHints *fLH0, *fLH1, *fLH2;
    
    TGVerticalFrame *fVf;
    TGVerticalFrame *fButtonContainer;
    TGHorizontalFrame *fButtonRow1;
    TGHorizontalFrame *fButtonRow2;

    //TGMenuBar *fMenuBar;
    //TGHorizontalFrame *fTopMenuFrame;               
    //TGHorizontalFrame *fPreMenuFrame; 

    //TGPopupMenu       *fMenuFile;
    //TGPopupMenu       *fMenuHelp;     

    TGTextButton      *fButton1; 
    TGTextButton      *fButton2; 
    TGTextButton      *fButton3; 
    TGTextButton      *fButton4; 
    
    TGTextButton      *fButton5; 
    TGTextButton      *fButton6; 
    TGTextButton      *fButton7; 
    TGTextButton      *fButton8; 

    //TGCanvas          *fListTreeCanvas;
    //TGListTree        *fListTree;
    HistList          *fHistList;

  public:
    enum EHistMessages {
      kThing1,
      kThing2,
      kSendHelp,
      kCloseWindow
    };

    void CreateWindow();
    void CloseWindow();

    void AddRootFile(TFile *file) { fHistList->InsertObject(file); }
    TList *GetAllActive();

  private:
    TGMainFrame *fMainWindow;
    //TGVerticalFrame   *fVf;

  //ClassDefOverride(Histomatic,0)
  ClassDef(Histomatic,0)
};


#endif



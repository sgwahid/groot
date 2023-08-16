#ifndef __GMARKER_H__
#define __GMARKER_H__

#include<TLine.h>
#include<TQObject.h>

class TH1;

class GMarker : public TLine { //, public TQObject {
  public:
    GMarker();
    ~GMarker();  

    void AddTo(TH1 *h, double x, double y=sqrt(-1),Option_t *opt="");
    void Remove();

    //void Update();
    //void doUpdate();


    void Paint(Option_t *opt="") override;

  private:
    TH1 *fHist;
    double fX;
    double fY;  

  ClassDefOverride(GMarker,0)
};


#endif

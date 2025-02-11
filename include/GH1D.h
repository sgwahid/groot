#ifndef __GH1D_H__
#define __GH1D_H__

#include<string>

#include<TH1D.h>
#include<TQObject.h>

class GH1D : public TH1D, public TQObject {
  public:
    GH1D();
    GH1D(std::string name,int nbinsx,const double *xbins);
    GH1D(std::string name,int nbinsx,const float  *xbins);
    GH1D(std::string name,int nbinsx,double xlow, double xhigh);

    GH1D(const char *name,const char *title,int nbinsx,const double *xbins);
    GH1D(const char *name,const char *title,int nbinsx,const float  *xbins);
    GH1D(const char *name,const char *title,int nbinsx,double xlow, double xhigh);

    //GH1D(const GH1D &h1d);
    GH1D(const TH1D &h1d);
    GH1D(const TH1F &h1f);

    GH1D(const TVectorD &v);
    
    virtual ~GH1D(); 

    void Draw(Option_t *opt="") override;
    TH1* DrawCopy(Option_t *opt="", const char *name_postfix="_copy") const override; 
    TH1* DrawNormalized(Option_t *opt="", double norm=1) const override; 

    void Paint(Option_t *opt="") override;

    TH1* Rebin(int ngroup=2,const char *newname="",const double *xbins=nullptr) override;
    void Unbin(int ngroup=-1);

    int GetNbinsOriginal() const { return fOriginalBins; }

  private:
    TH1D *fOriginal;    
    int  fOriginalBins;

  ClassDefOverride(GH1D,100)
};

#endif

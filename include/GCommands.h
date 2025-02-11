#ifndef __GCOMMANDS_H__
#define __GCOMMANDS_H__

class TH1;
class TF1;
class GGaus;
class GPeak;

typedef const char Option_t;

GPeak *PhotoPeakFit(TH1*,double,double,Option_t *opt="");
GGaus *GausFit(TH1*,double,double,Option_t *opt="");

TH1 *GrabHist(int i=0); //return the ith histogram from the current canvas.
TF1 *GrabFit(int i=0); //return the ith fit from the current canvas.
 

#endif

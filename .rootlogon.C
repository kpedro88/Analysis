#define LOCAL_ROOTLOGON

#include "style-CMSTDR.C"

void _rootlogon(){
  setTDRStyle();
  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libDataFormatsFWLite.so");
  gSystem->Load("libDataFormatsPatCandidates.so");
  FWLiteEnabler::enable();
}

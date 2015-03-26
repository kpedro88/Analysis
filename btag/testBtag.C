#include "BtagSF.hh"
void test () {
  
  //  BtagSF* btsf = new BtagSF(time(0));
  BtagSF* btsf = new BtagSF(12345);
  
  double jetpt = 100.0, jeteta = -0.1, jetcsv = 0.587, jetflavor = 5;
  bool isdata = false;
  unsigned int btagsys = 0, mistagsys = 0;   // 0 = central, 1 = down, 2 = up
  bool is2012 = true;
  bool btagged = (jetcsv > 0.244);
  
  cout << "Before modification: Is tagged = "<< btagged << endl;
  
  btagged = btsf->isbtagged(jetpt, jeteta, jetcsv, jetflavor, isdata ,btagsys, mistagsys, is2012);
  
  cout << "After modification: Is tagged = "<< btagged << endl;

}

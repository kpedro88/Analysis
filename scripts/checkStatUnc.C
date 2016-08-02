#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>
#include <TCollection.h>
#include <TKey.h>
#include <TClass.h>

#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

void checkStatUnc(string fname="datacards/RA2bin_signal.root"){
	//open file
	TFile* file = TFile::Open(fname.c_str());
	
	//iterate over file
	TIter next(file->GetListOfKeys());
	TKey *key;
	
	//set output precision
	cout << fixed << setprecision(2);
	
	while ((key = (TKey*)next())) {
		TClass *cl = gROOT->GetClass(key->GetClassName());
		if (!cl->InheritsFrom("TH1")) continue;
		TH1 *h = (TH1*)key->ReadObj();
		
		//find range of stat unc
		double unc_min = 1e10;
		double unc_max = 0;
		for(int b = 1; b < h->GetNbinsX()+1; ++b){
			double unc = h->GetBinError(b)/h->GetBinContent(b);
			if(unc < unc_min) unc_min = unc;
			if(unc > unc_max) unc_max = unc;
		}
		
		//print results
		cout << h->GetName() << ": " << unc_min*100 << "--" << unc_max*100 << "%" << endl;
	}

}
#ifndef ISRCORRECTOR_H
#define ISRCORRECTOR_H

#include "Helper.h"

#include <TH1.h>
#include <TLorentzVector.h>

#include <iostream>

class ISRCorrector {
	public:
		//constructor
		ISRCorrector() : h_njetsisr(NULL), h_weights(NULL) {}
		//destructor
		virtual ~ISRCorrector() {}
		
		//accessors
		void SetWeights(TH1* weights, TH1* all, bool debug=false){
			if(!weights) return;
			//normalize weights using overall NJetsISR spectrum so total number of gen events will stay the same
			h_weights = (TH1*)weights->Clone();
			h_njetsisr = all;
			double A_LO = h_njetsisr->Integral(-1,-1);
			TH1* h_njetsisrW = (TH1*)all->Clone();
			h_njetsisrW->Multiply(h_weights);
			double A_NLO = h_njetsisrW->Integral(-1,-1);
			delete h_njetsisrW;

			double scale = A_NLO > 0 ? A_LO/A_NLO : 1.;
			h_weights->Scale(scale);

			if(debug) std::cout << "ISRCorrector norm: " << A_LO << " / " << A_NLO << " = " << scale << std::endl;
		}
		
		//function
		double GetCorrection(int NJetsISR, bool debug=false){
			double binval = min(double(NJetsISR),h_weights->GetXaxis()->GetBinLowEdge(h_weights->GetNbinsX()));
			int bin = h_weights ? h_weights->GetXaxis()->FindBin(binval) : -1;
			double val = h_weights ? h_weights->GetBinContent(bin) : 1.;

			if(debug) std::cout << "ISRCorrector corr: NJetsISR = " << NJetsISR << ", binval = " << binval << ", bin = " << bin << ", val = " << val << std::endl;

			return val;
		}
		
		//member variables
		TH1 *h_njetsisr, *h_weights;
};

/*USAGE:
//open skim file as skimfile
TH1* h_njetsisr = helper::Get<TH1>(skimfile,"NJetsISR");
ISRCorrector isrcorr;
TFile* isrfile = helper::Open("corrections/ISRWeights.root");
//choose central, up, or down
TH1* h_isr = helper::Get<TH1>(isrfile,"isr_weights_central");
isrcorr.SetWeights(h_isr,h_njetsisr);
//in event loop
double w_isr = isrcorr.GetCorrection(NJetsISR);
*/

#endif

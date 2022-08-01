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
		void SetWeights(TH1* weights, TH1* all){
			if(!weights) return;
			//normalize weights using overall NJetsISR spectrum so total number of gen events will stay the same
			h_weights = (TH1*)weights->Clone();
			h_njetsisr = all;
			double A_LO = h_njetsisr->Integral(-1,-1);
			TH1* h_njetsisrW = (TH1*)all->Clone();
			h_njetsisrW->Multiply(h_weights);
			double A_NLO = h_njetsisrW->Integral(-1,-1);
			h_weights->Scale(A_LO/A_NLO);
		}
		
		//function
		double GetCorrection(int NJetsISR){
			return h_weights ? h_weights->GetBinContent(h_weights->GetXaxis()->FindBin(min(double(NJetsISR),h_weights->GetXaxis()->GetBinLowEdge(h_weights->GetNbinsX())))) : 1.;
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

#ifndef PILEUPACCEPTANCEUNCERTAINTY_H
#define PILEUPACCEPTANCEUNCERTAINTY_H

#include <TH1.h>
#include <TGraphErrors.h>

#include <cmath>
#include <iostream>

class PileupAcceptanceUncertainty {
	public:
		//constructor
		PileupAcceptanceUncertainty() : g_conf(NULL), h_nvtx(NULL), central(0.0), up(0.0), down(0.0) {}
		//destructor
		virtual ~PileupAcceptanceUncertainty() {}
		
		//accessors
		void SetInputs(TGraphErrors* gtmp, TH1* htmp, bool debug=false){
			g_conf = gtmp;
			h_nvtx = htmp;
			
			//check normalization
			double i_nvtx = h_nvtx->Integral(0,h_nvtx->GetNbinsX()+1);
			h_nvtx->Scale(1.0/i_nvtx);
			
			//compute central expectation value and uncertainty variations
			//assuming binning of g_conf and h_nvtx are consistent
			for(int b = 1; b <= h_nvtx->GetNbinsX(); ++b){
				double b_nvtx = h_nvtx->GetBinContent(b);
				double g_pt = g_conf->GetY()[b-1];
				double g_err = g_conf->GetEY()[b-1];
				central += b_nvtx*g_pt;
				up += b_nvtx*(g_pt+g_err);
				down += b_nvtx*(g_pt-g_err);
			}
			
			if(debug) std::cout << "PileupAcceptanceUncertainty: central = " << central << ", up = " << up << ", down = " << down << std::endl;
			
			//normalize
			up /= central;
			down /= central;
			
			if(debug) std::cout << "PileupAcceptanceUncertainty: normalized up = " << up << ", down = " << down << std::endl;
		}
		
		//function
		double GetCorrection(int unc){
			if(unc==-1) return down;
			else if(unc==1) return up;
			else return 1.0;
		}
		
		//member variables
		TGraphErrors *g_conf;
		TH1 *h_nvtx;
		double central, up, down;
};

/*USAGE:
//open skim file as skimfile
TGraphErrors *g_conf = (TGraphErrors*)skimfile->Get("pileupAccBand");
PileupAcceptanceUncertainty puacc;
TFile* nvtxfile = TFile::Open("corrections/NVtx_SLe_03Feb2017_35p9ifb.root","READ");
TH1* h_nvtx = (TH1*)nvtxfile->Get("nvertex_SingleElectron");
puacc.SetInputs(g_conf,h_nvtx);
//in event loop, choose up or down
double w_puacc = puacc.GetCorrection(1);
*/

#endif
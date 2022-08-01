#ifndef Flattener_H
#define Flattener_H

#include <TH1.h>
#include <TLorentzVector.h>

#include <iostream>

class Flattener {
	public:
		//constructor
		Flattener() : h_dist(NULL) {}
		//destructor
		virtual ~Flattener() {}
		
		//accessors
		void SetDist(TH1* dist, TH1* numer=NULL){
			if(!dist) return;
			//if numer is provided, normalize to that instead of flat (default)
			if(numer){
				h_dist = (TH1*)numer->Clone();
				h_dist->Divide(dist);
			}
			else {
				h_dist = (TH1*)dist->Clone();
				//make sure normalized
				h_dist->Scale(1.0/h_dist->Integral(0,h_dist->GetNbinsX()+1));
				//invert weights to flatten
				for(int b = 0; b <= h_dist->GetNbinsX()+1; ++b){
					double content = h_dist->GetBinContent(b);
					double weight = content>0 ? 1.0/content : 0.0;
					h_dist->SetBinContent(b,weight);
				}
			}
			h_dist->SetDirectory(0);
		}
		
		//function
		double GetWeight(double qty){
			if(!h_dist) return 1.0;
			return h_dist ? h_dist->GetBinContent(h_dist->GetXaxis()->FindBin(min(qty,h_dist->GetXaxis()->GetBinLowEdge(h_dist->GetNbinsX())))) : 1.;
		}
		
		//member variables
		TH1 *h_dist;
};

#endif

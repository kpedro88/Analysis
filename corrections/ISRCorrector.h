#ifndef ISRCORRECTOR_H
#define ISRCORRECTOR_H

class ISRCorrector {
	public:
		//constructor
		ISRCorrector() : h_genpt(NULL), h_weights(NULL), mother(0) {}
		//destructor
		virtual ~ISRCorrector() {}
		
		//accessors
		void SetMother(int m) { mother = m; }
		void SetWeights(TH1* weights, TH1* gen){
			if(!weights) return;
			//normalize weights using gen pt spectrum so total number of gen events will stay the same
			h_weights = (TH1*)weights->Clone();
			h_genpt = gen;
			double A_LO = h_genpt->Integral(0,h_genpt->GetNbinsX()+1);
			TH1* h_genptW = (TH1*)gen->Clone();
			h_genptW->Multiply(h_weights);
			double A_NLO = h_genptW->Integral(0,h_genptW->GetNbinsX()+1);
			h_weights->Scale(A_LO/A_NLO);
		}
		
		//function
		double GetCorrection(vector<TLorentzVector>* genParticles, vector<int>* genParticles_PDGid){
			if(mother==0 || h_weights==NULL) return 1.;

			//loop over genparticles
			TLorentzVector vgen;
			vgen.SetPtEtaPhiE(0,0,0,0);
			for(unsigned g = 0; g < genParticles_PDGid->size(); ++g){
				if(abs(genParticles_PDGid->at(g))==mother){
					vgen -= genParticles->at(g);
				}
			}
			double genpt = vgen.Pt();
			
			return h_weights ? h_weights->GetBinContent(h_weights->GetXaxis()->FindBin(min(genpt,h_weights->GetBinLowEdge(h_weights->GetNbinsX()+1)))) : 1.;
		}
		
		//member variables
		TH1 *h_genpt, *h_weights;
		int mother;
};

/*USAGE:
//open skim file as skimfile
TH1* h_genpt = (TH1*)skimfile->Get("GenPt");
ISRCorrector isrcorr;
TFile* isrfile = TFile::Open("corrections/ISRWeights.root","READ");
//choose central, up, or down
TH1* h_isr = (TH1*)isrfile->Get("isr_weights_central");
isrcorr.SetWeights(h_isr,h_genpt);
//PDG ID for gluino
isrcorr.SetMother(1000021);
//in event loop
double w_isr = iscorr.GetCorrection(genParticles,genParticles_PDGid);
*/

#endif
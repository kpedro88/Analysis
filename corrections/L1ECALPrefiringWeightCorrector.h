#ifndef L1ECALPrefiringWeightCorrector_H
#define L1ECALPrefiringWeightCorrector_H

#include "Helper.h"

#include <TFile.h>
#include <TH2.h>
#include <TLorentzVector.h>

#include <string>
#include <vector>
#include <array>
#include <cmath>

using namespace std;

//ported from: https://github.com/lathomas/cmssw/blob/L1Prefiring_9_4_9/L1Prefiring/EventWeightProducer/plugins/L1ECALPrefiringWeightProducer.cc

class L1ECALPrefiringWeightCorrector {
	public:
		//constructor
		L1ECALPrefiringWeightCorrector() : useEMpt(false), systUnc(0.0), h_photon(NULL), h_jet(NULL) {}
		
		void setup(string fname_, string dataEra_, bool useEMpt_, double systUnc_)
		{
			useEMpt = useEMpt_;
			systUnc = systUnc_;
			
			//get 2D histo maps from file
			TFile* file = helper::Open(fname_);
			
			string photonname = "L1prefiring_photonptvseta_"+ dataEra_;
			h_photon = helper::Get<TH2F>(file,photonname);
			h_photon->SetDirectory(0);
			
			string jetname = "L1prefiring_jet";
			jetname += (useEMpt ? "empt" : "pt");
			jetname += "vseta_" + dataEra_;
			h_jet = helper::Get<TH2F>(file,jetname);
			h_jet->SetDirectory(0);
			
			file->Close();
		}
		
		std::array<double,3> getWeights(const vector<TLorentzVector>& photons, const vector<TLorentzVector>& jets, const vector<double>& jets_neutralEmEnergyFraction, const vector<double>& jets_chargedEmEnergyFraction){
			//Probability for the event NOT to prefire, computed with the prefiring maps per object. 
			//Up and down values correspond to the resulting value when shifting up/down all prefiring rates in prefiring maps. 
			std::array<double,3> NonPrefiringProb = {1.,1.,1.}; //0: central, 1: up, 2: down
			
			//Start by applying the prefiring maps to photons in the affected regions.
			vector<TLorentzVector> affectedphotons;
			for(const auto& photon : photons){
				double pt = photon.Pt();
				double eta = photon.Eta();
				if(pt < 2.0 or abs(eta) < 2. or abs(eta) > 3.) continue;
				affectedphotons.push_back(photon);
				const auto& nonprefiringprob_gam = getNonPrefiringRate(eta, pt, h_photon);
				dot(NonPrefiringProb,nonprefiringprob_gam);
			}
			
			//Now apply the prefiring maps to jets in the affected regions. 
			for(unsigned j = 0; j < jets.size(); ++j){
				const auto& jet = jets[j];
				double pt = jet.Pt();
				double eta = jet.Eta();
				if(pt < 2.0 or abs(eta) < 2. or abs(eta) > 3.) continue;
				
				//Loop over photons to remove overlap
				std::array<double,3> NonPrefiringProbOverlap = {1.,1.,1.};
				bool overlap = false;
				for(const auto& photon: affectedphotons){
					double dR = jet.DeltaR(photon);
					if(dR > 0.4) continue;
					overlap = true;
					const auto& nonprefiringprob_gam = getNonPrefiringRate(photon.Eta(), photon.Pt(), h_photon);
					dot(NonPrefiringProbOverlap,nonprefiringprob_gam);
				}
				
				double ptem = pt*(jets_neutralEmEnergyFraction[j] + jets_chargedEmEnergyFraction[j]);
				const auto& nonprefiringprob_jet = getNonPrefiringRate(eta, useEMpt ? ptem : pt, h_jet);
				
				//If there are no overlapping photons, just multiply by the jet non prefiring rate
				if(!overlap) dot(NonPrefiringProb,nonprefiringprob_jet);
				else {
					for(unsigned i = 0; i < NonPrefiringProb.size(); ++i){
						//If overlapping photons have a non prefiring rate larger than the jet, then replace these weights by the jet one
						if(NonPrefiringProbOverlap[i] > nonprefiringprob_jet[i]){
							if(NonPrefiringProbOverlap[i] > 0.) NonPrefiringProb[i] *= nonprefiringprob_jet[i]/NonPrefiringProbOverlap[i];
							else NonPrefiringProb[i] = 0.;
						}
						//If overlapping photons have a non prefiring rate smaller than the jet, don't consider the jet in the event weight
					}
				}
			}
			
			return NonPrefiringProb;
		}
		
		std::array<double,3> getNonPrefiringRate(double eta, double pt, TH2F* h_prefmap){
			if(h_prefmap==0) return {0.,0.,0.};
			
			//Check pt is not above map overflow
			int nbinsy = h_prefmap->GetNbinsY();
			double maxy = h_prefmap->GetYaxis()->GetBinLowEdge(nbinsy+1);
			if(pt>=maxy) pt = maxy-0.01;
			int thebin = h_prefmap->FindBin(eta,pt);
			
			double prefrate = h_prefmap->GetBinContent(thebin);
			double preferr = h_prefmap->GetBinError(thebin);
			double prefrate_up = min(max(prefrate + preferr, (1. + systUnc)*prefrate),1.);
			double prefrate_dn = max(min(prefrate - preferr, (1. - systUnc)*prefrate),0.);
			
			return {1.-prefrate, 1.-prefrate_up, 1.-prefrate_dn};
		}
		
		void dot(array<double,3>& a1, const array<double,3>& a2){
			for(unsigned i = 0; i < a1.size(); ++i){
				a1[i] *= a2[i];
			}
		}
		
	private:
		//members
		bool useEMpt;
		double systUnc;
		TH2F *h_photon, *h_jet;
};

#endif

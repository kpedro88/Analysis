#ifndef BTAGCORRECTOR_H
#define BTAGCORRECTOR_H

//custom headers
#include "BTagCalibrationStandalone.cc"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>

//STL headers
#include <string>
#include <vector>

using namespace std;

class BTagCorrector {
	public:
		//constructor
		BTagCorrector() : debug(false), fastsim(false), 
			btagSFunc("central"), mistagSFunc("central"), btagCFunc("central"), ctagCFunc("central"), mistagCFunc("central"), 
			h_eff_b(NULL), h_eff_c(NULL), h_eff_udsg(NULL)
		{}
		//destructor
		virtual ~BTagCorrector() {}
		
		//accessors
		void SetDebug(bool d) { debug = d; }
		void SetFastSim(bool f) { fastsim = f; }
		void SetEffs(TFile* file){
			h_eff_b = (TH2F*)file->Get("h_eff_b");
			h_eff_c = (TH2F*)file->Get("h_eff_c");
			h_eff_udsg = (TH2F*)file->Get("h_eff_udsg");
			
			//if these are not found, check for their components
			if(!h_eff_b){
				TH2F* n_eff_b = (TH2F*)file->Get("n_eff_b");
				TH2F* d_eff_b = (TH2F*)file->Get("d_eff_b");
				h_eff_b = (TH2F*)n_eff_b->Clone("h_eff_b");
				h_eff_b->Divide(d_eff_b);
			}
			if(!h_eff_c){
				TH2F* n_eff_c = (TH2F*)file->Get("n_eff_c");
				TH2F* d_eff_c = (TH2F*)file->Get("d_eff_c");
				h_eff_c = (TH2F*)n_eff_c->Clone("h_eff_c");
				h_eff_c->Divide(d_eff_c);
			}
			if(!h_eff_udsg){
				TH2F* n_eff_udsg = (TH2F*)file->Get("n_eff_udsg");
				TH2F* d_eff_udsg = (TH2F*)file->Get("d_eff_udsg");
				h_eff_udsg = (TH2F*)n_eff_udsg->Clone("h_eff_udsg");
				h_eff_udsg->Divide(d_eff_udsg);
			}
		}
		void SetCalib(string cfile){
			//initialize btag helper classes
			calib = BTagCalibrationS("",cfile);
			reader = BTagCalibrationReaderS(BTagEntryS::OP_MEDIUM, "central", {"up","down"});
			reader.load(calib, BTagEntryS::FLAV_B, "comb"); reader.load(calib, BTagEntryS::FLAV_C, "comb");  reader.load(calib, BTagEntryS::FLAV_UDSG, "incl");
		}
		void SetCalibFastSim(string cfile){
			//read CFs
			calibFast = BTagCalibrationS("",cfile);
			readerFast = BTagCalibrationReaderS(BTagEntryS::OP_MEDIUM, "central", {"up","down"});
			readerFast.load(calibFast, BTagEntryS::FLAV_B, "fastsim"); readerFast.load(calibFast, BTagEntryS::FLAV_C, "fastsim");  readerFast.load(calibFast, BTagEntryS::FLAV_UDSG, "fastsim");
		}
		void SetBtagSFunc(int u) { btagSFunc = u==0 ? "central" : (u==1 ? "up" : "down"); }
		void SetCtagSFunc(int u) { btagSFunc = u==0 ? "central" : (u==1 ? "up" : "down"); } //ctag and btag are correlated
		void SetMistagSFunc(int u) { mistagSFunc = u==0 ? "central" : (u==1 ? "up" : "down"); }
		void SetBtagCFunc(int u) { btagCFunc = u==0 ? "central" : (u==1 ? "up" : "down"); }
		void SetCtagCFunc(int u) { ctagCFunc = u==0 ? "central" : (u==1 ? "up" : "down"); }
		void SetMistagCFunc(int u) { mistagCFunc = u==0 ? "central" : (u==1 ? "up" : "down"); }
		
		//method 1b
		vector<double> GetCorrections(vector<TLorentzVector>* Jets, vector<int>* Jets_flavor, vector<bool>* Jets_HTMask){
			//reset probabilities
			vector<double> prob(4,0.0);
			prob[0] = 1.0;
			
			//first loop over jets
			vector<vector<double> > sfEffLists = vector<vector<double> >(Jets->size(),vector<double>());
			for(unsigned ja = 0; ja < Jets->size(); ++ja){
				//HT jet cuts
				if(!Jets_HTMask->at(ja)) continue;
				
				//get sf and eff values (checks if already calculated)
				InitSFEff(Jets->at(ja).Pt(), Jets->at(ja).Eta(), Jets_flavor->at(ja), sfEffLists[ja]);
				double eps_a = sfEffLists[ja][0]*sfEffLists[ja][1]*sfEffLists[ja][2];
				
				//jet index, pt, eta, flavor, eff, sf, cf
				if(debug) cout << "Jet " << ja << ": " << Jets->at(ja).Pt() << ", " << fabs(Jets->at(ja).Eta()) << ", " << abs(Jets_flavor->at(ja)) 
								<< ", " << sfEffLists[ja][0] << ", " << sfEffLists[ja][1] << ", " << sfEffLists[ja][2] << endl;
				
				//calculate prob(0 b-tags)
				prob[0] *= (1-eps_a);
				
				//sub-probabilities for following calculations
				double subprob1 = 1.0;
				double subprob2 = 0.0;
				
				//second loop over jets
				for(unsigned jb = 0; jb < Jets->size(); ++jb){
					//skip the same jet
					if(jb==ja) continue;
					
					//HT jet cuts
					if(!Jets_HTMask->at(jb)) continue;
					
					//get sf and eff values (checks if already calculated)
					InitSFEff(Jets->at(jb).Pt(), Jets->at(jb).Eta(), Jets_flavor->at(jb), sfEffLists[jb]);
					double eps_b = sfEffLists[jb][0]*sfEffLists[jb][1]*sfEffLists[jb][2];
					
					//jet index, pt, eta, flavor, eff, sf, cf
					if(debug) cout << "\tJet " << jb << ": " << Jets->at(jb).Pt() << ", " << fabs(Jets->at(jb).Eta()) << ", " << abs(Jets_flavor->at(jb)) 
									<< ", " << sfEffLists[jb][0] << ", " << sfEffLists[jb][1] << ", " << sfEffLists[jb][2] << endl;
					
					//calculate prob(1 b-tag)
					subprob1 *= (1-eps_b);
					
					//sub-sub-probability for following calculations
					double subsubprob2 = 1.0;
					
					//third loop over jets (only for jb>ja)
					if(jb<ja) continue;
					for(unsigned jc = 0; jc < Jets->size(); ++jc){
						//skip the same jet
						if(jc==jb || jc==ja) continue;
						
						//HT jet cuts
						if(!Jets_HTMask->at(jc)) continue;
						
						//get sf and eff values (checks if already calculated)
						InitSFEff(Jets->at(jc).Pt(), Jets->at(jc).Eta(), Jets_flavor->at(jc), sfEffLists[jc]);
						double eps_c = sfEffLists[jc][0]*sfEffLists[jc][1]*sfEffLists[jc][2];
						
						//jet index, pt, eta, flavor, eff, sf, cf
						if(debug) cout << "\t\tJet " << jc << ": " << Jets->at(jc).Pt() << ", " << fabs(Jets->at(jc).Eta()) << ", " << abs(Jets_flavor->at(jc)) 
										<< ", " << sfEffLists[jc][0] << ", " << sfEffLists[jc][1] << ", " << sfEffLists[jc][2] << endl;
						
						//calculate prob(2 b-tags)
						subsubprob2 *= (1-eps_c);
					}
					
					//add up sub-sub-prob
					subprob2 += eps_b*subsubprob2;
				}
				
				//add up sub-probs
				prob[1] += eps_a*subprob1;
				prob[2] += eps_a*subprob2;
			}
			
			//conserve probability
			prob[3] = 1 - prob[0] - prob[1] - prob[2];
			if(debug) cout << prob[0] << ", " << prob[1] << ", " << prob[2] << ", " << prob[3] << endl;
			
			return prob;
		}
		
		//method 1a
		double GetSimpleCorrection(vector<TLorentzVector>* Jets, vector<int>* Jets_flavor, vector<bool>* Jets_HTMask, vector<double>* Jets_bDiscriminatorCSV){
			//result
			double c_numer = 1.0; //data
			double c_denom = 1.0; //mc
			
			//loop over jets
			vector<vector<double> > sfEffLists = vector<vector<double> >(Jets->size(),vector<double>());
			for(unsigned ja = 0; ja < Jets->size(); ++ja){
				//HT jet cuts
				if(!Jets_HTMask->at(ja)) continue;
				
				//get sf and eff values (checks if already calculated)
				InitSFEff(Jets->at(ja).Pt(), Jets->at(ja).Eta(), Jets_flavor->at(ja), sfEffLists[ja]);
				double eff_a = sfEffLists[ja][0]*sfEffLists[ja][2]; //eff * CF
				double sf_a = sfEffLists[ja][1];
				
				//jet index, pt, eta, flavor, csv, eff, sf, cf
				if(debug) cout << "Jet " << ja << ": " << Jets->at(ja).Pt() << ", " << fabs(Jets->at(ja).Eta()) << ", " << abs(Jets_flavor->at(ja))  << ", " << Jets_bDiscriminatorCSV->at(ja)
								<< ", " << sfEffLists[ja][0] << ", " << sfEffLists[ja][1] << ", " << sfEffLists[ja][2] << endl;
							
				if(Jets_bDiscriminatorCSV->at(ja) > 0.8484){
					c_numer *= eff_a;
					c_denom *= eff_a*sf_a;
				}
				else {
					c_numer *= 1-eff_a;
					c_denom *= 1-eff_a*sf_a;
				}
			}
			
			return c_numer/c_denom;
		}
		
		//helper function
		void InitSFEff(double pt, double eta, int flav, vector<double>& sfEffList){
			//avoid rerunning this
			if(sfEffList.size()>0) return;
			
			//use abs(eta) for now
			eta = fabs(eta);
			//use abs(flav) always
			flav = abs(flav);
			
			sfEffList = vector<double>(3,1.0); //eff, sf (central, up, or down), cf (central, up, or down)
			
			if(flav==5){ //b-tag
				sfEffList[0] = h_eff_b->GetBinContent(h_eff_b->FindBin(pt,eta));
				sfEffList[1] = reader.eval_auto_bounds(btagSFunc,BTagEntryS::FLAV_B,eta,pt);
				if(fastsim){
					sfEffList[2] = readerFast.eval_auto_bounds(btagCFunc,BTagEntryS::FLAV_B,eta,pt);
				}
			}
			else if(flav==4){ //charm mistag
				sfEffList[0] = h_eff_c->GetBinContent(h_eff_c->FindBin(pt,eta));
				sfEffList[1] = reader.eval_auto_bounds(btagSFunc,BTagEntryS::FLAV_C,eta,pt);
				if(fastsim){
					sfEffList[2] = readerFast.eval_auto_bounds(ctagCFunc,BTagEntryS::FLAV_C,eta,pt);
				}
			}
			else if(flav<4 || flav==21){ //udsg mistag
				sfEffList[0] = h_eff_udsg->GetBinContent(h_eff_udsg->FindBin(pt,eta));
				sfEffList[1] = reader.eval_auto_bounds(mistagSFunc,BTagEntryS::FLAV_UDSG,eta,pt);
				if(fastsim){
					sfEffList[2] = readerFast.eval_auto_bounds(mistagCFunc,BTagEntryS::FLAV_UDSG,eta,pt);
				}
			}
		}
		
		//member variables
		bool debug, fastsim;
		string btagSFunc, mistagSFunc;
		string btagCFunc, ctagCFunc, mistagCFunc;
		BTagCalibrationS calib, calibFast;
		BTagCalibrationReaderS reader;
		BTagCalibrationReaderS readerFast;
		TH2F *h_eff_b, *h_eff_c, *h_eff_udsg;
};

/*
USAGE:
//open skim file
BTagCorrector btagcorr;
btagcorr->SetEffs(file);
btagcorr->SetCalib("btag/CSVv2_ichep.csv");
//if fastsim
btagcorr->SetFastSim(true);
btagcorr->SetCalibFastSim("btag/CSV_13TEV_Combined_14_7_2016.csv");
//inside event loop
vector<double> prob = btagcorr->GetCorrections(Jets,Jets_hadronFlavor,Jets_HTMask);
//put event in each btag bin, weighted by prob[0], prob[1], prob[2], prob[3] for nb = 0, 1, 2, 3+
//instead, if cutting on nb, use method 1a in event loop to get event weight
double corr = btagcorr->GetSimpleCorrection(Jets,Jets_hadronFlavor,Jets_HTMask,Jets_bDiscriminatorCSV);
*/

#endif
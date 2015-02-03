#ifndef KBUILDER_H
#define KBUILDER_H
#define TreeClass_cxx

//custom headers
#include "KMap.h"
#include "KHelper.h"
#include "KBase.h"
#include "TreeClass.h"

//custom headers for weighting
#include "../btag/GetBtagScale.C"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TLorentzVector.h>

//STL headers
#include <string>
#include <iostream>
#include <map>

using namespace std;

void TreeClass::Loop() {}

//---------------------------------------------------------------
//histo builder class - loops over tree to fill histos for a base
class KBuilder : public TreeClass {
	public:
		//constructors
		KBuilder() : TreeClass(), MyBase(0), localOpt(0), globalOpt(0), stmp(""), htmp(0) {
			//must always have local & global option maps
			if(localOpt==0) localOpt = new OptionMap();
			if(globalOpt==0) globalOpt = new OptionMap();
		}
		KBuilder(KBase* MyBase_, TTree* tree_) : TreeClass(tree_), MyBase(MyBase_), localOpt(MyBase->GetLocalOpt()), globalOpt(MyBase->GetGlobalOpt()), stmp(""), htmp(0) {
			//must always have local & global option maps
			if(localOpt==0) localOpt = new OptionMap();
			if(globalOpt==0) globalOpt = new OptionMap();
		}
		//destructor
		virtual ~KBuilder() {}
		
		
		bool CheckDoubleCount(){
			pair<int,int> tmp(run,event);
			map<pair<int,int>,int>::iterator cmit = countmap.find(tmp);
			if(cmit!=countmap.end()) return true; //returns true if it finds a double count
			else {
				countmap[tmp] = 1;
				return false;
			}
		}

		//functions for histo creation
		virtual bool Cut() { //this implements the set of cuts common between data and MC
			bool goodEvent = true;
			
			//check for double counting
			string normtype = "";
			localOpt->Get("normtype",normtype);
			if ((normtype!="chargino" && normtype!="rpv") && CheckDoubleCount()) return false;
			
			int filter = -1;
			if(globalOpt->Get<int>("filter",filter)){
				//check appropriate filter conditions
				if(filter==0){ 
					if(globalOpt->Get("antiisosel",false)){ //anti-iso selection
						TLorentzVector v_mu;
						v_mu.SetPtEtaPhiM(MuonPt,MuonEta,MuonPhi,0.1056583715);
						bool anyGoodTau = false;
						for(int t = 0; t < HPSTauPt->size(); t++){
							bool goodTau = true;
							
							goodTau &= (PFJetMultiplicity>=2) || (globalOpt->Get("prepresel",false));
							
							TLorentzVector v_tau;
							v_tau.SetPtEtaPhiM(HPSTauPt->at(t),HPSTauEta->at(t),HPSTauPhi->at(t),1.77682);
							
							//store main jet & bjet observables
							//bjet is always index 1, other jet is always index 0
							if(goodTau && IndexTauJet != IndexMuonJet){
								TLorentzVector v_jet[2];
								TLorentzVector v_mujet[2];
								TLorentzVector v_taujet[2];
								for(int j = 0; j < 2; j++){
									//make jet vector and lepton+jet combination vectors
									v_jet[j].SetPtEtaPhiE(PFMainJetPt[j],PFMainJetEta[j],PFMainJetPhi[j],PFMainJetEnergy[j]);
									v_mujet[j] = v_mu + v_jet[j];
									v_taujet[j] = v_tau + v_jet[j];
								}

								//minimize delta(M_tj - M_mj) (pairing of leptons and jets)
								double pair_t0m1 = fabs(v_taujet[0].M()-v_mujet[1].M());
								double pair_t1m0 = fabs(v_taujet[1].M()-v_mujet[0].M());
								if(pair_t0m1 < pair_t1m0){
									FakeTauIndexTauJet.push_back(0);
									FakeTauIndexMuonJet.push_back(1);
									FakeTauMassTauJet.push_back(v_taujet[0].M());
								}
								else{
									FakeTauIndexTauJet.push_back(1);
									FakeTauIndexMuonJet.push_back(0);
									FakeTauMassTauJet.push_back(v_taujet[1].M());
								}
							}
							else{
								FakeTauIndexTauJet.push_back(-1);
								FakeTauIndexMuonJet.push_back(-1);
								FakeTauMassTauJet.push_back(0);
							}
							
							if(globalOpt->Get("finaltaupt",false)) goodTau &= HPSTauPt->at(t) > 50;
							
							if(globalOpt->Get("mainsel",false)) {
								goodTau &= (FakeTauIndexTauJet[t] != FakeTauIndexMuonJet[t]); //check that good jets were found and paired with leptons
								
								//options to do either part of final selection separately
								
								//if(globalOpt->Get("finalmasscut",false)) goodTau &= MassTauJet > 250;
								double mtb = 250;
								globalOpt->Get("finalmasscutval",mtb);
								if(globalOpt->Get("finalmasscut",false)) goodTau &= FakeTauMassTauJet[t] > mtb;
								else if(globalOpt->Get("finalmasscutinv",false)) goodTau &= FakeTauMassTauJet[t] < mtb;
								
								//final selection: M(tau,b) > 250 GeV and pT(tau) > 50 GeV
								//also final selection for lqd333
								if(globalOpt->Get("finalsel",false)) {
									goodTau &= (FakeTauMassTauJet[t] > 250) && (HPSTauPt->at(t) > 50);
								}
							}
							//RPV selection for lqd321
							else if(globalOpt->Get("lqd321sel",false)){ //Nbjet>=1, Njet>=5 (+ tau pT cut)
								//goodTau &= HPSTauPt->at(t) > 40;
								goodTau &= HPSTauPt->at(t) > 50;
							}
							
							//store decisions
							FakeTauDecision.push_back(goodTau);
							anyGoodTau |= goodTau;
							
							//gen-level checks for MC
							string normtype = "";
							localOpt->Get("normtype",normtype);
							if(normtype=="faketau" && globalOpt->Get("dofaketau",false) && HPSTauFakeJet->at(t) != true) goodTau &= false;
							else if(normtype=="nofaketau" && globalOpt->Get("dofaketau",false) && HPSTauFakeJet->at(t) != false) goodTau &= false;
							FakeTauGenDecision.push_back(goodTau);
						}
						goodEvent &= anyGoodTau;
						//RPV selection for lqd321
						if(globalOpt->Get("lqd321sel",false)){ //Nbjet>=1, Njet>=5 (+ tau pT cut)
							for(int j = 0; j < PFJetPt->size(); j++){
								if(BJetIndices.size()<1 && PFJetCSVBTag->at(j)>0.244) BJetIndices.push_back(j);
								else JetIndices.push_back(j);
							}
							goodEvent &= (BJetIndices.size()>=1) && (JetIndices.size()>=4);
						}
					}
					else {	
						//main sample
						goodEvent &= (MuonPt > 29) //1 lepton with pT > 29 GeV
						&& (HPSTauPt->at(0) > 29) //1 hadronic tau with pT > 29 GeV
						&& (globalOpt->Get("prepresel",false) || (PFJetMultiplicity >= 2 && PFJetPt->at(0) > 30 && PFJetPt->at(1) > 30)); //2 jets with pT > 30 GeV
						
						if(globalOpt->Get("finaltaupt",false)) goodEvent &= HPSTauPt->at(0) > 50;
						
						int dilepmass = -1;
						if(globalOpt->Get("dilepmass",dilepmass)){
							if(dilepmass==0) goodEvent &= (MassMuonTau < 60);
							else if(dilepmass==1) goodEvent &= ((MassMuonTau > 60) && (MassMuonTau < 120));
							else if(dilepmass==2) goodEvent &= (MassMuonTau > 120);
						}
						
						//main selection: 2 good jets, 1 b-jet
						if(globalOpt->Get("mainsel",false)) {
							goodEvent &= (IndexTauJet != IndexMuonJet); //check that good jets were found and paired with leptons
							
							//options to do either part of final selection separately
							
							//if(globalOpt->Get("finalmasscut",false)) goodEvent &= MassTauJet > 250;
							double mtb = 250;
							globalOpt->Get("finalmasscutval",mtb);
							if(globalOpt->Get("finalmasscut",false)) goodEvent &= MassTauJet > mtb;
							else if(globalOpt->Get("finalmasscutinv",false)) goodEvent &= MassTauJet < mtb;
							
							//final selection: M(tau,b) > 250 GeV and pT(tau) > 50 GeV
							//also final selection for lqd333
							if(globalOpt->Get("finalsel",false)) {
								goodEvent &= (MassTauJet > 250) && (HPSTauPt->at(0) > 50);
								ST_rpv = MuonPt + HPSTauPt->at(0) + PFMainJetPt[0] + PFMainJetPt[1]; //for lqd333sel1
							}
						}
						//RPV selection for lqd321
						else if(globalOpt->Get("lqd321sel",false)){ //Nbjet>=1, Njet>=5 (+ tau pT cut)
							//goodEvent &= HPSTauPt->at(0) > 40;
							goodEvent &= HPSTauPt->at(0) > 50;
							for(int j = 0; j < PFJetPt->size(); j++){
								if(BJetIndices.size()<1 && PFJetCSVBTag->at(j)>0.244) BJetIndices.push_back(j);
								else JetIndices.push_back(j);
							}
							goodEvent &= (BJetIndices.size()>=1) && (JetIndices.size()>=4);
						}
						//RPV selections - testing various versions for Keti
						//define ST for the event in here - member variable for ST_rpv
						else if(globalOpt->Get("lqd321sel1",false)){ //Nbjet>=1, Njet>=5
							goodEvent &= HPSTauPt->at(0) > 50;
							for(int j = 0; j < PFJetPt->size(); j++){
								if(BJetIndices.size()<1 && PFJetCSVBTag->at(j)>0.244) BJetIndices.push_back(j);
								else JetIndices.push_back(j);
							}
							goodEvent &= (BJetIndices.size()>=1) && (JetIndices.size()>=4);
							//calculate ST
							if(goodEvent) ST_rpv = MuonPt + HPSTauPt->at(0) + PFJetPt->at(BJetIndices[0]) + PFJetPt->at(JetIndices[0]) + PFJetPt->at(JetIndices[1]) + PFJetPt->at(JetIndices[2]) + PFJetPt->at(JetIndices[3]);
						}
						else if(globalOpt->Get("lqd321sel12",false)){ //Nbjet>=1, Njet>=5, pT>40
							goodEvent &= HPSTauPt->at(0) > 40;
							for(int j = 0; j < PFJetPt->size(); j++){
								if(BJetIndices.size()<1 && PFJetCSVBTag->at(j)>0.244) BJetIndices.push_back(j);
								else JetIndices.push_back(j);
							}
							goodEvent &= (BJetIndices.size()>=1) && (JetIndices.size()>=4);
							//calculate ST
							if(goodEvent) ST_rpv = MuonPt + HPSTauPt->at(0) + PFJetPt->at(BJetIndices[0]) + PFJetPt->at(JetIndices[0]) + PFJetPt->at(JetIndices[1]) + PFJetPt->at(JetIndices[2]) + PFJetPt->at(JetIndices[3]);
						}
						else if(globalOpt->Get("lqd321sel13",false)){ //Nbjet>=1, Njet>=6, pT>40
							goodEvent &= HPSTauPt->at(0) > 40;
							for(int j = 0; j < PFJetPt->size(); j++){
								if(BJetIndices.size()<1 && PFJetCSVBTag->at(j)>0.244) BJetIndices.push_back(j);
								else JetIndices.push_back(j);
							}
							goodEvent &= (BJetIndices.size()>=1) && (JetIndices.size()>=5);
							//calculate ST
							if(goodEvent) ST_rpv = MuonPt + HPSTauPt->at(0) + PFJetPt->at(BJetIndices[0]) + PFJetPt->at(JetIndices[0]) + PFJetPt->at(JetIndices[1]) + PFJetPt->at(JetIndices[2]) + PFJetPt->at(JetIndices[3]) + PFJetPt->at(JetIndices[4]);
						}
						else if(globalOpt->Get("lqd321sel14",false)){ //Nbjet>=1, Njet>=6, pT>30
							goodEvent &= HPSTauPt->at(0) > 30;
							for(int j = 0; j < PFJetPt->size(); j++){
								if(BJetIndices.size()<1 && PFJetCSVBTag->at(j)>0.244) BJetIndices.push_back(j);
								else JetIndices.push_back(j);
							}
							goodEvent &= (BJetIndices.size()>=1) && (JetIndices.size()>=5);
							//calculate ST
							if(goodEvent) ST_rpv = MuonPt + HPSTauPt->at(0) + PFJetPt->at(BJetIndices[0]) + PFJetPt->at(JetIndices[0]) + PFJetPt->at(JetIndices[1]) + PFJetPt->at(JetIndices[2]) + PFJetPt->at(JetIndices[3]) + PFJetPt->at(JetIndices[4]);
						}
						else if(globalOpt->Get("lqd321sel15",false)){ //Nbjet>=2, Njet>=5, pT>40
							goodEvent &= HPSTauPt->at(0) > 40;
							for(int j = 0; j < PFJetPt->size(); j++){
								if(BJetIndices.size()<2 && PFJetCSVBTag->at(j)>0.244) BJetIndices.push_back(j);
								else JetIndices.push_back(j);
							}
							goodEvent &= (BJetIndices.size()>=2) && (JetIndices.size()>=3);
							//calculate ST
							if(goodEvent) ST_rpv = MuonPt + HPSTauPt->at(0) + PFJetPt->at(BJetIndices[0]) + PFJetPt->at(BJetIndices[1]) + PFJetPt->at(JetIndices[0]) + PFJetPt->at(JetIndices[1]) + PFJetPt->at(JetIndices[2]);
						}
						else if(globalOpt->Get("lqd321sel16",false)){ //Nbjet>=2, Njet>=5, pT>30
							goodEvent &= HPSTauPt->at(0) > 30;
							for(int j = 0; j < PFJetPt->size(); j++){
								if(BJetIndices.size()<2 && PFJetCSVBTag->at(j)>0.244) BJetIndices.push_back(j);
								else JetIndices.push_back(j);
							}
							goodEvent &= (BJetIndices.size()>=2) && (JetIndices.size()>=3);
							//calculate ST
							if(goodEvent) ST_rpv = MuonPt + HPSTauPt->at(0) + PFJetPt->at(BJetIndices[0]) + PFJetPt->at(BJetIndices[1]) + PFJetPt->at(JetIndices[0]) + PFJetPt->at(JetIndices[1]) + PFJetPt->at(JetIndices[2]);
						}
						else if(globalOpt->Get("lqd321sel2",false)){ //Nbjet>=1, Njet>=4
							goodEvent &= HPSTauPt->at(0) > 50;
							for(int j = 0; j < PFJetPt->size(); j++){
								if(BJetIndices.size()<1 && PFJetCSVBTag->at(j)>0.244) BJetIndices.push_back(j);
								else JetIndices.push_back(j);
							}
							goodEvent &= (BJetIndices.size()>=1) && (JetIndices.size()>=3);
							//calculate ST
							if(goodEvent) ST_rpv = MuonPt + HPSTauPt->at(0) + PFJetPt->at(BJetIndices[0]) + PFJetPt->at(JetIndices[0]) + PFJetPt->at(JetIndices[1]) + PFJetPt->at(JetIndices[2]);
						}
						else if(globalOpt->Get("lqd321sel3",false)){ //Nbjet>=1, Njet>=4, pT(jet 3) > 50, pT(jet 4) > 40
							goodEvent &= HPSTauPt->at(0) > 50;
							for(int j = 0; j < PFJetPt->size(); j++){
								if(BJetIndices.size()<1 && PFJetCSVBTag->at(j)>0.244) BJetIndices.push_back(j);
								else JetIndices.push_back(j);
							}
							//BJet is always considered as jet #1 even if it is softer
							goodEvent &= (BJetIndices.size()>=1) && (JetIndices.size()>=3) && (PFJetPt->at(JetIndices[1])>50) && (PFJetPt->at(JetIndices[2])<40);
							//calculate ST
							if(goodEvent) ST_rpv = MuonPt + HPSTauPt->at(0) + PFJetPt->at(BJetIndices[0]) + PFJetPt->at(JetIndices[0]) + PFJetPt->at(JetIndices[1]) + PFJetPt->at(JetIndices[2]);
						}
						//note: lqd333sel1 -> finalsel
						/*else if(globalOpt->Get("lqd333sel2",false)){ //Nbjet>=2, Njet>=3
							goodEvent &= HPSTauPt->at(0) > 50;
							for(int j = 0; j < PFJetPt->size(); j++){
								if(BJetIndices.size()<2 && PFJetCSVBTag->at(j)>0.244) BJetIndices.push_back(j);
								else JetIndices.push_back(j);
							}
							goodEvent &= (BJetIndices.size()>=2) && (JetIndices.size()>=1);
							//calculate ST
							if(goodEvent) ST_rpv = MuonPt + HPSTauPt->at(0) + PFJetPt->at(BJetIndices[0]) + PFJetPt->at(BJetIndices[1]) + PFJetPt->at(JetIndices[0]);
						}*/
						else if(globalOpt->Get("lqd333sel2",false)){//3D optimization of sel2, w/ defaults: //Nbjet>=2, Njet>=3, pt>50
							double pt_val = 50;
							globalOpt->Get("pt_val",pt_val);
							goodEvent &= HPSTauPt->at(0) > pt_val;
							
							int nj_val = 3;
							globalOpt->Get("nj_val",nj_val);
							int nb_val = 2;
							globalOpt->Get("nb_val",nb_val);
							for(int j = 0; j < PFJetPt->size(); j++){
								if(BJetIndices.size()<nb_val && PFJetCSVBTag->at(j)>0.244) BJetIndices.push_back(j);
								else JetIndices.push_back(j);
							}
							goodEvent &= (BJetIndices.size()>=nb_val) && (JetIndices.size()>=(nj_val-nb_val));
							
							//calculate ST
							if(goodEvent) {
								ST_rpv = MuonPt + HPSTauPt->at(0);
								for(int j = 0; j < nb_val; j++){
									ST_rpv += PFJetPt->at(BJetIndices[j]);
								}
								for(int j = 0; j < nj_val-nb_val; j++){
									ST_rpv += PFJetPt->at(JetIndices[j]);
								}
							}
						}
					}
				}
				else if(filter==1){ //w+jets control region
					double MT=sqrt(2*MuonPt*PFMETPatType1*(1-cos(KMath::DeltaPhi(MuonPhi,PFMETPhiPatType1))));
					goodEvent &= (MuonPt > 29) //1 lepton with pT > 29 GeV
					&& (HPSTauPt->at(0) > 40) //1 hadronic tau with pT > 40 GeV
					&& (!(globalOpt->Get("jetveto",true)) || (PFJetMultiplicity == 0 || PFJetPt->at(0) < 30)) //0 jets with pT > 30 GeV - jet veto for fakes
					&& (!(globalOpt->Get("btagveto",false)) || IndexTauJet == IndexMuonJet) //if those indices are equal, there was no good jet that passed b-tagging
					&& MT > 50; //cut on transverse mass to reduce ttbar
					int nprong;
					if(globalOpt->Get<int>("tauprong",nprong)) goodEvent &= (HPSTauDecayMode->at(0)/5+1==nprong);
					string etaregion;
					if(globalOpt->Get<string>("etaregion",etaregion)){
						if(etaregion=="barrel") goodEvent &= (abs(HPSTauEta->at(0))<1.4);
						else if(etaregion=="endcap") goodEvent &= (abs(HPSTauEta->at(0))>=1.4);
					}
				}
				else if(filter==2){ //control region 2
					goodEvent &= (MuonPt > 25) //1 lepton with pT > 25 GeV
					&& (HPSTauPt->at(0) > 25) //1 hadronic tau with pT > 25 GeV
					&& (PFJetMultiplicity >=1 && PFJetPt->at(0) > 25); //1 jet with pT > 25 GeV
				}
				else if(filter==3){ //z->mumu control region
					if(globalOpt->Get("finaltaupt",false)) goodEvent &= HPSTauPt->at(0) > 50;
					goodEvent &= (MassMuonMuon > 50); //dimuon mass cut
					if(globalOpt->Get("zpeak",false)) goodEvent &= (MassMuonMuon > 70) && (MassMuonMuon < 110); //look only at region near Z mass peak (for normalization)
					int njets = 0;
					if(globalOpt->Get("njets",njets)) {
						if(globalOpt->Get("njets_excl",false)) goodEvent &= (PFJetMultiplicity == njets);
						else goodEvent &= (PFJetMultiplicity >= njets);
					}
					int nbjets = 0;
					if(globalOpt->Get("nbjets",nbjets)){
						int nb = 0;
						for(int j = 0; j < PFJetPt->size(); j++){
							if(PFJetCSVBTag->at(j)>0.244) nb++;
						}
						if(globalOpt->Get("nbjets_excl",false)) goodEvent &= (nbjets == nb);
						else goodEvent &= (nbjets >= nb);
					}
				}
				else if(filter==4){ //ttbar control region
					if(globalOpt->Get("finaltaupt",false)) goodEvent &= HPSTauPt->at(0) > 50;
					int njets = 0;
					if(globalOpt->Get("njets",njets)){
						if(globalOpt->Get("njets_excl",false)) goodEvent &= (PFJetMultiplicity == njets);
						else goodEvent &= (PFJetMultiplicity >= njets);
					}
					int nbjets = 0;
					if(globalOpt->Get("nbjets",nbjets)){
						int nb = 0;
						for(int j = 0; j < PFJetPt->size(); j++){
							if(PFJetCSVBTag->at(j)>0.244) nb++;
						}
						if(globalOpt->Get("nbjets_excl",false)) goodEvent &= (nbjets == nb);
						else goodEvent &= (nbjets >= nb);
					}
				}
				else {} //no filter requirement, do nothing
			}
		
			return goodEvent;
		}
		virtual double Weight() { return 1.; }
		virtual void FakeTauEstimation(double w){
			TGraphAsymmErrors* tfr = 0;
			if(globalOpt->Get("use_mc_tfr",false)) globalOpt->Get("tfr_mc",tfr);
			else globalOpt->Get("tfr_data",tfr);
			
			Double_t *x, *xel, *xeh, *y, *yel, *yeh;
			x = tfr->GetX();
			xel = tfr->GetEXlow();
			xeh = tfr->GetEXhigh();
			y = tfr->GetY();
			yel = tfr->GetEYlow();
			yeh = tfr->GetEYhigh();
			int nbins = tfr->GetN();
			
			//keep track of TFR values for previous taus in the event
			double f_prod = 1;
			double f_prod2 = 1; //for residual MC with real taus
			for(int t = 0; t < HPSTauPt->size(); t++){
				//check tau decision
				if(!FakeTauDecision[t]) { //do not include this tau at all
					FakeTauWeight.push_back(0);
					continue;
				}
				
				//find appropriate TFR for this pT
				double fr_curr = 0;
				//if it is below the minimum pT bin, it will not be counted
				if(HPSTauPt->at(t) < x[0] - xel[0]) {}
				//if it is above the maximum pT bin, just use the last value
				else if(HPSTauPt->at(t) > x[nbins-1] + xeh[nbins-1]) { fr_curr = y[nbins-1]; }
				else {
					for(int i = 0; i < nbins; i++){
						if(x[i] - xel[i] < HPSTauPt->at(t) && HPSTauPt->at(t) < x[i] + xeh[i]) {
							fr_curr = y[i];
							if(globalOpt->Get("tfr_unc_up",false)) fr_curr += yeh[i];
							else if(globalOpt->Get("tfr_unc_down",false)) fr_curr -= yel[i];
							break;
						}
					}
				}
				
				//calculate weight
				f_prod *= 1-fr_curr; //for combinatorics - no doublecounting
				
				//check final decision (including gen-level check if asked)
				if(!FakeTauGenDecision[t]){ //include tau in denom above, but not in weight or numer
					FakeTauWeight.push_back(0);
					continue;
				}

				//for shape estim.
				FakeTauWeight.push_back(fr_curr*f_prod2); //BEFORE including curr in f_prod2
				
				f_prod2 *= 1-fr_curr; //for combinatorics - no doublecounting
			}
			
			//second loop to divide shape estim weights by denominator
			for(int t = 0; t < HPSTauPt->size(); t++){
				FakeTauWeight[t] /= f_prod;
			}
			
			//calculate contribution
			double f = (1-f_prod2)/f_prod;
			double norm = w*f; //include event weight in case of MC
			double sumw2 = w*w*f*f; //for error calc
			
			//add result to database
			double ft_norm = 0;
			globalOpt->Get("ft_norm",ft_norm);
			globalOpt->Set("ft_norm",ft_norm + norm);

			double ft_err = 0;
			globalOpt->Get("ft_err",ft_err);
			globalOpt->Set("ft_err",ft_err + sumw2);
			
		}
		virtual void Loop() {
			if (fChain == 0) return;
			
			//loop over ntuple tree
			Long64_t nentries = fChain->GetEntriesFast();
			
			Long64_t nbytes = 0, nb = 0;
			for (Long64_t jentry=0; jentry<nentries;jentry++) {
				Long64_t ientry = LoadTree(jentry);
				if (ientry < 0) break;
				nb = fChain->GetEntry(jentry);   nbytes += nb;
				
				//clear event variables
				BJetIndices.clear();
				JetIndices.clear();
				ST_rpv = 0;
				FakeTauIndexTauJet.clear();
				FakeTauIndexMuonJet.clear();
				FakeTauMassTauJet.clear();
				FakeTauDecision.clear();
				FakeTauGenDecision.clear();
				FakeTauWeight.clear();
				
				if(!Cut()) continue;
				
				double w = Weight();
				
				if(globalOpt->Get("calcfaketau",false)) FakeTauEstimation(w);
				
				HMit sit;
				for(sit = MyBase->GetTable().begin(); sit != MyBase->GetTable().end(); sit++){
					//get histo name
					stmp = sit->first;
					htmp = sit->second;
				
					//list of cases for histo calculation and filling
					if(stmp=="qT"){//pT of muon + tau
						htmp->Fill(PtMuonTau,w);
					}
					else if(stmp=="mass"){//invariant mass of muon + tau
						htmp->Fill(MassMuonTau,w);
					}
					else if(stmp=="Tmass"){//transverse mass to check W+jet contribution: MT=sqrt(2*pt_lepton*pt_met*cos(1-deltaphi(met,lepton))
						double MT=sqrt(2*MuonPt*PFMETPatType1*(1-cos(KMath::DeltaPhi(MuonPhi,PFMETPhiPatType1))));
						htmp->Fill(MT,w);
					}
					else if(stmp=="Tmassiso"){//transverse mass to check W+jet contribution (isolated taus): MT=sqrt(2*pt_lepton*pt_met*cos(1-deltaphi(met,lepton))
						double MT=sqrt(2*MuonPt*PFMETPatType1*(1-cos(KMath::DeltaPhi(MuonPhi,PFMETPhiPatType1))));
						if(HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(0)) htmp->Fill(MT,w);
					}
					else if(stmp=="Tmassjet"){//transverse mass w/ leading jet
						if(PFJetPt->size()>0){
							double MTj=sqrt(2*PFJetPt->at(0)*PFMETPatType1*(1-cos(KMath::DeltaPhi(PFJetPhi->at(0),PFMETPhiPatType1))));
							htmp->Fill(MTj,w);
						}
					}
					else if(stmp=="Tmasstauantiisoall"){//transverse mass w/ all anti-iso taus
						for(int t = 0; t < HPSTauPt->size(); t++){
							if(!FakeTauGenDecision[t]) continue;
							double MTt=sqrt(2*HPSTauPt->at(t)*PFMETPatType1*(1-cos(KMath::DeltaPhi(HPSTauPhi->at(t),PFMETPhiPatType1))));
							htmp->Fill(MTt,w);
						}
					}
					else if(stmp=="massdimuon"){//invariant mass of muon and second muon (z+jets region)
						htmp->Fill(MassMuonMuon,w);
					}
					else if(stmp=="met"){//MET
						htmp->Fill(PFMETPatType1,w);
					}
					else if(stmp=="dphi"){//dphi of muon and tau
						htmp->Fill(KMath::DeltaPhi(MuonPhi,HPSTauPhi->at(0)),w);
					}
					else if(stmp=="dphimu"){//angular correlation w/ MET and muon
						htmp->Fill(KMath::DeltaPhi(PFMETPhiPatType1,MuonPhi),w);
					}
					else if(stmp=="dphitau"){//angular correlation w/ MET and tau
						htmp->Fill(KMath::DeltaPhi(PFMETPhiPatType1,HPSTauPhi->at(0)),w);
					}
					else if(stmp=="dphijet"){//angular correlation w/ MET and leading jet
						if(PFJetPt->size()>0) htmp->Fill(KMath::DeltaPhi(PFMETPhiPatType1,PFJetPhi->at(0)),w);
					}
					else if(stmp=="ptmu"){//muon pT
						if(globalOpt->Get("calcfaketau",false)){ //data-driven shape estimation for fake tau ST
							for(int t = 0; t < HPSTauPt->size(); t++){
								if(!FakeTauGenDecision[t]) continue;
								htmp->Fill(MuonPt,w*FakeTauWeight[t]);
							}
						}
						else{
							htmp->Fill(MuonPt,w);
						}
					}
					else if(stmp=="pttau"){//tau pT
						if(globalOpt->Get("calcfaketau",false)){ //data-driven shape estimation for fake tau
							for(int t = 0; t < HPSTauPt->size(); t++){
								if(!FakeTauGenDecision[t]) continue;
								htmp->Fill(HPSTauPt->at(t),w*FakeTauWeight[t]);
							}
						}
						else{
							htmp->Fill(HPSTauPt->at(0),w);
						}
					}
					else if(stmp=="pttauiso"){//tau pT w/ isolation (fake rate)
						if(HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(0)) htmp->Fill(HPSTauPt->at(0),w);
					}
					else if(stmp=="pttauantiiso"){//tau pT w/ anti-isolation
						if(!HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(0)) htmp->Fill(HPSTauPt->at(0),w);
					}
					else if(stmp=="pttauall"){//tau pT (all taus)
						for(int t = 0; t < HPSTauPt->size(); t++){
							if(!globalOpt->Get("checkfaketau",false) || HPSTauFakeJet->at(t)) htmp->Fill(HPSTauPt->at(t),w);
						}
					}
					else if(stmp=="pttauisoall"){//tau pT (all taus) w/ isolation (fake rate)
						for(int t = 0; t < HPSTauPt->size(); t++){
							if( (!globalOpt->Get("checkfaketau",false) || HPSTauFakeJet->at(t)) && HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(t)) htmp->Fill(HPSTauPt->at(t),w);
						}
					}
					else if(stmp=="pttauantiisoall"){//tau pT (all taus) w/ anti-isolation
						for(int t = 0; t < HPSTauPt->size(); t++){
							if(!FakeTauGenDecision[t]) continue;
							if(!HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(t)) htmp->Fill(HPSTauPt->at(t),w);
						}
					}
					else if(stmp=="tauisostatus"){//iso status of each tau candidate
						for(int t = 0; t < HPSTauPt->size(); t++){
							if(HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(t)) htmp->Fill(t+1,w);
						}
					}
					else if(stmp=="energytau"){//visible energy of tau
						//needs a 4vec...
						TLorentzVector v_tau;
						v_tau.SetPtEtaPhiM(HPSTauPt->at(0),HPSTauEta->at(0),HPSTauPhi->at(0),1.77682);
						htmp->Fill(v_tau.E(),w);
					
					}
					else if(stmp=="bcheckall"){//if fake taus come from a b quark
						for(int t = 0; t < HPSTauPt->size(); t++){
							if(HPSTauFakeJet->at(t)) htmp->Fill(HPSTauFakeBJet->at(t),w);
						}
					}
					else if(stmp=="bcheckiso"){//if iso fake taus come from a b quark
						for(int t = 0; t < HPSTauPt->size(); t++){
							if(HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(t) && HPSTauFakeJet->at(t)) htmp->Fill(HPSTauFakeBJet->at(t),w);
						}
					}
					else if(stmp=="bcheckleadiso"){//if lead iso fake tau comes from a b quark
						if(HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(0) && HPSTauFakeJet->at(0)) htmp->Fill(HPSTauFakeBJet->at(0),w);
					}
					else if(stmp=="phimu"){//muon phi
						htmp->Fill(MuonPhi,w);
					}
					else if(stmp=="phitau"){//tau phi
						htmp->Fill(HPSTauPhi->at(0),w);
					}
					else if(stmp=="etamu"){//muon eta
						htmp->Fill(MuonEta,w);
					}
					else if(stmp=="etatau"){//tau eta
						if(globalOpt->Get("calcfaketau",false)){ //data-driven shape estimation for fake tau ST
							for(int t = 0; t < HPSTauEta->size(); t++){
								if(!FakeTauGenDecision[t]) continue;
								htmp->Fill(HPSTauEta->at(t),w*FakeTauWeight[t]);
							}
						}
						else{
							htmp->Fill(HPSTauEta->at(0),w);
						}
					}
					else if(stmp=="etatauall"){//tau eta (all taus)
						for(int t = 0; t < HPSTauPt->size(); t++){
							if(!globalOpt->Get("checkfaketau",false) || HPSTauFakeJet->at(t)) htmp->Fill(HPSTauEta->at(t),w);
						}
					}
					else if(stmp=="etatauisoall"){//tau eta (all taus) w/ isolation (fake rate)
						for(int t = 0; t < HPSTauPt->size(); t++){
							if( (!globalOpt->Get("checkfaketau",false) || HPSTauFakeJet->at(t)) && HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(t)) htmp->Fill(HPSTauEta->at(t),w);
						}
					}
					else if(stmp=="nvertex"){//# good vertices
						htmp->Fill(nGoodVertices,w);
					}
					else if(stmp=="nmuon"){//muon multiplicity
						htmp->Fill(MuonMultiplicity,w);
					}
					else if(stmp=="ntau"){//tau multiplicity
						htmp->Fill(HPSTauMultiplicity,w);
					}
					else if(stmp=="ntauiso"){//tau multiplicity (iso taus)
						int ctr = 0;
						for(int t = 0; t < HPSTauPt->size(); t++){
							if(HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(t)) ctr++;
						}
						if(ctr>0) htmp->Fill(ctr,w);
					}
					else if(stmp=="ntauantiiso"){//tau multiplicity (anti-iso taus)
						int ctr = 0;
						for(int t = 0; t < HPSTauPt->size(); t++){
							if(!FakeTauGenDecision[t]) continue;
							if(!HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(t)) ctr++;
						}
						if(ctr>0) htmp->Fill(ctr,w);
					}
					else if(stmp=="njet"){//jet multiplicity
						htmp->Fill(PFJetMultiplicity,w);
					}
					else if(stmp=="leadjetpt"){//leading jet pT
						if(PFJetPt->size()>0){
							htmp->Fill(PFJetPt->at(0),w);
						}
					}
					else if(stmp=="subleadjetpt"){//sub-leading jet pT
						if(PFJetPt->size()>1){
							for(unsigned j = 1; j < PFJetPt->size(); j++){
								htmp->Fill(PFJetPt->at(j),w);
							}
						}
					}
					else if(stmp=="leadbjetpt"){//leading b-jet pT
						if(PFJetPt->size()>0){
							for(unsigned j = 0; j < PFJetPt->size(); j++){
								if(PFJetCSVBTag->at(j)>0.244){
									htmp->Fill(PFJetPt->at(j),w);
									break;
								}
							}
						}
					}
					else if(stmp=="secondbjetpt"){//second leading b-jet pT
						if(PFJetPt->size()>0){
							bool found_lead = false;
							for(unsigned j = 0; j < PFJetPt->size(); j++){
								if(PFJetCSVBTag->at(j)>0.244){
									if(!found_lead){
										found_lead = true;
										continue;
									}
									else{
										htmp->Fill(PFJetPt->at(j),w);
										break;
									}
								}
							}
						}
					}
					else if(stmp=="nbjet"){//b-jet multiplicity
						if(PFJetPt->size()>0){
							int nbjet = 0;
							for(unsigned j = 0; j < PFJetPt->size(); j++){
								if(PFJetCSVBTag->at(j)>0.244) nbjet++;
							}
							htmp->Fill(nbjet,w);
						}
					}
					else if(stmp=="muoniso"){//muon isolation variable
						htmp->Fill(MuonPFRelIsoTight,w);
					}
					else if(stmp=="charge"){//product of muon and tau charges
						htmp->Fill(MuonCharge*HPSTauCharge->at(0),w);
					}
					else if(stmp=="btagjetpt"){//pt of selected b-tag jet
						htmp->Fill(PFMainJetPt[1],w);
					}
					else if(stmp=="btagjetmass"){//mass of selected b-tag jet (from 4vec)
						TLorentzVector v_jet;
						v_jet.SetPtEtaPhiE(PFMainJetPt[1],PFMainJetEta[1],PFMainJetPhi[1],PFMainJetEnergy[1]);
						htmp->Fill(v_jet.M(),w);
					}
					else if(stmp=="addljetpt"){//pt of selected addl. jet
						htmp->Fill(PFMainJetPt[0],w);
					}
					else if(stmp=="addljetmass"){//mass of selected addl. jet (from 4vec)
						TLorentzVector v_jet;
						v_jet.SetPtEtaPhiE(PFMainJetPt[0],PFMainJetEta[0],PFMainJetPhi[0],PFMainJetEnergy[0]);
						htmp->Fill(v_jet.M(),w);
					}
					else if(stmp=="leadseljetpt"){//pt of leading selected jet
						htmp->Fill(max(PFMainJetPt[0],PFMainJetPt[1]),w);
					}
					else if(stmp=="secondseljetpt"){//pt of second selected jet
						htmp->Fill(min(PFMainJetPt[0],PFMainJetPt[1]),w);
					}
					else if(stmp=="dvtx"){//difference in vertex indices for muon and selected jets
						bool foundB = false;
						bool foundJ = false;
						if(globalOpt->Get("lqd321sel",false)) {
							htmp->Fill(abs(MuonVtxIndex - PFJetVtxIndex->at(BJetIndices[0])),w);
							for(int j = 0; j < 4; j++){
								htmp->Fill(abs(MuonVtxIndex - PFJetVtxIndex->at(JetIndices[j])),w);
							}
						}
						else {
							for(int j = 0; j < PFJetPt->size(); j++){
								if(foundJ && foundB) break;
							
								if(!foundB && PFJetCSVBTag->at(j)>0.244){
									foundB = true;
									htmp->Fill(abs(MuonVtxIndex - PFJetVtxIndex->at(j)),w);
								}
								else if(!foundJ){
									foundJ = true;
									htmp->Fill(abs(MuonVtxIndex - PFJetVtxIndex->at(j)),w);
								}
							}
						}
						
					}
					else if(stmp=="masstaub"){//invariant mass of tau and matched (b)jet
						htmp->Fill(MassTauJet,w);
					}
					else if(stmp=="masstaubantiiso"){//invariant mass of tau and matched (b)jet (anti-iso region)
						for(int t = 0; t < HPSTauPt->size(); t++){
							if(!FakeTauGenDecision[t]) continue;
							htmp->Fill(FakeTauMassTauJet[t],w);
						}
					}
					else if(stmp=="massmuonb"){//invariant mass of muon and matched (b)jet
						htmp->Fill(MassMuonJet,w);
					}
					else if(stmp=="st"){//scalar sum of momenta (defined differently for lqd321)
						double st_base = 0;
						if(globalOpt->Get("lqd321sel",false)) st_base = MuonPt + PFJetPt->at(BJetIndices[0]) + PFJetPt->at(JetIndices[0]) + PFJetPt->at(JetIndices[1]) + PFJetPt->at(JetIndices[2]) + PFJetPt->at(JetIndices[3]);
						else if(!globalOpt->Get("mainsel",false)) st_base = MuonPt + PFJetPt->at(0) + PFJetPt->at(1);
						else st_base = MuonPt + PFMainJetPt[0] + PFMainJetPt[1];
						
						if(globalOpt->Get("calcfaketau",false)){ //data-driven shape estimation for fake tau ST
							for(int t = 0; t < HPSTauPt->size(); t++){
								if(!FakeTauGenDecision[t]) continue;
								htmp->Fill(st_base + HPSTauPt->at(t),w*FakeTauWeight[t]);
							}
						}
						else{
							double st_tot = st_base + HPSTauPt->at(0);
							htmp->Fill(st_tot,w);
							//printeventnum uses histo minimum as the threshold
							if(globalOpt->Get("printeventnum",false) && st_tot > htmp->GetXaxis()->GetXmin()){
								//run:ls:event format for crab
								string filename = "";
								localOpt->Get("filename",filename);
								cout << "ST = " << st_tot << " GeV in " << filename << endl;
								cout << run << ":" << ls << ":" << event << endl;
								cout << "muon: pt = " << MuonPt << ", eta = " << MuonEta << ", phi = " << MuonPhi << endl;
								cout << "tau: pt = " << HPSTauPt->at(0) << ", eta = " << HPSTauEta->at(0) << ", phi = " << HPSTauPhi->at(0) << endl;
								if(globalOpt->Get("lqd321sel",false)){
									cout << "b-jet: pt = " << PFJetPt->at(BJetIndices[0]) << ", eta = " << PFJetEta->at(BJetIndices[0]) << ", phi = " << PFJetPhi->at(BJetIndices[0]) << endl;
									for(int j = 0; j < 4; j++){
										cout << "jet " << j+1 << ": pt = " << PFJetPt->at(JetIndices[j]) << ", eta = " << PFJetEta->at(JetIndices[j]) << ", phi = " << PFJetPhi->at(JetIndices[j]) << endl;
									}
								}
								else if(!globalOpt->Get("mainsel",false)){
									cout << "jet 1: pt = " << PFJetPt->at(0) << ", eta = " << PFJetEta->at(0) << ", phi = " << PFJetPhi->at(0) << endl;
									cout << "jet 2: pt = " << PFJetPt->at(1) << ", eta = " << PFJetEta->at(1) << ", phi = " << PFJetPhi->at(1) << endl;
								}
								else{
									cout << "b-jet: pt = " << PFMainJetPt[1] << ", eta = " << PFMainJetEta[1] << ", phi = " << PFMainJetPhi[1] << endl;
									cout << "jet: pt = " << PFMainJetPt[0] << ", eta = " << PFMainJetEta[0] << ", phi = " << PFMainJetPhi[0] << endl;
								}
							}
						}
					}
					else if(stmp=="stlep"){//scalar sum of momenta for leptons
						double st_base = MuonPt;
						
						if(globalOpt->Get("calcfaketau",false)){ //data-driven shape estimation for fake tau ST
							for(int t = 0; t < HPSTauPt->size(); t++){
								if(!FakeTauGenDecision[t]) continue;
								htmp->Fill(st_base + HPSTauPt->at(t),w*FakeTauWeight[t]);
							}
						}
						else{
							htmp->Fill(st_base + HPSTauPt->at(0),w);
						}
					}
					else if(stmp=="stjet"){//scalar sum of momenta for jets
						double st_base = 0;
						if(globalOpt->Get("lqd321sel",false)) st_base = PFJetPt->at(BJetIndices[0]) + PFJetPt->at(JetIndices[0]) + PFJetPt->at(JetIndices[1]) + PFJetPt->at(JetIndices[2]) + PFJetPt->at(JetIndices[3]);
						else if(!globalOpt->Get("mainsel",false)) st_base = PFJetPt->at(0) + PFJetPt->at(1);
						else st_base = PFMainJetPt[0] + PFMainJetPt[1];
						
						if(globalOpt->Get("calcfaketau",false)){ //data-driven shape estimation for fake tau ST
							for(int t = 0; t < HPSTauPt->size(); t++){
								if(!FakeTauGenDecision[t]) continue;
								htmp->Fill(st_base,w*FakeTauWeight[t]);
							}
						}
						else{
							htmp->Fill(st_base,w);
						}
					}
					else if(stmp=="st_alt"){//scalar sum of momenta (alfredo's weird definition)
						double st = MuonPt + HPSTauPt->at(0);
						if(PFMainJetCSVBTag[0]>0.244) st += PFMainJetPt[0];
						if(PFMainJetCSVBTag[1]>0.244) st += PFMainJetPt[1];
						htmp->Fill(st,w);
					}
					else if(stmp=="strpv"){//scalar sum of momenta for RPV - varying # objects based on selection, computed in Cut()
						htmp->Fill(ST_rpv,w);
					}
					else { //if it's a histogram with no known variable or calculation, do nothing
					}
				}
			}
			
			if(globalOpt->Get("plotoverflow",false)){
				HMit sit;
				for(sit = MyBase->GetTable().begin(); sit != MyBase->GetTable().end(); sit++){
					//get histo name
					stmp = sit->first;
					htmp = sit->second;
					
					//temporary histo to calculate error correctly when adding overflow bin to last bin
					TH1F* otmp = (TH1F*)htmp->Clone();
					otmp->Reset("ICEM");
					int ovbin = htmp->GetNbinsX()+1;
					double err = 0.;
					otmp->SetBinContent(ovbin-1,htmp->IntegralAndError(ovbin,ovbin,err));
					otmp->SetBinError(ovbin-1,err);
					
					//add overflow bin to last bin
					htmp->Add(otmp);
					
					//remove overflow bin from htmp (for consistent integral/yield)
					htmp->SetBinContent(ovbin,0);
					htmp->SetBinError(ovbin,0);
					
					delete otmp;
				}
			}
		}		

	protected:
		//member variables
		KBase* MyBase;
		OptionMap* localOpt;
		OptionMap* globalOpt;
		map<pair<int,int>,int> countmap;
		string stmp;
		TH1F* htmp;
		
		//variables for special event selections (RPV)
		vector<int> BJetIndices;
		vector<int> JetIndices;
		double ST_rpv;
		
		//variables for data-driven fake tau ST shape
		vector<int> FakeTauIndexLeadingJet;
		vector<int> FakeTauIndexSecondJet;
		vector<int> FakeTauIndexTauJet;
		vector<int> FakeTauIndexMuonJet;
		vector<double> FakeTauMassTauJet;
		vector<bool> FakeTauDecision;
		vector<bool> FakeTauGenDecision;
		vector<double> FakeTauWeight;
};

void KBase::Build(){
	if(!isBuilt) {
		if(MyBuilder==0) MyBuilder = new KBuilder(this,tree);
		MyBuilder->Loop(); //loop over tree to build histograms
		isBuilt = true;
	}
}

//---------------------------------------------------------
//extension of builder class for data - has blinding option
class KBuilderData : public KBuilder {
	public:
		//constructors
		KBuilderData() : KBuilder() { }
		KBuilderData(KBase* MyBase_, TTree* tree_) : KBuilder(MyBase_,tree_) {}
		//destructor
		virtual ~KBuilderData() {}
		
		//functions for histo creation
		bool Cut(){
			bool goodEvent = KBuilder::Cut();
			
			//special blinding option for data (disabled by default)
			if(globalOpt->Get("blind",false)){
				goodEvent &= (MassTauJet < 180); //do not look at signal region
				//could make this setting into a double value for variable blinding...
			}
			
			return goodEvent;
		}
};

void KBaseData::Build(){
	if(!isBuilt) {
		if(MyBuilder==0) MyBuilder = new KBuilderData(this,tree);
		MyBuilder->Loop(); //loop over tree to build histograms
		isBuilt = true;
	}
}

//------------------------------------------------------------------------------------------------------------
//extension of builder class for MC - has weighting (corrections & normalization), extra cuts (fake tau, etc.)
class KBuilderMC : public KBuilder {
	public:
		//constructors
		KBuilderMC() : KBuilder() { }
		KBuilderMC(KBase* MyBase_, TTree* tree_) : KBuilder(MyBase_,tree_) { }
		//destructor
		virtual ~KBuilderMC() {}
		
		//functions for histo creation
		bool Cut(){
			bool goodEvent = KBuilder::Cut();
			
			//add extra MC cuts here
			if(!globalOpt->Get("antiisosel",false)){ //do not do these checks for the anti-iso region, they will be done at object level where appropriate
				string normtype = "";
				localOpt->Get("normtype",normtype);
				int nchargino = 0;
				if(normtype=="faketau" && globalOpt->Get("dofaketau",false)) goodEvent &= HPSTauFakeJet->at(0);
				else if(normtype=="nofaketau" && globalOpt->Get("dofaketau",false)) goodEvent &= !HPSTauFakeJet->at(0);
				else if(normtype=="ttbar" && globalOpt->Get("dottbar",false)) goodEvent &= HPSTauReal->at(0);
				else if(normtype=="faketaulep" && globalOpt->Get("dottbar",false)) goodEvent &= HPSTauFakeLep->at(0);
				else if(normtype=="chargino" && globalOpt->Get("dochargino",false) && globalOpt->Get("nchargino",nchargino)) goodEvent &= (CharginoMultiplicity == nchargino);
			}
			
			return goodEvent;
		}
		double Weight(){
			double w = 1.;
			
			//check option in case correction types are disabled globally
			//(enabled by default)
			if(globalOpt->Get("pucorr",true)) {
				TH1F* puWeights;
				globalOpt->Get("puWeights",puWeights);
				w *= puWeights->GetBinContent(puWeights->GetXaxis()->FindBin(trueNInteraction));
			}

			if(globalOpt->Get("mucorr",true)) {
				TH2F* muIDTight;
				globalOpt->Get("muIDTight",muIDTight);
				w *= muIDTight->GetBinContent(muIDTight->FindBin(fabs(MuonEta),MuonPt));
			}
			
			if(globalOpt->Get("btagcorr",true)) {
				int bSF = 0;
				int mSF = 0;
				globalOpt->Get("btagSFunc",bSF);
				globalOpt->Get("mistagSFunc",mSF);
				//btag corrs only applied to specific plots in preselection
				if(stmp=="leadbjetpt" || stmp=="secondbjetpt" || globalOpt->Get("mainsel",false) || globalOpt->Get("lqd321sel",false) || globalOpt->Get("dobtagcorr",false))
					w *= GetBtagScale(PFJetPt,PFJetEta,PFJetPartonFlavour,bSF,mSF);
			}
			
			//now do scaling: norm*xsection/nevents
			//should this be a separate function using Scale()?
			int nEventProc = 0;
			double xsection = 0;
			if(localOpt->Get("nEventProc",nEventProc) && nEventProc>0 && localOpt->Get("xsection",xsection)) w *= xsection/nEventProc;
			
			//get norm from options
			double norm = 0;
			//use lumi norm (default)
			if(globalOpt->Get("luminorm",norm)) w *= norm;
			
			return w;
		}

};

void KBaseMC::Build(){
	if(!isBuilt) {
		if(MyBuilder==0) MyBuilder = new KBuilderMC(this,tree);
		MyBuilder->Loop(); //loop over tree to build histograms
		isBuilt = true;
	}
}

#endif

#ifndef KSKIMMERVARIATORS_H
#define KSKIMMERVARIATORS_H

//custom headers
#include "KVariation.h"
#include "KSkimmer.h"
#include "KMath.h"

//ROOT headers
#include <TLorentzVector.h>
#include <TMath.h>

//STL headers
#include <string>
#include <vector>

using namespace std;

//base class for variators is in KVariation.h


//----------------------------------------------------
//variation of jet-related variables
//for: JECs, various Zinv cleanings
class KJetVariator : public KVariator<KSkimmer> {
	public:
		enum vartypes { NoVar=0, JECup=1, JECdown=2, clean=3, manual=4, cleanDY=5, cleanGJ=6, cleanGJloose=7 };
		//constructor
		KJetVariator() : KVariator<KSkimmer>() { }
		KJetVariator(string name_, OptionMap* localOpt_) : KVariator<KSkimmer>(name_,localOpt_), enabled(false), vtype(NoVar),
			b_bestPhoton(true),b_Jets(true),b_HTJetsMask(true),b_MHTJetsMask(true),b_Jets_partonFlavor(true),b_Jets_hadronFlavor(true),b_Jets_bDiscriminatorCSV(true)
		{
			//check options
			string vname = "";
			localOpt->Get("vartype",vname);
			if(vname=="JECup") vtype = JECup;
			else if(vname=="JECdown") vtype = JECdown;
			else if(vname=="clean") vtype = clean;
			else if(vname=="manual") vtype = manual;
			else if(vname=="cleanDY") vtype = cleanDY;
			else if(vname=="cleanGJ") vtype = cleanGJ;
			else if(vname=="cleanGJloose") vtype = cleanGJloose;
		}
		//helper functions
		virtual void EnableBranches(string* branches, unsigned size){
			if(enabled) return;
			for(unsigned i = 0; i < size; ++i){
				looper->fChain->SetBranchStatus(branches[i].c_str(),1);
			}
			enabled = true;
		}
		virtual void CheckBranches(){
			if(vtype==JECup){
				string branchlist[] = {"JetsJECup","HTJetsMaskJECup","MHTJetsMaskJECup","JetsJECup_partonFlavor","JetsJECup_hadronFlavor","JetsJECup_bDiscriminatorCSV","JetIDJECup","NJetsJECup","BTagsJECup","HTJECup","MHTJECup","MHT_PhiJECup","DeltaPhi1JECup","DeltaPhi2JECup","DeltaPhi3JECup","DeltaPhi4JECup"};
				unsigned branchsize = 16;
				EnableBranches(branchlist,branchsize);
			}
			else if(vtype==JECdown){
				string branchlist[] = {"JetsJECdown","HTJetsMaskJECdown","MHTJetsMaskJECdown","JetsJECdown_partonFlavor","JetsJECdown_hadronFlavor","JetsJECdown_bDiscriminatorCSV","JetIDJECdown","NJetsJECdown","BTagsJECdown","HTJECdown","MHTJECdown","MHT_PhiJECdown","DeltaPhi1JECdown","DeltaPhi2JECdown","DeltaPhi3JECdown","DeltaPhi4JECdown"};
				unsigned branchsize = 16;
				EnableBranches(branchlist,branchsize);
			}
			else if(vtype==clean){
				string branchlist[] = {"Jetsclean","HTJetsMaskclean","MHTJetsMaskclean","Jetsclean_partonFlavor","Jetsclean_hadronFlavor","Jetsclean_bDiscriminatorCSV","JetIDclean","NJetsclean","BTagsclean","HTclean","MHTclean","MHT_Phiclean","METPtclean","METPhiclean","DeltaPhi1clean","DeltaPhi2clean","DeltaPhi3clean","DeltaPhi4clean"};
				unsigned branchsize = 18;
				EnableBranches(branchlist,branchsize);
			}
			else if(vtype==manual){
				string branchlist[] = {"Jetsclean","HTJetsMaskclean","MHTJetsMaskclean","Jetsclean_partonFlavor","Jetsclean_hadronFlavor","Jetsclean_bDiscriminatorCSV","JetIDclean","photonCands","photon_hadTowOverEM","photon_passElectronVeto","photon_pfNeutralIsoRhoCorr","photon_pfGammaIsoRhoCorr","MHTJetsMask","HTJetsMask","METPtclean","METPhiclean"};
				unsigned branchsize = 16;
				EnableBranches(branchlist,branchsize);
			}
			else if(vtype==cleanDY){
				string branchlist[] = {"JetscleanDY","HTJetsMaskcleanDY","MHTJetsMaskcleanDY","JetscleanDY_partonFlavor","JetscleanDY_hadronFlavor","JetscleanDY_bDiscriminatorCSV","JetIDcleanDY","NJetscleanDY","BTagscleanDY","isoElectronTrackscleanDY","isoMuonTrackscleanDY","isoPionTrackscleanDY","HTcleanDY","MHTcleanDY","MHT_PhicleanDY","METPtcleanDY","METPhicleanDY","DeltaPhi1cleanDY","DeltaPhi2cleanDY","DeltaPhi3cleanDY","DeltaPhi4cleanDY"};
				unsigned branchsize = 21;
				EnableBranches(branchlist,branchsize);
			}
			else if(vtype==cleanGJ){
				string branchlist[] = {"JetscleanGJ","HTJetsMaskcleanGJ","MHTJetsMaskcleanGJ","JetscleanGJ_partonFlavor","JetscleanGJ_hadronFlavor","JetscleanGJ_bDiscriminatorCSV","JetIDcleanGJ","NJetscleanGJ","BTagscleanGJ","isoElectronTrackscleanGJ","isoMuonTrackscleanGJ","isoPionTrackscleanGJ","HTcleanGJ","MHTcleanGJ","MHT_PhicleanGJ","METPtcleanGJ","METPhicleanGJ","DeltaPhi1cleanGJ","DeltaPhi2cleanGJ","DeltaPhi3cleanGJ","DeltaPhi4cleanGJ"};
				unsigned branchsize = 21;
				EnableBranches(branchlist,branchsize);
			}
			else if(vtype==cleanGJloose){
				string branchlist[] = {"bestPhotonLoose","NumPhotonsLoose","JetscleanGJloose","HTJetsMaskcleanGJloose","MHTJetsMaskcleanGJloose","JetscleanGJloose_partonFlavor","JetscleanGJloose_hadronFlavor","JetscleanGJloose_bDiscriminatorCSV","JetIDcleanGJloose","NJetscleanGJloose","BTagscleanGJloose","isoElectronTrackscleanGJloose","isoMuonTrackscleanGJloose","isoPionTrackscleanGJloose","HTcleanGJloose","MHTcleanGJloose","MHT_PhicleanGJloose","METPtcleanGJloose","METPhicleanGJloose","DeltaPhi1cleanGJloose","DeltaPhi2cleanGJloose","DeltaPhi3cleanGJloose","DeltaPhi4cleanGJloose"};
				unsigned branchsize = 23;
				EnableBranches(branchlist,branchsize);
			}
			
			//check pointer branches
			if(!looper->fChain->GetBranchStatus("bestPhoton")) b_bestPhoton = false;
			if(!looper->fChain->GetBranchStatus("Jets")) b_Jets = false;
			if(!looper->fChain->GetBranchStatus("HTJetsMask")) b_HTJetsMask = false;
			if(!looper->fChain->GetBranchStatus("MHTJetsMask")) b_MHTJetsMask = false;
			if(!looper->fChain->GetBranchStatus("Jets_partonFlavor")) b_Jets_partonFlavor = false;
			if(!looper->fChain->GetBranchStatus("Jets_hadronFlavor")) b_Jets_hadronFlavor = false;
			if(!looper->fChain->GetBranchStatus("Jets_bDiscriminatorCSV")) b_Jets_bDiscriminatorCSV = false;
		}
		//functions
		virtual void DoVariation() {
			//store original values
			if(b_bestPhoton) bestPhoton = *(looper->bestPhoton);
			if(b_Jets) Jets = *(looper->Jets);
			if(b_HTJetsMask) HTJetsMask = *(looper->HTJetsMask);
			if(b_MHTJetsMask) MHTJetsMask = *(looper->MHTJetsMask);
			if(b_Jets_partonFlavor) Jets_partonFlavor = *(looper->Jets_partonFlavor);
			if(b_Jets_hadronFlavor) Jets_hadronFlavor = *(looper->Jets_hadronFlavor);
			if(b_Jets_bDiscriminatorCSV)Jets_bDiscriminatorCSV = *(looper->Jets_bDiscriminatorCSV);
			JetID = looper->JetID;
			NumPhotons = looper->NumPhotons;
			NJets = looper->NJets;
			BTags = looper->BTags;
			isoElectronTracks = looper->isoElectronTracks;
			isoMuonTracks = looper->isoMuonTracks;
			isoPionTracks = looper->isoPionTracks;
			HT = looper->HT;
			METPt = looper->METPt;
			METPhi = looper->METPhi;
			MHT = looper->MHT;
			MHT_Phi = looper->MHT_Phi;
			DeltaPhi1 = looper->DeltaPhi1;
			DeltaPhi2 = looper->DeltaPhi2;
			DeltaPhi3 = looper->DeltaPhi3;
			DeltaPhi4 = looper->DeltaPhi4;
			
			//set to clean vars
			if(vtype==JECup){
				if(b_Jets) *(looper->Jets) = *(looper->JetsJECup);
				if(b_HTJetsMask) *(looper->HTJetsMask) = *(looper->HTJetsMaskJECup);
				if(b_MHTJetsMask) *(looper->MHTJetsMask) = *(looper->MHTJetsMaskJECup);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->JetsJECup_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->JetsJECup_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->JetsJECup_bDiscriminatorCSV);
				looper->JetID = looper->JetIDJECup;
				looper->NJets = looper->NJetsJECup;
				looper->BTags = looper->BTagsJECup;
				looper->HT = looper->HTJECup;
				looper->MHT = looper->MHTJECup;
				looper->MHT_Phi = looper->MHT_PhiJECup;
				looper->DeltaPhi1 = looper->DeltaPhi1JECup;
				looper->DeltaPhi2 = looper->DeltaPhi2JECup;
				looper->DeltaPhi3 = looper->DeltaPhi3JECup;
				looper->DeltaPhi4 = looper->DeltaPhi4JECup;
			}
			else if(vtype==JECdown){
				if(b_Jets) *(looper->Jets) = *(looper->JetsJECdown);
				if(b_HTJetsMask) *(looper->HTJetsMask) = *(looper->HTJetsMaskJECdown);
				if(b_MHTJetsMask) *(looper->MHTJetsMask) = *(looper->MHTJetsMaskJECdown);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->JetsJECdown_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->JetsJECdown_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->JetsJECdown_bDiscriminatorCSV);
				looper->JetID = looper->JetIDJECdown;
				looper->NJets = looper->NJetsJECdown;
				looper->BTags = looper->BTagsJECdown;
				looper->HT = looper->HTJECdown;
				looper->MHT = looper->MHTJECdown;
				looper->MHT_Phi = looper->MHT_PhiJECdown;
				looper->DeltaPhi1 = looper->DeltaPhi1JECdown;
				looper->DeltaPhi2 = looper->DeltaPhi2JECdown;
				looper->DeltaPhi3 = looper->DeltaPhi3JECdown;
				looper->DeltaPhi4 = looper->DeltaPhi4JECdown;
			}
			else if(vtype==clean){
				if(b_Jets) *(looper->Jets) = *(looper->Jetsclean);
				if(b_HTJetsMask) *(looper->HTJetsMask) = *(looper->HTJetsMaskclean);
				if(b_MHTJetsMask) *(looper->MHTJetsMask) = *(looper->MHTJetsMaskclean);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->Jetsclean_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->Jetsclean_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->Jetsclean_bDiscriminatorCSV);
				looper->JetID = looper->JetIDclean;
				looper->NJets = looper->NJetsclean;
				looper->BTags = looper->BTagsclean;
				looper->HT = looper->HTclean;
				looper->MHT = looper->MHTclean;
				looper->MHT_Phi = looper->MHT_Phiclean;
				looper->METPt = looper->METPtclean;
				looper->METPhi = looper->METPhiclean;
				looper->DeltaPhi1 = looper->DeltaPhi1clean;
				looper->DeltaPhi2 = looper->DeltaPhi2clean;
				looper->DeltaPhi3 = looper->DeltaPhi3clean;
				looper->DeltaPhi4 = looper->DeltaPhi4clean;
			}
			else if(vtype==manual){
				//for these variables, use old-cleaned collections
				if(b_Jets) *(looper->Jets) = *(looper->Jetsclean);
				if(b_HTJetsMask) *(looper->HTJetsMask) = *(looper->HTJetsMaskclean);
				if(b_MHTJetsMask) *(looper->MHTJetsMask) = *(looper->MHTJetsMaskclean);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->Jetsclean_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->Jetsclean_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->Jetsclean_bDiscriminatorCSV);
				looper->JetID = looper->JetIDclean;
				
				//modified photon ID - do not apply sigma_ieta_ieta_, charged hadron isolation cuts
				double bestPhotonPt = 0;
				int bestPhotonIndex = -1;
				int newNumPhotons = 0;
				for(unsigned p = 0; p < looper->photonCands->size(); ++p){
					bool isBarrelPhoton = fabs(looper->photonCands->at(p).Eta()) < 1.4442;
					bool isEndcapPhoton = fabs(looper->photonCands->at(p).Eta()) > 1.566 && fabs(looper->photonCands->at(p).Eta()) < 2.5;
					bool passAcc = isBarrelPhoton || isEndcapPhoton;
					//no sigma_ieta_ieta cuts in ID
					bool passID =  (isBarrelPhoton && looper->photon_hadTowOverEM->at(p) < 0.028 && looper->photon_passElectronVeto->at(p))
								|| (isEndcapPhoton && looper->photon_hadTowOverEM->at(p) < 0.093 && looper->photon_passElectronVeto->at(p));
					//no charged iso cuts in iso
					bool passIso = (isBarrelPhoton && looper->photon_pfNeutralIsoRhoCorr->at(p) < (7.23 + TMath::Exp(0.0028*(looper->photonCands->at(p).Pt()+0.5408))) && looper->photon_pfGammaIsoRhoCorr->at(p) < (2.11 + 0.0014*looper->photonCands->at(p).Pt()))
								|| (isEndcapPhoton && looper->photon_pfNeutralIsoRhoCorr->at(p) < (8.89 + 0.01725*looper->photonCands->at(p).Pt()) && looper->photon_pfGammaIsoRhoCorr->at(p) < (3.09 + 0.0091*looper->photonCands->at(p).Pt()));
					bool goodPhoton = passAcc && passID && passIso && looper->photonCands->at(p).Pt() > 100.0;
					if(goodPhoton){
						++newNumPhotons;
						if(looper->photonCands->at(p).Pt() > bestPhotonPt){
							bestPhotonPt = looper->photonCands->at(p).Pt();
							bestPhotonIndex = p;
						}
					}
				}
				
				//reassign best photon if found
				if(bestPhotonIndex!=-1){
					vector<TLorentzVector> bestPhotonClean;
					bestPhotonClean.push_back(looper->photonCands->at(bestPhotonIndex));
					*(looper->bestPhoton) = bestPhotonClean;
				}
				looper->NumPhotons = newNumPhotons;
				
				//recalculate jet-based quantities, cleaning out new bestPhoton
				vector<TLorentzVector> htjetsClean;
				TLorentzVector mhtLorentzClean; mhtLorentzClean.SetPtEtaPhiE(0,0,0,0);
				double HTClean = 0.0;
				int NJetsClean = 0;
				int BTagsClean = 0;
				for(unsigned j = 0; j < looper->Jets->size(); ++j){
					//MHTJets cuts
					if(!looper->MHTJetsMask->at(j)) continue;
					
					//object cleaning
					bool overlap = false;
					for(unsigned m = 0; m < looper->Muons->size(); ++m){
						if(KMath::DeltaR(looper->Muons->at(m).Phi(),looper->Muons->at(m).Eta(),looper->Jets->at(j).Phi(),looper->Jets->at(j).Eta())<0.4) { overlap = true; break; }
					}
					if(overlap) continue;
					for(unsigned e = 0; e < looper->Electrons->size(); ++e){
						if(KMath::DeltaR(looper->Electrons->at(e).Phi(),looper->Electrons->at(e).Eta(),looper->Jets->at(j).Phi(),looper->Jets->at(j).Eta())<0.4) { overlap = true; break; }
					}
					if(overlap) continue;
					for(unsigned g = 0; g < looper->bestPhoton->size(); ++g){
						if(KMath::DeltaR(looper->bestPhoton->at(g).Phi(),looper->bestPhoton->at(g).Eta(),looper->Jets->at(j).Phi(),looper->Jets->at(j).Eta())<0.4) { overlap = true; break; }
					}
					if(overlap) continue;
					
					//jet has passed all cuts
					mhtLorentzClean -= looper->Jets->at(j);
					
					//HTJets cuts
					if(looper->HTJetsMask->at(j)) {
						htjetsClean.push_back(looper->Jets->at(j));
						HTClean += looper->Jets->at(j).Pt();
						++NJetsClean;
						if(looper->Jets_bDiscriminatorCSV->at(j) > 0.890) ++BTagsClean;
					}
				}
				
				//set to clean vars
				looper->NJets = NJetsClean;
				looper->BTags = BTagsClean;
				looper->HT = HTClean;
				looper->MHT = mhtLorentzClean.Pt();
				looper->MHT_Phi = mhtLorentzClean.Phi();

				double MHTPhiClean = mhtLorentzClean.Phi();
				//test MHT clean calc
				//cout << "delta MHT clean = " << mhtLorentzClean.Pt() - looper->MHTclean << endl;
				
				//reset vars
				looper->DeltaPhi1 = looper->DeltaPhi2 = looper->DeltaPhi3 = looper->DeltaPhi4 = 0;
				for(unsigned j = 0; j < min((unsigned)4,(unsigned)htjetsClean.size()); ++j){
					//recalc delta phi
					if(j==0) looper->DeltaPhi1 = abs(KMath::DeltaPhi(htjetsClean.at(j).Phi(),MHTPhiClean));
					else if(j==1) looper->DeltaPhi2 = abs(KMath::DeltaPhi(htjetsClean.at(j).Phi(),MHTPhiClean));
					else if(j==2) looper->DeltaPhi3 = abs(KMath::DeltaPhi(htjetsClean.at(j).Phi(),MHTPhiClean));
					else if(j==3) looper->DeltaPhi4 = abs(KMath::DeltaPhi(htjetsClean.at(j).Phi(),MHTPhiClean));
				}
				
				//too lazy to recalculate these
				looper->METPt = looper->METPtclean;
				looper->METPhi = looper->METPhiclean;
			}
			else if(vtype==cleanDY){
				if(b_Jets) *(looper->Jets) = *(looper->JetscleanDY);
				if(b_HTJetsMask) *(looper->HTJetsMask) = *(looper->HTJetsMaskcleanDY);
				if(b_MHTJetsMask) *(looper->MHTJetsMask) = *(looper->MHTJetsMaskcleanDY);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->JetscleanDY_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->JetscleanDY_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->JetscleanDY_bDiscriminatorCSV);
				looper->JetID = looper->JetIDcleanDY;
				looper->NJets = looper->NJetscleanDY;
				looper->BTags = looper->BTagscleanDY;
				looper->isoElectronTracks = looper->isoElectronTrackscleanDY;
				looper->isoMuonTracks = looper->isoMuonTrackscleanDY;
				looper->isoPionTracks = looper->isoPionTrackscleanDY;
				looper->HT = looper->HTcleanDY;
				looper->MHT = looper->MHTcleanDY;
				looper->MHT_Phi = looper->MHT_PhicleanDY;
				looper->METPt = looper->METPtcleanDY;
				looper->METPhi = looper->METPhicleanDY;
				looper->DeltaPhi1 = looper->DeltaPhi1cleanDY;
				looper->DeltaPhi2 = looper->DeltaPhi2cleanDY;
				looper->DeltaPhi3 = looper->DeltaPhi3cleanDY;
				looper->DeltaPhi4 = looper->DeltaPhi4cleanDY;
			}
			else if(vtype==cleanGJ){
				if(b_Jets) *(looper->Jets) = *(looper->JetscleanGJ);
				if(b_HTJetsMask) *(looper->HTJetsMask) = *(looper->HTJetsMaskcleanGJ);
				if(b_MHTJetsMask) *(looper->MHTJetsMask) = *(looper->MHTJetsMaskcleanGJ);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->JetscleanGJ_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->JetscleanGJ_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->JetscleanGJ_bDiscriminatorCSV);
				looper->JetID = looper->JetIDcleanGJ;
				looper->NJets = looper->NJetscleanGJ;
				looper->BTags = looper->BTagscleanGJ;
				looper->isoElectronTracks = looper->isoElectronTrackscleanGJ;
				looper->isoMuonTracks = looper->isoMuonTrackscleanGJ;
				looper->isoPionTracks = looper->isoPionTrackscleanGJ;
				looper->HT = looper->HTcleanGJ;
				looper->MHT = looper->MHTcleanGJ;
				looper->MHT_Phi = looper->MHT_PhicleanGJ;
				looper->METPt = looper->METPtcleanGJ;
				looper->METPhi = looper->METPhicleanGJ;
				looper->DeltaPhi1 = looper->DeltaPhi1cleanGJ;
				looper->DeltaPhi2 = looper->DeltaPhi2cleanGJ;
				looper->DeltaPhi3 = looper->DeltaPhi3cleanGJ;
				looper->DeltaPhi4 = looper->DeltaPhi4cleanGJ;
			}
			else if(vtype==cleanGJloose){
				if(b_bestPhoton) *(looper->bestPhoton) = *(looper->bestPhotonLoose);
				if(b_Jets) *(looper->Jets) = *(looper->JetscleanGJloose);
				if(b_HTJetsMask) *(looper->HTJetsMask) = *(looper->HTJetsMaskcleanGJloose);
				if(b_MHTJetsMask) *(looper->MHTJetsMask) = *(looper->MHTJetsMaskcleanGJloose);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->JetscleanGJloose_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->JetscleanGJloose_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->JetscleanGJloose_bDiscriminatorCSV);
				looper->JetID = looper->JetIDcleanGJloose;
				looper->NumPhotons = looper->NumPhotonsLoose;
				looper->NJets = looper->NJetscleanGJloose;
				looper->BTags = looper->BTagscleanGJloose;
				looper->isoElectronTracks = looper->isoElectronTrackscleanGJloose;
				looper->isoMuonTracks = looper->isoMuonTrackscleanGJloose;
				looper->isoPionTracks = looper->isoPionTrackscleanGJloose;
				looper->HT = looper->HTcleanGJloose;
				looper->MHT = looper->MHTcleanGJloose;
				looper->MHT_Phi = looper->MHT_PhicleanGJloose;
				looper->METPt = looper->METPtcleanGJloose;
				looper->METPhi = looper->METPhicleanGJloose;
				looper->DeltaPhi1 = looper->DeltaPhi1cleanGJloose;
				looper->DeltaPhi2 = looper->DeltaPhi2cleanGJloose;
				looper->DeltaPhi3 = looper->DeltaPhi3cleanGJloose;
				looper->DeltaPhi4 = looper->DeltaPhi4cleanGJloose;
			}
		}
		virtual void UndoVariation() {
			//restore original values
			if(b_bestPhoton) *(looper->bestPhoton) = bestPhoton;
			if(b_Jets) *(looper->Jets) = Jets;
			if(b_HTJetsMask) *(looper->HTJetsMask) = HTJetsMask;
			if(b_MHTJetsMask) *(looper->MHTJetsMask) = MHTJetsMask;
			if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = Jets_partonFlavor;
			if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = Jets_hadronFlavor;
			if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = Jets_bDiscriminatorCSV;
			looper->JetID = JetID;
			looper->NumPhotons = NumPhotons;
			looper->NJets = NJets;
			looper->BTags = BTags;
			looper->isoElectronTracks = isoElectronTracks;
			looper->isoMuonTracks = isoMuonTracks;
			looper->isoPionTracks = isoPionTracks;
			looper->HT = HT;
			looper->METPt = METPt;
			looper->METPhi = METPhi;
			looper->MHT = MHT;
			looper->MHT_Phi = MHT_Phi;
			looper->DeltaPhi1 = DeltaPhi1;
			looper->DeltaPhi2 = DeltaPhi2;
			looper->DeltaPhi3 = DeltaPhi3;
			looper->DeltaPhi4 = DeltaPhi4;
		}
		
		//member variables
		bool enabled;
		bool b_bestPhoton,b_Jets,b_HTJetsMask,b_MHTJetsMask,b_Jets_partonFlavor,b_Jets_hadronFlavor,b_Jets_bDiscriminatorCSV;
		vartypes vtype;
		vector<TLorentzVector> bestPhoton;
		vector<TLorentzVector> Jets;
		vector<bool> HTJetsMask;
		vector<bool> MHTJetsMask;
		vector<int> Jets_hadronFlavor;
		vector<int> Jets_partonFlavor;
		vector<double> Jets_bDiscriminatorCSV;
		Bool_t          JetID;
		Int_t           NumPhotons;
		Int_t           NJets;
		Int_t           BTags;
		Int_t           isoElectronTracks;
		Int_t           isoMuonTracks;
		Int_t           isoPionTracks;
		Double_t        HT;
		Double_t        METPt;
		Double_t        METPhi;
		Double_t        MHT;
		Double_t        MHT_Phi;
		Double_t        DeltaPhi1;
		Double_t        DeltaPhi2;
		Double_t        DeltaPhi3;
		Double_t        DeltaPhi4;
};

namespace KParser {
	template <>
	KVariator<KSkimmer>* processVariator<KSkimmer>(KNamed* tmp){
		KVariator<KSkimmer>* vtmp = 0;
		string vname = tmp->first;
		OptionMap* omap = tmp->second;
		
		//check for all known variators
		if(vname=="Jet") vtmp = new KJetVariator(vname,omap);
		else {} //skip unknown variators

		if(!vtmp) cout << "Input error: unknown variator " << vname << ". This variator will be skipped." << endl;

		return vtmp;
	}
}

#endif
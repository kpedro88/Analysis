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
		KJetVariator(string name_, OptionMap* localOpt_) : KVariator<KSkimmer>(name_,localOpt_), vtype(NoVar) {
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
		//functions
		virtual void DoVariation() {
			//store original values
			bestPhoton = *(looper->bestPhoton);
			Jets = *(looper->Jets);
			HTJetsMask = *(looper->HTJetsMask);
			MHTJetsMask = *(looper->MHTJetsMask);
			Jets_partonFlavor = *(looper->Jets_partonFlavor);
			Jets_hadronFlavor = *(looper->Jets_hadronFlavor);
			Jets_bDiscriminatorCSV = *(looper->Jets_bDiscriminatorCSV);
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
				*(looper->Jets) = *(looper->JetsJECup);
				*(looper->HTJetsMask) = *(looper->HTJetsMaskJECup);
				*(looper->MHTJetsMask) = *(looper->MHTJetsMaskJECup);
				*(looper->Jets_partonFlavor) = *(looper->JetsJECup_partonFlavor);
				*(looper->Jets_hadronFlavor) = *(looper->JetsJECup_hadronFlavor);
				*(looper->Jets_bDiscriminatorCSV) = *(looper->JetsJECup_bDiscriminatorCSV);
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
				*(looper->Jets) = *(looper->JetsJECdown);
				*(looper->HTJetsMask) = *(looper->HTJetsMaskJECdown);
				*(looper->MHTJetsMask) = *(looper->MHTJetsMaskJECdown);
				*(looper->Jets_partonFlavor) = *(looper->JetsJECdown_partonFlavor);
				*(looper->Jets_hadronFlavor) = *(looper->JetsJECdown_hadronFlavor);
				*(looper->Jets_bDiscriminatorCSV) = *(looper->JetsJECdown_bDiscriminatorCSV);
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
				*(looper->Jets) = *(looper->Jetsclean);
				*(looper->HTJetsMask) = *(looper->HTJetsMaskclean);
				*(looper->MHTJetsMask) = *(looper->MHTJetsMaskclean);
				*(looper->Jets_partonFlavor) = *(looper->Jetsclean_partonFlavor);
				*(looper->Jets_hadronFlavor) = *(looper->Jetsclean_hadronFlavor);
				*(looper->Jets_bDiscriminatorCSV) = *(looper->Jetsclean_bDiscriminatorCSV);
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
				*(looper->Jets) = *(looper->Jetsclean);
				*(looper->HTJetsMask) = *(looper->HTJetsMaskclean);
				*(looper->MHTJetsMask) = *(looper->MHTJetsMaskclean);
				*(looper->Jets_partonFlavor) = *(looper->Jetsclean_partonFlavor);
				*(looper->Jets_hadronFlavor) = *(looper->Jetsclean_hadronFlavor);
				*(looper->Jets_bDiscriminatorCSV) = *(looper->Jetsclean_bDiscriminatorCSV);
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
				*(looper->Jets) = *(looper->JetscleanDY);
				*(looper->HTJetsMask) = *(looper->HTJetsMaskcleanDY);
				*(looper->MHTJetsMask) = *(looper->MHTJetsMaskcleanDY);
				*(looper->Jets_partonFlavor) = *(looper->JetscleanDY_partonFlavor);
				*(looper->Jets_hadronFlavor) = *(looper->JetscleanDY_hadronFlavor);
				*(looper->Jets_bDiscriminatorCSV) = *(looper->JetscleanDY_bDiscriminatorCSV);
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
				*(looper->Jets) = *(looper->JetscleanGJ);
				*(looper->HTJetsMask) = *(looper->HTJetsMaskcleanGJ);
				*(looper->MHTJetsMask) = *(looper->MHTJetsMaskcleanGJ);
				*(looper->Jets_partonFlavor) = *(looper->JetscleanGJ_partonFlavor);
				*(looper->Jets_hadronFlavor) = *(looper->JetscleanGJ_hadronFlavor);
				*(looper->Jets_bDiscriminatorCSV) = *(looper->JetscleanGJ_bDiscriminatorCSV);
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
				*(looper->Jets) = *(looper->JetscleanGJloose);
				*(looper->HTJetsMask) = *(looper->HTJetsMaskcleanGJloose);
				*(looper->MHTJetsMask) = *(looper->MHTJetsMaskcleanGJloose);
				*(looper->Jets_partonFlavor) = *(looper->JetscleanGJloose_partonFlavor);
				*(looper->Jets_hadronFlavor) = *(looper->JetscleanGJloose_hadronFlavor);
				*(looper->Jets_bDiscriminatorCSV) = *(looper->JetscleanGJloose_bDiscriminatorCSV);
				looper->JetID = looper->JetIDcleanGJloose;
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
			*(looper->bestPhoton) = bestPhoton;
			*(looper->Jets) = Jets;
			*(looper->HTJetsMask) = HTJetsMask;
			*(looper->MHTJetsMask) = MHTJetsMask;
			*(looper->Jets_partonFlavor) = Jets_partonFlavor;
			*(looper->Jets_hadronFlavor) = Jets_hadronFlavor;
			*(looper->Jets_bDiscriminatorCSV) = Jets_bDiscriminatorCSV;
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
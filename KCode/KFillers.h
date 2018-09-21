#ifndef KFILLERS_H
#define KFILLERS_H

//custom headers
#include "KMap.h"
#include "KMath.h"
#include "KHisto.h"
#include "KBuilderSelectors.h"

//ROOT headers
#include "TMath.h"
#include "TLorentzVector.h"

//STL headers
#include <cmath>

using namespace std;

//plot yield vs. bin of RA2 search -> depends on RA2Bin selector
class KFiller_RA2bin : public KFiller {
	public:
		//inherit constructors
		using KFiller::KFiller;
		virtual void CheckDeps(){
			RA2Bin = sel->Get<KRA2BinSelector*>("RA2Bin");
			BTagSF = sel->Get<KBTagSFSelector*>("BTagSF");
			bool DoBTagSF = khtmp->MCWeight ? khtmp->MCWeight->btagcorr : false;
			if(!DoBTagSF) BTagSF = NULL;
		}
		virtual void CheckBranches(){
			if(RA2Bin && RA2Bin->debug){
				looper->fChain->SetBranchStatus("RunNum",1);
				looper->fChain->SetBranchStatus("LumiBlockNum",1);
				looper->fChain->SetBranchStatus("EvtNum",1);
			}
		}
		virtual void Fill(KValue& value, double w) {
			if(!RA2Bin) return;
			if(RA2Bin->RA2Exclusive) {
				if(RA2Bin->debug==1){
					cout << "Run = " << looper->RunNum << ", LS = " << looper->LumiBlockNum << ", Evt = " << looper->EvtNum;
					for(unsigned q = 0; q < RA2Bin->RA2VarNames.size(); ++q){
						if(RA2Bin->RA2VarNames[q]=="NJets") cout << ", NJets = " << looper->NJets;
						else if(RA2Bin->RA2VarNames[q]=="BTags") cout << ", BTags = " << looper->BTags;
						else if(RA2Bin->RA2VarNames[q]=="MHT") cout << ", MHT = " << looper->MHT;
						else if(RA2Bin->RA2VarNames[q]=="HT") cout << ", HT = " << looper->HT;
					}
					cout << endl;
				}
				else if(RA2Bin->debug==2){
					//RA2bin number starts at 1
					cout << looper->RunNum << "\t" << looper->LumiBlockNum << "\t" << looper->EvtNum << "\t" << RA2Bin->RA2bins[0] << "\t" << RA2Bin->labels[RA2Bin->RA2bins[0]-1] << endl;
				}
				value.Fill(RA2Bin->RA2bins[0],w);
			}
			else {
				for(unsigned b = 0; b < RA2Bin->RA2bins.size(); b++){
					double wb = w;
					//weight by btag scale factor probability if available
					if(BTagSF) {
						int nb = RA2Bin->GetBin("BTags",RA2Bin->RA2binVec[b]);
						if(nb>=0 && ((unsigned)nb)<BTagSF->prob.size()) wb *= BTagSF->prob[nb];
						else wb = 0; //btag sf failed
					}
					value.Fill(RA2Bin->RA2bins[b],wb);
				}
			}
		}
		//member variables
		KRA2BinSelector* RA2Bin = NULL;
		KBTagSFSelector* BTagSF = NULL;
};
REGISTER_FILLER(RA2bin);

//jet multiplicity
class KFiller_njets : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"NJets"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->NJets,w); }
};
REGISTER_FILLER(njets);

//ISR jet multiplicity
class KFiller_njetsisr : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"NJetsISR"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->NJetsISR,w); }
};
REGISTER_FILLER(njetsisr);

//b-jet multiplicity
class KFiller_nbjets : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void CheckDeps(){
			BTagSF = sel->Get<KBTagSFSelector*>("BTagSF");
			bool DoBTagSF = khtmp->MCWeight ? khtmp->MCWeight->btagcorr : false;
			if(!DoBTagSF) BTagSF = NULL;
		}
		virtual void CheckBranches(){
			if(!BTagSF) looper->fChain->SetBranchStatus("BTags",1);
		}
		virtual void Fill(KValue& value, double w) {
			if(BTagSF){
				for(unsigned b = 0; b < BTagSF->prob.size(); b++){
					//weight by btag scale factor probability if available
					double wb = w*BTagSF->prob[b];
					value.Fill(b,wb);
				}
			}
			else value.Fill(looper->BTags,w);
		}
		//member variables
		KBTagSFSelector* BTagSF = NULL;
};
REGISTER_FILLER(nbjets);

//-----------------------------------------------------------------------------
//for hemispheres
class KFillerHemi : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void CheckDeps(){ Hemisphere = sel->Get<KHemisphereSelector*>("Hemisphere"); }
		virtual void Fill(KValue& value, double w) { if(Hemisphere) Fill_(value,w); }
		virtual void Fill_(KValue& value, double w) {}
		//member variables
		KHemisphereSelector* Hemisphere = NULL;
};

//jet multiplicity
class KFiller_njetshemi : public KFillerHemi {
	public:
		using KFillerHemi::KFillerHemi;
		virtual void Fill_(KValue& value, double w) { value.Fill(Hemisphere->NJets,w); }
};
REGISTER_FILLER(njetshemi);

//b-jet multiplicity
class KFiller_nbjetshemi : public KFillerHemi {
	public:
		using KFillerHemi::KFillerHemi;
		virtual void Fill_(KValue& value, double w) { value.Fill(Hemisphere->BTags,w); }
};
REGISTER_FILLER(nbjetshemi);

//jet multiplicity
class KFiller_njetsopphemi : public KFillerHemi {
	public:
		using KFillerHemi::KFillerHemi;
		virtual void Fill_(KValue& value, double w) { value.Fill(Hemisphere->NJetsOpp,w); }
};
REGISTER_FILLER(njetsopphemi);

//b-jet multiplicity
class KFiller_nbjetsopphemi : public KFillerHemi {
	public:
		using KFillerHemi::KFillerHemi;
		virtual void Fill_(KValue& value, double w) { value.Fill(Hemisphere->BTagsOpp,w); }
};
REGISTER_FILLER(nbjetsopphemi);

//-----------------------------------------------------------------------------
//sum of jet pt
class KFiller_ht : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"HT"}; }		
		virtual void Fill(KValue& value, double w) { value.Fill(looper->HT,w); }
};
REGISTER_FILLER(ht);

//missing hadronic energy
class KFiller_mht : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"MHT"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->MHT,w); }
};
REGISTER_FILLER(mht);

//missing hadronic energy
class KFiller_genmht : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"GenMHT"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->GenMHT,w); }
};
REGISTER_FILLER(genmht);

//missing energy
class KFiller_met : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"MET"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->MET,w); }
};
REGISTER_FILLER(met);

//# leptons (mu or ele)
class KFiller_nleptons : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"NMuons","NElectrons"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->NMuons+looper->NElectrons,w); }
};
REGISTER_FILLER(nleptons);

//# electrons
class KFiller_nelectrons : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"NElectrons"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->NElectrons,w); }
};
REGISTER_FILLER(nelectrons);

//# muons
class KFiller_nmuons : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"NMuons"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->NMuons,w); }
};
REGISTER_FILLER(nmuons);

//# iso tracks
class KFiller_nisotrack : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"isoElectronTracks","isoMuonTracks","isoPionTracks"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->isoElectronTracks+looper->isoMuonTracks+looper->isoPionTracks,w); }
};
REGISTER_FILLER(nisotrack);

//# good vertices
class KFiller_nvertex : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"NVtx"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->NVtx,w); }
};
REGISTER_FILLER(nvertex);

//# interactions
class KFiller_numint : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"NumInteractions"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->NumInteractions,w); }
};
REGISTER_FILLER(numint);

//madgraph HT
class KFiller_madht : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"madHT"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->madHT,w); }
};
REGISTER_FILLER(madht);

//pT of leading jet
class KFiller_leadjetpt : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"Jets"}; }
		virtual void Fill(KValue& value, double w) { if(looper->Jets->size()>0) value.Fill(looper->Jets->at(0).Pt(),w); }
};
REGISTER_FILLER(leadjetpt);

//ratio of MHT & pT of leading jet
class KFiller_mhtleadjetptratio : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"Jets","MHT"}; }
		virtual void Fill(KValue& value, double w) { if(looper->Jets->size()>0) value.Fill(looper->MHT/looper->Jets->at(0).Pt(),w); }
};
REGISTER_FILLER(mhtleadjetptratio);

//ratio of MET & pT of leading jet
class KFiller_metleadjetptratio : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"Jets","MET"}; }
		virtual void Fill(KValue& value, double w) { if(looper->Jets->size()>0) value.Fill(looper->MET/looper->Jets->at(0).Pt(),w); }
};
REGISTER_FILLER(metleadjetptratio);

//pT of leading jet w/ hadronFlavor==5, |eta|<2.4
class KFiller_leadbhadronjetpt : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"Jets","Jets_hadronFlavor"}; }
		virtual void Fill(KValue& value, double w) {
			for(unsigned j = 0; j < looper->Jets->size(); ++j){
				if(abs(looper->Jets_hadronFlavor->at(j))==5 && fabs(looper->Jets->at(j).Eta())<2.4){
					value.Fill(looper->Jets->at(j).Pt(),w);
					break;
				}
			}
		}
};
REGISTER_FILLER(leadbhadronjetpt);

//deltaphi of leading jet
class KFiller_deltaphi1 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"DeltaPhi1"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->DeltaPhi1,w); }
};
REGISTER_FILLER(deltaphi1);

//deltaphi of 2nd jet
class KFiller_deltaphi2 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"DeltaPhi2"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->DeltaPhi2,w); }
};
REGISTER_FILLER(deltaphi2);

//deltaphi of 3rd jet
class KFiller_deltaphi3 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"DeltaPhi3"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->DeltaPhi3,w); }
};
REGISTER_FILLER(deltaphi3);

//deltaphi of 4th jet
class KFiller_deltaphi4 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"DeltaPhi4"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->DeltaPhi4,w); }
};
REGISTER_FILLER(deltaphi4);

//difference between mMother and mLSP
class KFiller_deltam : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void CheckBase(){ deltaM = 0; base->GetLocalOpt()->Get("deltaM",deltaM); }
		virtual void Fill(KValue& value, double w) { value.Fill(deltaM,w); }
		//member variables
		double deltaM;
};
REGISTER_FILLER(deltam);

//pT of each mother particle
class KFiller_motherpt : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"GenParticles","GenParticles_PdgId"}; }
		virtual void CheckBase(){ base->GetLocalOpt()->Get("mother",mother); }
		virtual void Fill(KValue& value, double w) {
			//loop over genparticles
			for(unsigned g = 0; g < looper->GenParticles_PdgId->size(); ++g){
				if(binary_search(mother.begin(),mother.end(),abs(looper->GenParticles_PdgId->at(g)))){
					value.Fill(looper->GenParticles->at(g).Pt(),w);
				}
			}
		}
		//member variables
		vector<int> mother;
};
REGISTER_FILLER(motherpt);

//pT of mother particle system recoiling against ISR jets
class KFiller_recoil : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"GenParticles","GenParticles_PdgId"}; }
		virtual void CheckBase(){ base->GetLocalOpt()->Get("mother",mother); }
		virtual void Fill(KValue& value, double w) {
			//loop over genparticles
			TLorentzVector vgen;
			vgen.SetPtEtaPhiE(0,0,0,0);
			for(unsigned g = 0; g < looper->GenParticles_PdgId->size(); ++g){
				if(binary_search(mother.begin(),mother.end(),abs(looper->GenParticles_PdgId->at(g)))){
					vgen += looper->GenParticles->at(g);
				}
			}
			value.Fill(vgen.Pt(),w);
		}
		//member variables
		vector<int> mother;
};
REGISTER_FILLER(recoil);

//delta phi of mother particle system with MHT
class KFiller_deltaphirecoil : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"GenParticles","GenParticles_PdgId","MHTPhi"}; }
		virtual void CheckBase(){ base->GetLocalOpt()->Get("mother",mother); }
		virtual void Fill(KValue& value, double w) {
			//loop over genparticles
			TLorentzVector vgen;
			vgen.SetPtEtaPhiE(0,0,0,0);
			for(unsigned g = 0; g < looper->GenParticles_PdgId->size(); ++g){
				if(binary_search(mother.begin(),mother.end(),abs(looper->GenParticles_PdgId->at(g)))){
					vgen += looper->GenParticles->at(g);
				}
			}
			value.Fill(KMath::DeltaPhi(vgen.Phi(),looper->MHTPhi),w);
		}
		//member variables
		vector<int> mother;
};
REGISTER_FILLER(deltaphirecoil);

//sigma ieta ieta variable for all photon candidates
class KFiller_sigmaietaieta : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void CheckDeps(){ PhotonID = sel->Get<KPhotonIDSelector*>("PhotonID"); }
		virtual void ListBranches() { branches = {"Photons_sigmaIetaIeta"}; }
		virtual void Fill(KValue& value, double w) {
			if(PhotonID){
				for(unsigned p = 0; p < PhotonID->goodPhotons.size(); ++p){
					value.Fill(looper->Photons_sigmaIetaIeta->at(PhotonID->goodPhotons[p]),w);
				}
			}
			else { //if no ID applied, just plot everything
				for(unsigned p = 0; p < looper->Photons_sigmaIetaIeta->size(); ++p){
					value.Fill(looper->Photons_sigmaIetaIeta->at(p),w);
				}
			}
		}
		//member variables
		KPhotonIDSelector* PhotonID = NULL;
};
REGISTER_FILLER(sigmaietaieta);

//sigma ieta ieta variable for best photon
class KFiller_bestsigmaietaieta : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"Photons","Photons_sigmaIetaIeta"}; }
		virtual void Fill(KValue& value, double w) {
			double best_sieie=0;
			double best_pt=0;
			for(unsigned p = 0; p < looper->Photons->size(); ++p){
				if(looper->Photons->at(p).Pt()>best_pt){
					best_pt = looper->Photons->at(p).Pt();
					best_sieie = looper->Photons_sigmaIetaIeta->at(p);
				}
			}
			value.Fill(best_sieie,w);
		}
};
REGISTER_FILLER(bestsigmaietaieta);

//RMS of PDF weights
class KFiller_pdfrms : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"PDFweights"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(TMath::RMS(looper->PDFweights->begin(),looper->PDFweights->end()),w); }
};
REGISTER_FILLER(pdfrms);

//all PDF weights
class KFiller_pdfweight : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"PDFweights"}; }
		virtual void Fill(KValue& value, double w) {
			for(unsigned w = 0; w < looper->PDFweights->size(); ++w){
				value.Fill(looper->PDFweights->at(w),w);
			}
		}
};
REGISTER_FILLER(pdfweight);

//-----------------------------------------------------------------------------
//jet ID quantities... (w/ optional eta region specification)
class KFillerJetID : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void CheckDeps(){ JetEtaRegion = sel->Get<KJetEtaRegionSelector*>("JetEtaRegion"); }
		virtual void ListBranches() { branches = {"Jets"}; ListBranches_(); }
		virtual void ListBranches_() {}
		virtual void Fill(KValue& value, double w) {
			for(unsigned j = 0; j < looper->Jets->size(); ++j){
				if(!JetEtaRegion || JetEtaRegion->mask[j]) Fill_(value,w,j);
			}
		}
		virtual void Fill_(KValue& value, double w, unsigned j) {}
		//member variables
		KJetEtaRegionSelector* JetEtaRegion = NULL;
};

class KFiller_neufrac : public KFillerJetID {
	public:
		using KFillerJetID::KFillerJetID;
		virtual void ListBranches_() { branches.push_back("Jets_neutralHadronEnergyFraction"); }
		virtual void Fill_(KValue& value, double w, unsigned j) { value.Fill(looper->Jets_neutralHadronEnergyFraction->at(j),w); }
};
REGISTER_FILLER(neufrac);

class KFiller_phofrac : public KFillerJetID {
	public:
		using KFillerJetID::KFillerJetID;
		virtual void ListBranches_() { branches.push_back("Jets_photonEnergyFraction"); }
		virtual void Fill_(KValue& value, double w, unsigned j) { value.Fill(looper->Jets_photonEnergyFraction->at(j),w); }
};
REGISTER_FILLER(phofrac);

class KFiller_chgfrac : public KFillerJetID {
	public:
		using KFillerJetID::KFillerJetID;
		virtual void ListBranches_() { branches.push_back("Jets_chargedHadronEnergyFraction"); }
		virtual void Fill_(KValue& value, double w, unsigned j) { value.Fill(looper->Jets_chargedHadronEnergyFraction->at(j),w); }
};
REGISTER_FILLER(chgfrac);

class KFiller_chgemfrac : public KFillerJetID {
	public:
		using KFillerJetID::KFillerJetID;
		virtual void ListBranches_() { branches.push_back("Jets_chargedEmEnergyFraction"); }
		virtual void Fill_(KValue& value, double w, unsigned j) { value.Fill(looper->Jets_chargedEmEnergyFraction->at(j),w); }
};
REGISTER_FILLER(chgemfrac);

class KFiller_chgmulti : public KFillerJetID {
	public:
		using KFillerJetID::KFillerJetID;
		virtual void ListBranches_() { branches.push_back("Jets_chargedMultiplicity"); }
		virtual void Fill_(KValue& value, double w, unsigned j) { value.Fill(looper->Jets_chargedMultiplicity->at(j),w); }
};
REGISTER_FILLER(chgmulti);

class KFiller_neumulti : public KFillerJetID {
	public:
		using KFillerJetID::KFillerJetID;
		virtual void ListBranches_() { branches.push_back("Jets_neutralMultiplicity"); }
		virtual void Fill_(KValue& value, double w, unsigned j) { value.Fill(looper->Jets_neutralMultiplicity->at(j),w); }
};
REGISTER_FILLER(neumulti);

class KFiller_nconstit : public KFillerJetID {
	public:
		using KFillerJetID::KFillerJetID;
		virtual void ListBranches_() { branches.push_back("Jets_chargedMultiplicity"); branches.push_back("Jets_neutralMultiplicity"); }
		virtual void Fill_(KValue& value, double w, unsigned j) { value.Fill(looper->Jets_chargedMultiplicity->at(j)+looper->Jets_neutralMultiplicity->at(j),w); }
};
REGISTER_FILLER(nconstit);

//-----------------------------------------------------------------------------
//transverse mass
class KFiller_MTAK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"MT_AK8"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->MT_AK8,w); }
};
REGISTER_FILLER(MTAK8);

//dijet mass
class KFiller_MJJAK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"MJJ_AK8"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->MJJ_AK8,w); }
};
REGISTER_FILLER(MJJAK8);

//transverse mass from softdrop
class KFiller_MTSDAK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"JetsAK8_subjets","MET","METPhi"}; }	
		virtual void Fill(KValue& value, double w) {
			TLorentzVector vjjjj;
			if(looper->JetsAK8_subjets->size()>1){
				for(unsigned j = 0; j < 2; ++j){
					for(const auto& subjet: looper->JetsAK8_subjets->at(j)){
						vjjjj += subjet;
					}
				}
				double MT = KMath::TransverseMass(vjjjj.Px(),vjjjj.Py(),vjjjj.M(),looper->MET*cos(looper->METPhi),looper->MET*sin(looper->METPhi),0);
				value.Fill(MT,w);
			}		
		}
};
REGISTER_FILLER(MTSDAK8);

//dijet+truth mass
class KFiller_MmcAK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"Mmc_AK8"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->Mmc_AK8,w); }
};
REGISTER_FILLER(MmcAK8);

//trijet mass
class KFiller_MJJJAK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"JetsAK8"}; }
		virtual void Fill(KValue& value, double w) {
			TLorentzVector vjjj;
			for(unsigned j = 0; j < min(looper->JetsAK8->size(),3ul); ++j){
				vjjj += looper->JetsAK8->at(j);
			}
			value.Fill(vjjj.M(),w);
		}
};
REGISTER_FILLER(MJJJAK8);

//trijet mT
class KFiller_MTJJJAK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"JetsAK8","MET","METPhi"}; }
		virtual void Fill(KValue& value, double w) {
			TLorentzVector vjjj;
			for(unsigned j = 0; j < min(looper->JetsAK8->size(),3ul); ++j){
				vjjj += looper->JetsAK8->at(j);
			}
			double MT = KMath::TransverseMass(vjjj.Px(),vjjj.Py(),vjjj.M(),looper->MET*cos(looper->METPhi),looper->MET*sin(looper->METPhi),0);
			value.Fill(MT,w);
		}
};
REGISTER_FILLER(MTJJJAK8);

//deta(j1,j2)
class KFiller_deltaetaAK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"JetsAK8"}; }
		virtual void Fill(KValue& value, double w) { if(looper->JetsAK8->size()>1) value.Fill(abs(looper->JetsAK8->at(0).Eta()-looper->JetsAK8->at(1).Eta()),w); }
};
REGISTER_FILLER(deltaetaAK8);

//dphi(j1,MET)
class KFiller_deltaphi1AK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"DeltaPhi1_AK8"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->DeltaPhi1_AK8,w); }
};
REGISTER_FILLER(deltaphi1AK8);

//dphi(j2,MET)
class KFiller_deltaphi2AK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"DeltaPhi2_AK8"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->DeltaPhi2_AK8,w); }
};
REGISTER_FILLER(deltaphi2AK8);

//min dphi(j1/2,MET)
class KFiller_deltaphiminAK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"DeltaPhiMin_AK8"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->DeltaPhiMin_AK8,w); }
};
REGISTER_FILLER(deltaphiminAK8);

//dphi(jj,MET)
class KFiller_deltaphijjmetAK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"METPhi","JetsAK8"}; }
		virtual void Fill(KValue& value, double w) {
			TLorentzVector vjj;
			if(looper->JetsAK8->size()>1){
				for(unsigned j = 0; j < 2; ++j){
					vjj += looper->JetsAK8->at(j);
				}
				double dphi = abs(KMath::DeltaPhi(vjj.Phi(),looper->METPhi));
				value.Fill(dphi,w);
			}
		}
};
REGISTER_FILLER(deltaphijjmetAK8);

//dR(j1,j2)
class KFiller_dRAK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"JetsAK8"}; }
		virtual void Fill(KValue& value, double w) { if(looper->JetsAK8->size()>1) value.Fill(abs(looper->JetsAK8->at(0).DeltaR(looper->JetsAK8->at(1))),w); }
};
REGISTER_FILLER(dRAK8);

//AK8 dijet pT asymmetry
class KFiller_ptAsymAK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"JetsAK8"}; }
		virtual void Fill(KValue& value, double w) { if(looper->JetsAK8->size()>1) value.Fill((looper->JetsAK8->at(0).Pt()-looper->JetsAK8->at(1).Pt())/(looper->JetsAK8->at(0).Pt()+looper->JetsAK8->at(1).Pt()),w); }
};
REGISTER_FILLER(ptAsymAK8);

//AK8 dijet m_sd asymmetry
class KFiller_msdAsymAK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"JetsAK8_softDropMass"}; }
		virtual void Fill(KValue& value, double w) {
			if(looper->JetsAK8_softDropMass->size()>1) value.Fill(abs(looper->JetsAK8_softDropMass->at(0)-looper->JetsAK8_softDropMass->at(1))/(looper->JetsAK8_softDropMass->at(0)+looper->JetsAK8_softDropMass->at(1)),w);
		}
};
REGISTER_FILLER(msdAsymAK8);

//HT from AK8 jets
class KFiller_htAK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"JetsAK8"}; }
		virtual void Fill(KValue& value, double w) {
			double htak8 = 0.0;
			for(const auto& jet : *(looper->JetsAK8)){
				if(abs(jet.Eta())<2.4) htak8 += jet.Pt();
			}
			value.Fill(htak8,w);		
		}
};
REGISTER_FILLER(htAK8);

//number of AK8 jets
class KFiller_njetsAK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"JetsAK8"}; }
		virtual void Fill(KValue& value, double w) {
			int num = 0;
			for(const auto& jet : *(looper->JetsAK8)){
				if(abs(jet.Eta())<2.4) ++num;
			}
			value.Fill(num,w);	
		}
};
REGISTER_FILLER(njetsAK8);

//MET/MT
class KFiller_metMTratio : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"MET","MT_AK8"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->MT_AK8>0 ? looper->MET/looper->MT_AK8 : 0.0,w); }
};
REGISTER_FILLER(metMTratio);

//MET significance
class KFiller_metsig : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"METSignificance"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->METSignificance,w); }
};
REGISTER_FILLER(metsig);

//log(MET significance)
class KFiller_logmetsig : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"METSignificance"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(log(looper->METSignificance),w); }
};
REGISTER_FILLER(logmetsig);

//gen MET
class KFiller_genmet : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"GenMET"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->GenMET,w); }
};
REGISTER_FILLER(genmet);

//event weight (always 1 per event)
class KFiller_eventweight : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void Fill(KValue& value, double w) { value.Fill(w); }
};
REGISTER_FILLER(eventweight);

//log event weight (always 1 per event)
class KFiller_logeventweight : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void Fill(KValue& value, double w) { value.Fill(log(abs(w))); }
};
REGISTER_FILLER(logeventweight);

//HT/MT
class KFiller_htMTratio : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"HT","MT_AK8"}; }
		virtual void Fill(KValue& value, double w) { value.Fill(looper->MT_AK8>0 ? looper->HT/looper->MT_AK8 : 0.0,w); }
};
REGISTER_FILLER(htMTratio);

//sum pt of leading 2 jets
class KFiller_ptdijetAK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"JetsAK8"}; }
		virtual void Fill(KValue& value, double w) {
			double pt = 0.0;
			for(unsigned j = 0; j < min(looper->JetsAK8->size(),2ul); ++j){
				pt += looper->JetsAK8->at(j).Pt();
			}
			value.Fill(pt,w);
		}
};
REGISTER_FILLER(ptdijetAK8);

//ptdijetAK8/MT
class KFiller_ptMTratio : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"JetsAK8","MT_AK8"}; }
		virtual void Fill(KValue& value, double w) {
			double pt = 0.0;
			for(unsigned j = 0; j < min(looper->JetsAK8->size(),2ul); ++j){
				pt += looper->JetsAK8->at(j).Pt();
			}
			value.Fill(looper->MT_AK8>0 ? pt/looper->MT_AK8 : 0.0, w);
		}
};
REGISTER_FILLER(ptMTratio);

//num b hadrons (leading 2 jets)
class KFiller_nbdijetAK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"JetsAK8_NumBhadrons"}; }
		virtual void Fill(KValue& value, double w) {
			int num = 0;
			for(unsigned j = 0; j < min(looper->JetsAK8_NumBhadrons->size(),2ul); ++j){
				num += looper->JetsAK8_NumBhadrons->at(j);
			}
			value.Fill(num,w);
		}
};
REGISTER_FILLER(nbdijetAK8);

//num c hadrons (leading 2 jets)
class KFiller_ncdijetAK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void ListBranches() { branches = {"JetsAK8_NumChadrons"}; }
		virtual void Fill(KValue& value, double w) {
			int num = 0;
			for(unsigned j = 0; j < min(looper->JetsAK8_NumChadrons->size(),2ul); ++j){
				num += looper->JetsAK8_NumChadrons->at(j);
			}
			value.Fill(num,w);
		}
};
REGISTER_FILLER(ncdijetAK8);

//-----------------------------------------------------------------------------
//event shape quantities (from AK4 jets)
class KFillerEventShapeAK4 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void CheckDeps(){ EventShapeAK4 = sel->Get<KEventShapeSelector*>("EventShapeAK4"); }		
		virtual void Fill(KValue& value, double w) { if(EventShapeAK4) Fill_(value,w); }
		virtual void Fill_(KValue& value, double w) {}
		//member variables
		KEventShapeSelector* EventShapeAK4 = NULL;
};

class KFiller_boost : public KFillerEventShapeAK4 {
	public:
		using KFillerEventShapeAK4::KFillerEventShapeAK4;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK4->boost,w); }
};
REGISTER_FILLER(boost);

class KFiller_absboost : public KFillerEventShapeAK4 {
	public:
		using KFillerEventShapeAK4::KFillerEventShapeAK4;
		virtual void Fill_(KValue& value, double w) { value.Fill(abs(EventShapeAK4->boost),w); }
};
REGISTER_FILLER(absboost);

class KFiller_sphericity : public KFillerEventShapeAK4 {
	public:
		using KFillerEventShapeAK4::KFillerEventShapeAK4;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK4->sphericity,w); }
};
REGISTER_FILLER(sphericity);

class KFiller_aplanarity : public KFillerEventShapeAK4 {
	public:
		using KFillerEventShapeAK4::KFillerEventShapeAK4;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK4->aplanarity,w); }
};
REGISTER_FILLER(aplanarity);

class KFiller_esvC : public KFillerEventShapeAK4 {
	public:
		using KFillerEventShapeAK4::KFillerEventShapeAK4;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK4->esvC,w); }
};
REGISTER_FILLER(esvC);

class KFiller_esvD : public KFillerEventShapeAK4 {
	public:
		using KFillerEventShapeAK4::KFillerEventShapeAK4;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK4->esvD,w); }
};
REGISTER_FILLER(esvD);

class KFiller_lambda1 : public KFillerEventShapeAK4 {
	public:
		using KFillerEventShapeAK4::KFillerEventShapeAK4;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK4->lambda1,w); }
};
REGISTER_FILLER(lambda1);

class KFiller_lambda2 : public KFillerEventShapeAK4 {
	public:
		using KFillerEventShapeAK4::KFillerEventShapeAK4;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK4->lambda2,w); }
};
REGISTER_FILLER(lambda2);

class KFiller_lambda3 : public KFillerEventShapeAK4 {
	public:
		using KFillerEventShapeAK4::KFillerEventShapeAK4;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK4->lambda3,w); }
};
REGISTER_FILLER(lambda3);

class KFiller_fwm1 : public KFillerEventShapeAK4 {
	public:
		using KFillerEventShapeAK4::KFillerEventShapeAK4;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK4->fwm1,w); }
};
REGISTER_FILLER(fwm1);

class KFiller_fwm2 : public KFillerEventShapeAK4 {
	public:
		using KFillerEventShapeAK4::KFillerEventShapeAK4;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK4->fwm2,w); }
};
REGISTER_FILLER(fwm2);

class KFiller_fwm3 : public KFillerEventShapeAK4 {
	public:
		using KFillerEventShapeAK4::KFillerEventShapeAK4;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK4->fwm3,w); }
};
REGISTER_FILLER(fwm3);

class KFiller_fwm4 : public KFillerEventShapeAK4 {
	public:
		using KFillerEventShapeAK4::KFillerEventShapeAK4;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK4->fwm4,w); }
};
REGISTER_FILLER(fwm4);

class KFiller_fwm5 : public KFillerEventShapeAK4 {
	public:
		using KFillerEventShapeAK4::KFillerEventShapeAK4;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK4->fwm5,w); }
};
REGISTER_FILLER(fwm5);

class KFiller_fwm6 : public KFillerEventShapeAK4 {
	public:
		using KFillerEventShapeAK4::KFillerEventShapeAK4;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK4->fwm6,w); }
};
REGISTER_FILLER(fwm6);

//-----------------------------------------------------------------------------
//event shape quantities (from AK8 jets)
class KFillerEventShapeAK8 : public KFiller {
	public:
		using KFiller::KFiller;
		virtual void CheckDeps(){ EventShapeAK8 = sel->Get<KEventShapeSelector*>("EventShapeAK8"); }
		virtual void Fill(KValue& value, double w) { if(EventShapeAK8) Fill_(value,w); }
		virtual void Fill_(KValue& value, double w) {}
		//member variables
		KEventShapeSelector* EventShapeAK8 = NULL;
};

class KFiller_boostAK8 : public KFillerEventShapeAK8 {
	public:
		using KFillerEventShapeAK8::KFillerEventShapeAK8;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK8->boost,w); }
};
REGISTER_FILLER(boostAK8);

class KFiller_absboostAK8 : public KFillerEventShapeAK8 {
	public:
		using KFillerEventShapeAK8::KFillerEventShapeAK8;
		virtual void Fill_(KValue& value, double w) { value.Fill(abs(EventShapeAK8->boost),w); }
};
REGISTER_FILLER(absboostAK8);

class KFiller_sphericityAK8 : public KFillerEventShapeAK8 {
	public:
		using KFillerEventShapeAK8::KFillerEventShapeAK8;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK8->sphericity,w); }
};
REGISTER_FILLER(sphericityAK8);

class KFiller_aplanarityAK8 : public KFillerEventShapeAK8 {
	public:
		using KFillerEventShapeAK8::KFillerEventShapeAK8;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK8->aplanarity,w); }
};
REGISTER_FILLER(aplanarityAK8);

class KFiller_esvCAK8 : public KFillerEventShapeAK8 {
	public:
		using KFillerEventShapeAK8::KFillerEventShapeAK8;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK8->esvC,w); }
};
REGISTER_FILLER(esvCAK8);

class KFiller_esvDAK8 : public KFillerEventShapeAK8 {
	public:
		using KFillerEventShapeAK8::KFillerEventShapeAK8;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK8->esvD,w); }
};
REGISTER_FILLER(esvDAK8);

class KFiller_lambda1AK8 : public KFillerEventShapeAK8 {
	public:
		using KFillerEventShapeAK8::KFillerEventShapeAK8;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK8->lambda1,w); }
};
REGISTER_FILLER(lambda1AK8);

class KFiller_lambda2AK8 : public KFillerEventShapeAK8 {
	public:
		using KFillerEventShapeAK8::KFillerEventShapeAK8;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK8->lambda2,w); }
};
REGISTER_FILLER(lambda2AK8);

class KFiller_lambda3AK8 : public KFillerEventShapeAK8 {
	public:
		using KFillerEventShapeAK8::KFillerEventShapeAK8;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK8->lambda3,w); }
};
REGISTER_FILLER(lambda3AK8);

class KFiller_fwm1AK8 : public KFillerEventShapeAK8 {
	public:
		using KFillerEventShapeAK8::KFillerEventShapeAK8;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK8->fwm1,w); }
};
REGISTER_FILLER(fwm1AK8);

class KFiller_fwm2AK8 : public KFillerEventShapeAK8 {
	public:
		using KFillerEventShapeAK8::KFillerEventShapeAK8;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK8->fwm2,w); }
};
REGISTER_FILLER(fwm2AK8);

class KFiller_fwm3AK8 : public KFillerEventShapeAK8 {
	public:
		using KFillerEventShapeAK8::KFillerEventShapeAK8;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK8->fwm3,w); }
};
REGISTER_FILLER(fwm3AK8);

class KFiller_fwm4AK8 : public KFillerEventShapeAK8 {
	public:
		using KFillerEventShapeAK8::KFillerEventShapeAK8;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK8->fwm4,w); }
};
REGISTER_FILLER(fwm4AK8);

class KFiller_fwm5AK8 : public KFillerEventShapeAK8 {
	public:
		using KFillerEventShapeAK8::KFillerEventShapeAK8;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK8->fwm5,w); }
};
REGISTER_FILLER(fwm5AK8);

class KFiller_fwm6AK8 : public KFillerEventShapeAK8 {
	public:
		using KFillerEventShapeAK8::KFillerEventShapeAK8;
		virtual void Fill_(KValue& value, double w) { value.Fill(EventShapeAK8->fwm6,w); }
};
REGISTER_FILLER(fwm6AK8);

//-----------------------------------------------------------------------------
//per-jet quantities
class KJetFiller_pt : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8->size()>index) value.Fill(looper->JetsAK8->at(index).Pt(),w); }
};
REGISTER_JETFILLER(pt);

class KJetFiller_genpt : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"GenJetsAK8"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->GenJetsAK8->size()>index and looper->GenJetsAK8->at(index).Pt()>150) value.Fill(looper->GenJetsAK8->at(index).Pt(),w); }
};
REGISTER_JETFILLER(genpt);

class KJetFiller_eta : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8->size()>index) value.Fill(looper->JetsAK8->at(index).Eta(),w); }
};
REGISTER_JETFILLER(eta);

class KJetFiller_abseta : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8->size()>index) value.Fill(abs(looper->JetsAK8->at(index).Eta()),w); }
};
REGISTER_JETFILLER(abseta);

//num b hadrons
class KJetFiller_nb : public KJetFiller {
    public:
        using KJetFiller::KJetFiller;
        virtual void ListBranches() { branches = {"JetsAK8_NumBhadrons"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_NumBhadrons->size()>index) value.Fill(looper->JetsAK8_NumBhadrons->at(index),w); }
};
REGISTER_JETFILLER(nb);

//num c hadrons
class KJetFiller_nc : public KJetFiller {
    public:
        using KJetFiller::KJetFiller;
        virtual void ListBranches() { branches = {"JetsAK8_NumChadrons"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_NumChadrons->size()>index) value.Fill(looper->JetsAK8_NumChadrons->at(index),w); }
};
REGISTER_JETFILLER(nc);

class KJetFiller_mass : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8->size()>index) value.Fill(looper->JetsAK8->at(index).M(),w); }
};
REGISTER_JETFILLER(mass);

class KJetFiller_axisminor : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_axisminor"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_axisminor->size()>index) value.Fill(looper->JetsAK8_axisminor->at(index),w); }
};
REGISTER_JETFILLER(axisminor);

class KJetFiller_axismajor : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_axismajor"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_axismajor->size()>index) value.Fill(looper->JetsAK8_axismajor->at(index),w); }
};
REGISTER_JETFILLER(axismajor);

class KJetFiller_girth : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_girth"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_girth->size()>index) value.Fill(looper->JetsAK8_girth->at(index),w); }
};
REGISTER_JETFILLER(girth);

class KJetFiller_mhalf : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_momenthalf"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_momenthalf->size()>index) value.Fill(looper->JetsAK8_momenthalf->at(index),w); }
};
REGISTER_JETFILLER(mhalf);

class KJetFiller_mult : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_multiplicity"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_multiplicity->size()>index) value.Fill(looper->JetsAK8_multiplicity->at(index),w); }
};
REGISTER_JETFILLER(mult);

class KJetFiller_overflow : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_overflow"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_overflow->size()>index) value.Fill(looper->JetsAK8_overflow->at(index),w); }
};
REGISTER_JETFILLER(overflow);

class KJetFiller_ptD : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_ptD"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_ptD->size()>index) value.Fill(looper->JetsAK8_ptD->at(index),w); }
};
REGISTER_JETFILLER(ptD);

//derived substructure variables
class KJetFiller_tau21 : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_NsubjettinessTau1","JetsAK8_NsubjettinessTau2"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) {
			if(looper->JetsAK8_NsubjettinessTau1->size()>index and looper->JetsAK8_NsubjettinessTau1->at(index)>0) value.Fill(looper->JetsAK8_NsubjettinessTau2->at(index)/looper->JetsAK8_NsubjettinessTau1->at(index),w);
		}
};
REGISTER_JETFILLER(tau21);

class KJetFiller_tau32 : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_NsubjettinessTau3","JetsAK8_NsubjettinessTau2"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) {
			if(looper->JetsAK8_NsubjettinessTau2->size()>index and looper->JetsAK8_NsubjettinessTau2->at(index)>0) value.Fill(looper->JetsAK8_NsubjettinessTau3->at(index)/looper->JetsAK8_NsubjettinessTau2->at(index),w);
		}
};
REGISTER_JETFILLER(tau32);

class KJetFiller_msd : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_softDropMass"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_softDropMass->size()>index) value.Fill(looper->JetsAK8_softDropMass->at(index),w); }
};
REGISTER_JETFILLER(msd);

class KJetFiller_nsubjet : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_subjets"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_subjets->size()>index) value.Fill(looper->JetsAK8_subjets->at(index).size(),w); }
};
REGISTER_JETFILLER(nsubjet);

//bdt output
class KJetFiller_bdt : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void CheckDeps(){ BDT = sel->Get<KBDTSelector*>("BDT"); }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(BDT and BDT->JetsAK8_bdt.size()>index) value.Fill(BDT->JetsAK8_bdt[index],w); }		//member variables
		KBDTSelector* BDT = NULL;
};
REGISTER_JETFILLER(bdt);
/*
class KJetFiller_fChEM : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_chargedEmEnergyFraction"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_chargedEmEnergyFraction->size()>index) value.Fill(looper->JetsAK8_chargedEmEnergyFraction->at(index),w); }
};
REGISTER_JETFILLER(fChEM);

class KJetFiller_fChHad : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_chargedHadronEnergyFraction"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_chargedHadronEnergyFraction->size()>index) value.Fill(looper->JetsAK8_chargedHadronEnergyFraction->at(index),w); }
};
REGISTER_JETFILLER(fChHad);

class KJetFiller_nChHad : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_chargedHadronMultiplicity"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_chargedHadronMultiplicity->size()>index) value.Fill(looper->JetsAK8_chargedHadronMultiplicity->at(index),w); }
};
REGISTER_JETFILLER(nChHad);

class KJetFiller_nCh : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_chargedMultiplicity"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_chargedMultiplicity->size()>index) value.Fill(looper->JetsAK8_chargedMultiplicity->at(index),w); }
};
REGISTER_JETFILLER(nCh);

class KJetFiller_fEle : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_electronEnergyFraction"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_electronEnergyFraction->size()>index) value.Fill(looper->JetsAK8_electronEnergyFraction->at(index),w); }
};
REGISTER_JETFILLER(fEle);

class KJetFiller_nEle : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_electronMultiplicity"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_electronMultiplicity->size()>index) value.Fill(looper->JetsAK8_electronMultiplicity->at(index),w); }
};
REGISTER_JETFILLER(nEle);

class KJetFiller_fHFEM : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_hfEMEnergyFraction"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_hfEMEnergyFraction->size()>index) value.Fill(looper->JetsAK8_hfEMEnergyFraction->at(index),w); }
};
REGISTER_JETFILLER(fHFEM);

class KJetFiller_fHFHad : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_hfHadronEnergyFraction"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_hfHadronEnergyFraction->size()>index) value.Fill(looper->JetsAK8_hfHadronEnergyFraction->at(index),w); }
};
REGISTER_JETFILLER(fHFHad);

class KJetFiller_lean : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_lean"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_lean->size()>index) value.Fill(looper->JetsAK8_lean->at(index),w); }
};
REGISTER_JETFILLER(lean);

class KJetFiller_fMu : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_muonEnergyFraction"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_muonEnergyFraction->size()>index) value.Fill(looper->JetsAK8_muonEnergyFraction->at(index),w); }
};
REGISTER_JETFILLER(fMu);

class KJetFiller_nMu : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_muonMultiplicity"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_muonMultiplicity->size()>index) value.Fill(looper->JetsAK8_muonMultiplicity->at(index),w); }
};
REGISTER_JETFILLER(nMu);

class KJetFiller_fNeuEM : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_neutralEmEnergyFraction"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_neutralEmEnergyFraction->size()>index) value.Fill(looper->JetsAK8_neutralEmEnergyFraction->at(index),w); }
};
REGISTER_JETFILLER(fNeuEM);

class KJetFiller_fNeuHad : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_neutralHadronEnergyFraction"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_neutralHadronEnergyFraction->size()>index) value.Fill(looper->JetsAK8_neutralHadronEnergyFraction->at(index),w); }
};
REGISTER_JETFILLER(fNeuHad);

class KJetFiller_nNeuHad : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_neutralHadronMultiplicity"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_neutralHadronMultiplicity->size()>index) value.Fill(looper->JetsAK8_neutralHadronMultiplicity->at(index),w); }
};
REGISTER_JETFILLER(nNeuHad);

class KJetFiller_nNeu : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_neutralMultiplicity"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_neutralMultiplicity->size()>index) value.Fill(looper->JetsAK8_neutralMultiplicity->at(index),w); }
};
REGISTER_JETFILLER(nNeu);

class KJetFiller_fPho : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_photonEnergyFraction"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_photonEnergyFraction->size()>index) value.Fill(looper->JetsAK8_photonEnergyFraction->at(index),w); }
};
REGISTER_JETFILLER(fPho);

class KJetFiller_nPho : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_photonMultiplicity"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_photonMultiplicity->size()>index) value.Fill(looper->JetsAK8_photonMultiplicity->at(index),w); }
};
REGISTER_JETFILLER(nPho);
*/
class KJetFiller_ptdrlog : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_ptdrlog"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_ptdrlog->size()>index) value.Fill(looper->JetsAK8_ptdrlog->at(index),w); }
};
REGISTER_JETFILLER(ptdrlog);

class KJetFiller_ptdrlognorm : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void ListBranches() { branches = {"JetsAK8_ptdrlog","JetsAK8","JetsAK8_multiplicity"}; }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(looper->JetsAK8_ptdrlog->size()>index) value.Fill(looper->JetsAK8_multiplicity->at(index)*log(looper->JetsAK8->at(index).Pt())-looper->JetsAK8_ptdrlog->at(index),w); }
};
REGISTER_JETFILLER(ptdrlognorm);

//-----------------------------------------------------------------------------
//per-jet quantities based on dark hadron gen info
class KJetFillerDarkHadron : public KJetFiller {
	public:
		using KJetFiller::KJetFiller;
		virtual void CheckDeps(){ DarkHadron = sel->Get<KDarkHadronSelector*>("DarkHadron"); }
		virtual void FillPerJet(KValue& value, double w, unsigned index) { if(DarkHadron) FillPerJet_(value,w,index); }
		virtual void FillPerJet_(KValue& value, double w, unsigned index) {}
		//member variables
		KDarkHadronSelector* DarkHadron = NULL;
};

class KJetFiller_nstable : public KJetFillerDarkHadron {
	public:
		using KJetFillerDarkHadron::KJetFillerDarkHadron;
		virtual void FillPerJet_(KValue& value, double w, unsigned index) { if(DarkHadron->n_stable.size()>index) value.Fill(DarkHadron->n_stable[index],w); }
};
REGISTER_JETFILLER(nstable);

class KJetFiller_nunstable : public KJetFillerDarkHadron {
	public:
		using KJetFillerDarkHadron::KJetFillerDarkHadron;
		virtual void FillPerJet_(KValue& value, double w, unsigned index) { if(DarkHadron->n_unstable.size()>index) value.Fill(DarkHadron->n_unstable[index],w); }
};
REGISTER_JETFILLER(nunstable);

class KJetFiller_nvisible : public KJetFillerDarkHadron {
	public:
		using KJetFillerDarkHadron::KJetFillerDarkHadron;
		virtual void FillPerJet_(KValue& value, double w, unsigned index) { if(DarkHadron->n_visible.size()>index) value.Fill(DarkHadron->n_visible[index],w); }
};
REGISTER_JETFILLER(nvisible);

class KJetFiller_rinv : public KJetFillerDarkHadron {
	public:
		using KJetFillerDarkHadron::KJetFillerDarkHadron;
		virtual void FillPerJet_(KValue& value, double w, unsigned index) { if(DarkHadron->rinv.size()>index) value.Fill(DarkHadron->rinv[index],w); }
};
REGISTER_JETFILLER(rinv);

//-----------------------------------------------------------------------------

#endif

#ifndef KBDTVar_H
#define KBDTVar_H

#include "KChecker.h"
#include "KLooper.h"
#include "KFactory.h"
#include "KMath.h"

#include "TTree.h"
#include "TMVA/Reader.h"
#include "../KMVA/BDTree.h"

#include <string>
#include <vector>
#include <cmath>

using namespace std;

//helper class, similar to KFiller
class KBDTVar : public KChecker {
	public:
		KBDTVar() : KChecker("",NULL) { }
		KBDTVar(string name_) : KChecker(name_,NULL) { }
		
		//functions
		virtual void SetBranch(TTree* tree){
			tree->Branch(name.c_str(),&branch,(name+"/F").c_str());
		}
		virtual void SetVariable(TMVA::Reader* reader){
			reader->AddVariable(name.c_str(),&branch);
		}
		virtual void SetVariable(BDTree* bdtree){
			pbranch = bdtree->SetVariable(name);
			use_kmva = true;
		}
		
		virtual void Fill(unsigned index) {
			Fill_(index);
			if(use_kmva) *pbranch = branch;
		}
		virtual void Fill_(unsigned index) { }
		
		//members
		float branch;
		bool use_kmva = false;
		float* pbranch;
};
typedef KFactory<KBDTVar,string> KBDTVarFactory;
#define REGISTER_BDTVAR(a) REGISTER_MACRO2(KBDTVarFactory,KBDTVar_##a,a)

class KBDTVar_pt : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8->at(j).Pt(); }
};
REGISTER_BDTVAR(pt);

class KBDTVar_eta : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8->at(j).Eta(); }
};
REGISTER_BDTVAR(eta);

class KBDTVar_phi : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8->at(j).Phi(); }
};
REGISTER_BDTVAR(phi);

class KBDTVar_index : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void Fill_(unsigned j) { branch = j; }
};
REGISTER_BDTVAR(index);

class KBDTVar_deltaphi : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8","METPhi"}; }
		virtual void Fill_(unsigned j) { branch = abs(KMath::DeltaPhi(looper->METPhi,looper->JetsAK8->at(j).Phi())); }
};
REGISTER_BDTVAR(deltaphi);

class KBDTVar_ptD : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_ptD"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_ptD->at(j); }
};
REGISTER_BDTVAR(ptD);

class KBDTVar_axismajor : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_axismajor"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_axismajor->at(j); }
};
REGISTER_BDTVAR(axismajor);

class KBDTVar_axisminor : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_axisminor"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_axisminor->at(j); }
};
REGISTER_BDTVAR(axisminor);

class KBDTVar_ecfN2b1 : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_ecfN2b1"}; }
		virtual void Fill_(unsigned j) { branch = max(looper->JetsAK8_ecfN2b1->at(j),-1.); }
};
REGISTER_BDTVAR(ecfN2b1);

class KBDTVar_ecfN2b2 : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_ecfN2b2"}; }
		virtual void Fill_(unsigned j) { branch = max(looper->JetsAK8_ecfN2b2->at(j),-1.); }
};
REGISTER_BDTVAR(ecfN2b2);

class KBDTVar_ecfN3b1 : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_ecfN3b1"}; }
		virtual void Fill_(unsigned j) { branch = max(looper->JetsAK8_ecfN3b1->at(j),-1.); }
};
REGISTER_BDTVAR(ecfN3b1);

class KBDTVar_ecfN3b2 : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_ecfN3b2"}; }
		virtual void Fill_(unsigned j) { branch = max(looper->JetsAK8_ecfN3b2->at(j),-1.); }
};
REGISTER_BDTVAR(ecfN3b2);

class KBDTVar_girth : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_girth"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_girth->at(j); }
};
REGISTER_BDTVAR(girth);

class KBDTVar_momenthalf : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_momenthalf"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_momenthalf->at(j); }
};
REGISTER_BDTVAR(momenthalf);

class KBDTVar_msd : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_softDropMass"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_softDropMass->at(j); }
};
REGISTER_BDTVAR(msd);

class KBDTVar_mult : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_multiplicity"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_multiplicity->at(j); }
};
REGISTER_BDTVAR(mult);

class KBDTVar_tau21 : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_NsubjettinessTau1","JetsAK8_NsubjettinessTau2"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_NsubjettinessTau1->at(j) > 0 ? looper->JetsAK8_NsubjettinessTau2->at(j)/looper->JetsAK8_NsubjettinessTau1->at(j) : -1; }
};
REGISTER_BDTVAR(tau21);

class KBDTVar_tau32 : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_NsubjettinessTau2","JetsAK8_NsubjettinessTau3"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_NsubjettinessTau2->at(j) > 0 ? looper->JetsAK8_NsubjettinessTau3->at(j)/looper->JetsAK8_NsubjettinessTau2->at(j) : -1; }
};
REGISTER_BDTVAR(tau32);

class KBDTVar_lean : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_lean"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_lean->at(j); }
};
REGISTER_BDTVAR(lean);

class KBDTVar_ptdrlog : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_ptdrlog"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_ptdrlog->at(j); }
};
REGISTER_BDTVAR(ptdrlog);

class KBDTVar_fChEM : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_chargedEmEnergyFraction"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_chargedEmEnergyFraction->at(j); }
};
REGISTER_BDTVAR(fChEM);

class KBDTVar_fChHad : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_chargedHadronEnergyFraction"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_chargedHadronEnergyFraction->at(j); }
};
REGISTER_BDTVAR(fChHad);

class KBDTVar_fEle : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_electronEnergyFraction"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_electronEnergyFraction->at(j); }
};
REGISTER_BDTVAR(fEle);

class KBDTVar_fHFEM : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_hfEMEnergyFraction"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_hfEMEnergyFraction->at(j); }
};
REGISTER_BDTVAR(fHFEM);

class KBDTVar_fHFHad : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_hfHadronEnergyFraction"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_hfHadronEnergyFraction->at(j); }
};
REGISTER_BDTVAR(fHFHad);

class KBDTVar_fMu : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_muonEnergyFraction"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_muonEnergyFraction->at(j); }
};
REGISTER_BDTVAR(fMu);

class KBDTVar_fNeuEM : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_neutralEmEnergyFraction"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_neutralEmEnergyFraction->at(j); }
};
REGISTER_BDTVAR(fNeuEM);

class KBDTVar_fNeuHad : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_neutralHadronEnergyFraction"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_neutralHadronEnergyFraction->at(j); }
};
REGISTER_BDTVAR(fNeuHad);

class KBDTVar_fPho : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_photonEnergyFraction"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_photonEnergyFraction->at(j); }
};
REGISTER_BDTVAR(fPho);

class KBDTVar_nCh : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_chargedMultiplicity"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_chargedMultiplicity->at(j); }
};
REGISTER_BDTVAR(nCh);

class KBDTVar_nChHad : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_chargedHadronMultiplicity"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_chargedHadronMultiplicity->at(j); }
};
REGISTER_BDTVAR(nChHad);

class KBDTVar_nEle : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_electronMultiplicity"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_electronMultiplicity->at(j); }
};
REGISTER_BDTVAR(nEle);

class KBDTVar_nMu : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_muonMultiplicity"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_muonMultiplicity->at(j); }
};
REGISTER_BDTVAR(nMu);

class KBDTVar_nNeuHad : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_neutralHadronMultiplicity"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_neutralHadronMultiplicity->at(j); }
};
REGISTER_BDTVAR(nNeuHad);

class KBDTVar_nNeu : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_neutralMultiplicity"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_neutralMultiplicity->at(j); }
};
REGISTER_BDTVAR(nNeu);

class KBDTVar_nPho : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_photonMultiplicity"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_photonMultiplicity->at(j); }
};
REGISTER_BDTVAR(nPho);

class KBDTVar_nSubjets : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_subjets"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_subjets->at(j).size(); }
};
REGISTER_BDTVAR(nSubjets);

class KBDTVar_subjetCSV1 : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_subjets","JetsAK8_subjets_bDiscriminatorCSV"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_subjets->at(j).size()>0 ? looper->JetsAK8_subjets_bDiscriminatorCSV->at(j).at(0) : -10; }
};
REGISTER_BDTVAR(subjetCSV1);

class KBDTVar_subjetCSV2 : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_subjets","JetsAK8_subjets_bDiscriminatorCSV"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_subjets->at(j).size()>1 ? looper->JetsAK8_subjets_bDiscriminatorCSV->at(j).at(1) : -10; }
};
REGISTER_BDTVAR(subjetCSV2);

//forward declaration
class KJetMatchSelector;
class KBDTVar_maxbvsall : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"Jets_bJetTagDeepCSVBvsAll",prefilled_branch}; }
		//implemented in KCommonSelectors to avoid circular dependency
		virtual void CheckDeps();
		virtual void Fill_(unsigned j);

		//members
		KJetMatchSelector* JetMatch = NULL;
		string prefilled_branch = "JetsAK8_maxBvsAll";
		bool branch_present = false;
};
REGISTER_BDTVAR(maxbvsall);

class KBDTVar_isHV : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"JetsAK8_isHV"}; }
		virtual void Fill_(unsigned j) { branch = looper->JetsAK8_isHV->at(j); }
};
REGISTER_BDTVAR(isHV);

class KBDTVar_mt : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"MT_AK8"}; }
		virtual void Fill_(unsigned j) { branch = looper->MT_AK8; }
};
REGISTER_BDTVAR(mt);

class KBDTVar_procweight : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"Weight"}; }
		virtual void Fill_(unsigned j) { branch = looper->Weight; }
};
REGISTER_BDTVAR(procweight);

class KBDTVar_puweight : public KBDTVar {
	public:
		using KBDTVar::KBDTVar;
		virtual void ListBranches() { branches = {"puWeightNew"}; }
		virtual void Fill_(unsigned j) { branch = looper->puWeightNew; }
};
REGISTER_BDTVAR(puweight);

#endif

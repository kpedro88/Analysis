#ifndef KSKIMMERVARIATORS_H
#define KSKIMMERVARIATORS_H

//custom headers
#include "KVariation.h"
#include "KSkimmer.h"
#include "KMath.h"

//ROOT headers
#include <TLorentzVector.h>
#include <TMath.h>
#include <TTree.h>

//STL headers
#include <string>
#include <vector>
#include <map>

using namespace std;

//base class for variators is in KVariation.h

//todo: rework this class to handle new method of storing systematics (origIndex, etc.)
//      consider separating clean and jec/jer parts
/*
//----------------------------------------------------
//variation of jet-related variables
//for: JECs, JERs, Zinv cleaning
class KJetVariator : public KVariator {
	public:
		enum vartypes { NoVar=0, JECup=1, JECdown=2, JERup=3, JERdown=4, clean=5 };
		//constructor
		KJetVariator() : KVariator() { }
		KJetVariator(string name_, OptionMap* localOpt_) : KVariator(name_,localOpt_), vtype(NoVar)
		{
			//check options
			string vname = "";
			localOpt->Get("vartype",vname);
			if(vname=="JECup") vtype = JECup;
			else if(vname=="JECdown") vtype = JECdown;
			else if(vname=="JERup") vtype = JERup;
			else if(vname=="JERdown") vtype = JERdown;
			else if(vname=="clean") vtype = clean;
		}
		virtual void CheckBranches(){
			//set up linked branches for all variations
			branches = {
				//AK4 jets
				new KLinkedBranchVL(KBranchVL(looper->Jets,"Jets"),vtype==JECup?KBranchVL(looper->JetsJECup,"JetsJECup"):vtype==JECdown?KBranchVL(looper->JetsJECdown,"JetsJECdown"):vtype==JERup?KBranchVL(looper->JetsJERup,"JetsJERup"):vtype==JERdown?KBranchVL(looper->JetsJERdown,"JetsJERdown"):vtype==clean?KBranchVL(looper->Jetsclean,"Jetsclean"):KBranchVL(NULL,"")),
				new KLinkedBranchVB(KBranchVB(looper->Jets_HTMask,"Jets_HTMask"),vtype==JECup?KBranchVB(looper->JetsJECup_HTMask,"JetsJECup_HTMask"):vtype==JECdown?KBranchVB(looper->JetsJECdown_HTMask,"JetsJECdown_HTMask"):vtype==JERup?KBranchVB(looper->JetsJERup_HTMask,"JetsJERup_HTMask"):vtype==JERdown?KBranchVB(looper->JetsJERdown_HTMask,"JetsJERdown_HTMask"):vtype==clean?KBranchVB(looper->Jetsclean_HTMask,"Jetsclean_HTMask"):KBranchVB(NULL,"")),
				new KLinkedBranchVB(KBranchVB(looper->Jets_MHTMask,"Jets_MHTMask"),vtype==JECup?KBranchVB(looper->JetsJECup_MHTMask,"JetsJECup_MHTMask"):vtype==JECdown?KBranchVB(looper->JetsJECdown_MHTMask,"JetsJECdown_MHTMask"):vtype==JERup?KBranchVB(looper->JetsJERup_MHTMask,"JetsJERup_MHTMask"):vtype==JERdown?KBranchVB(looper->JetsJERdown_MHTMask,"JetsJERdown_MHTMask"):vtype==clean?KBranchVB(looper->Jetsclean_MHTMask,"Jetsclean_MHTMask"):KBranchVB(NULL,"")),
				new KLinkedBranchVB(KBranchVB(looper->Jets_ISRMask,"Jets_ISRMask"),vtype==JECup?KBranchVB(looper->JetsJECup_ISRMask,"JetsJECup_ISRMask"):vtype==JECdown?KBranchVB(looper->JetsJECdown_ISRMask,"JetsJECdown_ISRMask"):vtype==JERup?KBranchVB(looper->JetsJERup_ISRMask,"JetsJERup_ISRMask"):vtype==JERdown?KBranchVB(looper->JetsJERdown_ISRMask,"JetsJERdown_ISRMask"):vtype==clean?KBranchVB(looper->Jetsclean_ISRMask,"Jetsclean_ISRMask"):KBranchVB(NULL,"")),
				new KLinkedBranchVB(KBranchVB(looper->Jets_LeptonMask,"Jets_LeptonMask"),vtype==JECup?KBranchVB(looper->JetsJECup_LeptonMask,"JetsJECup_LeptonMask"):vtype==JECdown?KBranchVB(looper->JetsJECdown_LeptonMask,"JetsJECdown_LeptonMask"):vtype==JERup?KBranchVB(looper->JetsJERup_LeptonMask,"JetsJERup_LeptonMask"):vtype==JERdown?KBranchVB(looper->JetsJERdown_LeptonMask,"JetsJERdown_LeptonMask"):KBranchVB(NULL,"")),
				new KLinkedBranchVB(KBranchVB(looper->Jets_ID,"Jets_ID"),vtype==JECup?KBranchVB(looper->JetsJECup_ID,"JetsJECup_ID"):vtype==JECdown?KBranchVB(looper->JetsJECdown_ID,"JetsJECdown_ID"):vtype==JERup?KBranchVB(looper->JetsJERup_ID,"JetsJERup_ID"):vtype==JERdown?KBranchVB(looper->JetsJERdown_ID,"JetsJERdown_ID"):vtype==clean?KBranchVB(looper->Jetsclean_ID,"Jetsclean_ID"):KBranchVB(NULL,"")),
				new KLinkedBranchVI(KBranchVI(looper->Jets_partonFlavor,"Jets_partonFlavor"),vtype==JECup?KBranchVI(looper->JetsJECup_partonFlavor,"JetsJECup_partonFlavor"):vtype==JECdown?KBranchVI(looper->JetsJECdown_partonFlavor,"JetsJECdown_partonFlavor"):vtype==JERup?KBranchVI(looper->JetsJERup_partonFlavor,"JetsJERup_partonFlavor"):vtype==JERdown?KBranchVI(looper->JetsJERdown_partonFlavor,"JetsJERdown_partonFlavor"):vtype==clean?KBranchVI(looper->Jetsclean_partonFlavor,"Jetsclean_partonFlavor"):KBranchVI(NULL,"")),
				new KLinkedBranchVI(KBranchVI(looper->Jets_hadronFlavor,"Jets_hadronFlavor"),vtype==JECup?KBranchVI(looper->JetsJECup_hadronFlavor,"JetsJECup_hadronFlavor"):vtype==JECdown?KBranchVI(looper->JetsJECdown_hadronFlavor,"JetsJECdown_hadronFlavor"):vtype==JERup?KBranchVI(looper->JetsJERup_hadronFlavor,"JetsJERup_hadronFlavor"):vtype==JERdown?KBranchVI(looper->JetsJERdown_hadronFlavor,"JetsJERdown_hadronFlavor"):vtype==clean?KBranchVI(looper->Jetsclean_hadronFlavor,"Jetsclean_hadronFlavor"):KBranchVI(NULL,"")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_bDiscriminatorCSV,"Jets_bDiscriminatorCSV"),vtype==JECup?KBranchVD(looper->JetsJECup_bDiscriminatorCSV,"JetsJECup_bDiscriminatorCSV"):vtype==JECdown?KBranchVD(looper->JetsJECdown_bDiscriminatorCSV,"JetsJECdown_bDiscriminatorCSV"):vtype==JERup?KBranchVD(looper->JetsJERup_bDiscriminatorCSV,"JetsJERup_bDiscriminatorCSV"):vtype==JERdown?KBranchVD(looper->JetsJERdown_bDiscriminatorCSV,"JetsJERdown_bDiscriminatorCSV"):vtype==clean?KBranchVD(looper->Jetsclean_bDiscriminatorCSV,"Jetsclean_bDiscriminatorCSV"):KBranchVD(NULL,"")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_muonEnergyFraction,"Jets_muonEnergyFraction"),vtype==JECup?KBranchVD(looper->JetsJECup_muonEnergyFraction,"JetsJECup_muonEnergyFraction"):vtype==JECdown?KBranchVD(looper->JetsJECdown_muonEnergyFraction,"JetsJECdown_muonEnergyFraction"):vtype==JERup?KBranchVD(looper->JetsJERup_muonEnergyFraction,"JetsJERup_muonEnergyFraction"):vtype==JERdown?KBranchVD(looper->JetsJERdown_muonEnergyFraction,"JetsJERdown_muonEnergyFraction"):vtype==clean?KBranchVD(looper->Jetsclean_muonEnergyFraction,"Jetsclean_muonEnergyFraction"):KBranchVD(NULL,"")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_chargedHadronEnergyFraction,"Jets_chargedHadronEnergyFraction"),vtype==JECup?KBranchVD(looper->JetsJECup_chargedHadronEnergyFraction,"JetsJECup_chargedHadronEnergyFraction"):vtype==JECdown?KBranchVD(looper->JetsJECdown_chargedHadronEnergyFraction,"JetsJECdown_chargedHadronEnergyFraction"):vtype==JERup?KBranchVD(looper->JetsJERup_chargedHadronEnergyFraction,"JetsJERup_chargedHadronEnergyFraction"):vtype==JERdown?KBranchVD(looper->JetsJERdown_chargedHadronEnergyFraction,"JetsJERdown_chargedHadronEnergyFraction"):vtype==clean?KBranchVD(looper->Jetsclean_chargedHadronEnergyFraction,"Jetsclean_chargedHadronEnergyFraction"):KBranchVD(NULL,"")),
				new KLinkedBranchB(KBranchB(&looper->JetID,"JetID"),vtype==JECup?KBranchB(&looper->JetIDJECup,"JetIDJECup"):vtype==JECdown?KBranchB(&looper->JetIDJECdown,"JetIDJECdown"):vtype==JERup?KBranchB(&looper->JetIDJERup,"JetIDJERup"):vtype==JERdown?KBranchB(&looper->JetIDJERdown,"JetIDJERdown"):vtype==clean?KBranchB(&looper->JetIDclean,"JetIDclean"):KBranchB(NULL,"")),
				new KLinkedBranchI(KBranchI(&looper->NJets,"NJets"),vtype==JECup?KBranchI(&looper->NJetsJECup,"NJetsJECup"):vtype==JECdown?KBranchI(&looper->NJetsJECdown,"NJetsJECdown"):vtype==JERup?KBranchI(&looper->NJetsJERup,"NJetsJERup"):vtype==JERdown?KBranchI(&looper->NJetsJERdown,"NJetsJERdown"):vtype==clean?KBranchI(&looper->NJetsclean,"NJetsclean"):KBranchI(NULL,"")),
				new KLinkedBranchI(KBranchI(&looper->NJetsISR,"NJetsISR"),vtype==JECup?KBranchI(&looper->NJetsISRJECup,"NJetsISRJECup"):vtype==JECdown?KBranchI(&looper->NJetsISRJECdown,"NJetsISRJECdown"):vtype==JERup?KBranchI(&looper->NJetsISRJERup,"NJetsISRJERup"):vtype==JERdown?KBranchI(&looper->NJetsISRJERdown,"NJetsISRJERdown"):vtype==clean?KBranchI(&looper->NJetsISRclean,"NJetsISRclean"):KBranchI(NULL,"")),
				new KLinkedBranchI(KBranchI(&looper->BTags,"BTags"),vtype==JECup?KBranchI(&looper->BTagsJECup,"BTagsJECup"):vtype==JECdown?KBranchI(&looper->BTagsJECdown,"BTagsJECdown"):vtype==JERup?KBranchI(&looper->BTagsJERup,"BTagsJERup"):vtype==JERdown?KBranchI(&looper->BTagsJERdown,"BTagsJERdown"):vtype==clean?KBranchI(&looper->BTagsclean,"BTagsclean"):KBranchI(NULL,"")),
				new KLinkedBranchD(KBranchD(&looper->HT,"HT"),vtype==JECup?KBranchD(&looper->HTJECup,"HTJECup"):vtype==JECdown?KBranchD(&looper->HTJECdown,"HTJECdown"):vtype==JERup?KBranchD(&looper->HTJERup,"HTJERup"):vtype==JERdown?KBranchD(&looper->HTJERdown,"HTJERdown"):vtype==clean?KBranchD(&looper->HTclean,"HTclean"):KBranchD(NULL,"")),
				new KLinkedBranchD(KBranchD(&looper->MHT,"MHT"),vtype==JECup?KBranchD(&looper->MHTJECup,"MHTJECup"):vtype==JECdown?KBranchD(&looper->MHTJECdown,"MHTJECdown"):vtype==JERup?KBranchD(&looper->MHTJERup,"MHTJERup"):vtype==JERdown?KBranchD(&looper->MHTJERdown,"MHTJERdown"):vtype==clean?KBranchD(&looper->MHTclean,"MHTclean"):KBranchD(NULL,"")),
				new KLinkedBranchD(KBranchD(&looper->MHTPhi,"MHTPhi"),vtype==JECup?KBranchD(&looper->MHTPhiJECup,"MHTPhiJECup"):vtype==JECdown?KBranchD(&looper->MHTPhiJECdown,"MHTPhiJECdown"):vtype==JERup?KBranchD(&looper->MHTPhiJERup,"MHTPhiJERup"):vtype==JERdown?KBranchD(&looper->MHTPhiJERdown,"MHTPhiJERdown"):vtype==clean?KBranchD(&looper->MHTPhiclean,"MHTPhiclean"):KBranchD(NULL,"")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi1,"DeltaPhi1"),vtype==JECup?KBranchD(&looper->DeltaPhi1JECup,"DeltaPhi1JECup"):vtype==JECdown?KBranchD(&looper->DeltaPhi1JECdown,"DeltaPhi1JECdown"):vtype==JERup?KBranchD(&looper->DeltaPhi1JERup,"DeltaPhi1JERup"):vtype==JERdown?KBranchD(&looper->DeltaPhi1JERdown,"DeltaPhi1JERdown"):vtype==clean?KBranchD(&looper->DeltaPhi1clean,"DeltaPhi1clean"):KBranchD(NULL,"")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi2,"DeltaPhi2"),vtype==JECup?KBranchD(&looper->DeltaPhi2JECup,"DeltaPhi2JECup"):vtype==JECdown?KBranchD(&looper->DeltaPhi2JECdown,"DeltaPhi2JECdown"):vtype==JERup?KBranchD(&looper->DeltaPhi2JERup,"DeltaPhi2JERup"):vtype==JERdown?KBranchD(&looper->DeltaPhi2JERdown,"DeltaPhi2JERdown"):vtype==clean?KBranchD(&looper->DeltaPhi2clean,"DeltaPhi2clean"):KBranchD(NULL,"")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi3,"DeltaPhi3"),vtype==JECup?KBranchD(&looper->DeltaPhi3JECup,"DeltaPhi3JECup"):vtype==JECdown?KBranchD(&looper->DeltaPhi3JECdown,"DeltaPhi3JECdown"):vtype==JERup?KBranchD(&looper->DeltaPhi3JERup,"DeltaPhi3JERup"):vtype==JERdown?KBranchD(&looper->DeltaPhi3JERdown,"DeltaPhi3JERdown"):vtype==clean?KBranchD(&looper->DeltaPhi3clean,"DeltaPhi3clean"):KBranchD(NULL,"")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi4,"DeltaPhi4"),vtype==JECup?KBranchD(&looper->DeltaPhi4JECup,"DeltaPhi4JECup"):vtype==JECdown?KBranchD(&looper->DeltaPhi4JECdown,"DeltaPhi4JECdown"):vtype==JERup?KBranchD(&looper->DeltaPhi4JERup,"DeltaPhi4JERup"):vtype==JERdown?KBranchD(&looper->DeltaPhi4JERdown,"DeltaPhi4JERdown"):vtype==clean?KBranchD(&looper->DeltaPhi4clean,"DeltaPhi4clean"):KBranchD(NULL,"")),
				//AK8 jets
				new KLinkedBranchVL(KBranchVL(looper->JetsAK8,"JetsAK8"),vtype==clean?KBranchVL(looper->JetsAK8Clean,"JetsAK8Clean"):KBranchVL(NULL,"")),
				new KLinkedBranchVB(KBranchVB(looper->JetsAK8_ID,"JetsAK8_ID"),vtype==clean?KBranchVB(looper->JetsAK8Clean_ID,"JetsAK8Clean_ID"):KBranchVB(NULL,"")),
				new KLinkedBranchVI(KBranchVI(looper->JetsAK8_NumBhadrons,"JetsAK8_NumBhadrons"),vtype==clean?KBranchVI(looper->JetsAK8Clean_NumBhadrons,"JetsAK8Clean_NumBhadrons"):KBranchVI(NULL,"")),
				new KLinkedBranchVI(KBranchVI(looper->JetsAK8_NumChadrons,"JetsAK8_NumChadrons"),vtype==clean?KBranchVI(looper->JetsAK8Clean_NumChadrons,"JetsAK8Clean_NumChadrons"):KBranchVI(NULL,"")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_doubleBDiscriminator,"JetsAK8_doubleBDiscriminator"),vtype==clean?KBranchVD(looper->JetsAK8Clean_doubleBDiscriminator,"JetsAK8Clean_doubleBDiscriminator"):KBranchVD(NULL,"")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_NsubjettinessTau1,"JetsAK8_NsubjettinessTau1"),vtype==clean?KBranchVD(looper->JetsAK8Clean_NsubjettinessTau1,"JetsAK8Clean_NsubjettinessTau1"):KBranchVD(NULL,"")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_NsubjettinessTau2,"JetsAK8_NsubjettinessTau2"),vtype==clean?KBranchVD(looper->JetsAK8Clean_NsubjettinessTau2,"JetsAK8Clean_NsubjettinessTau2"):KBranchVD(NULL,"")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_NsubjettinessTau3,"JetsAK8_NsubjettinessTau3"),vtype==clean?KBranchVD(looper->JetsAK8Clean_NsubjettinessTau3,"JetsAK8Clean_NsubjettinessTau3"):KBranchVD(NULL,"")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_prunedMass,"JetsAK8_prunedMass"),vtype==clean?KBranchVD(looper->JetsAK8Clean_prunedMass,"JetsAK8Clean_prunedMass"):KBranchVD(NULL,"")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_axismajor,"JetsAK8_axismajor"),vtype==clean?KBranchVD(looper->JetsAK8Clean_axismajor,"JetsAK8Clean_axismajor"):KBranchVD(NULL,"")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_axisminor,"JetsAK8_axisminor"),vtype==clean?KBranchVD(looper->JetsAK8Clean_axisminor,"JetsAK8Clean_axisminor"):KBranchVD(NULL,"")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_girth,"JetsAK8_girth"),vtype==clean?KBranchVD(looper->JetsAK8Clean_girth,"JetsAK8Clean_girth"):KBranchVD(NULL,"")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_momenthalf,"JetsAK8_momenthalf"),vtype==clean?KBranchVD(looper->JetsAK8Clean_momenthalf,"JetsAK8Clean_momenthalf"):KBranchVD(NULL,"")),
				new KLinkedBranchVI(KBranchVI(looper->JetsAK8_multiplicity,"JetsAK8_multiplicity"),vtype==clean?KBranchVI(looper->JetsAK8Clean_multiplicity,"JetsAK8Clean_multiplicity"):KBranchVI(NULL,"")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_overflow,"JetsAK8_overflow"),vtype==clean?KBranchVD(looper->JetsAK8Clean_overflow,"JetsAK8Clean_overflow"):KBranchVD(NULL,"")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_ptD,"JetsAK8_ptD"),vtype==clean?KBranchVD(looper->JetsAK8Clean_ptD,"JetsAK8Clean_ptD"):KBranchVD(NULL,"")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_softDropMass,"JetsAK8_softDropMass"),vtype==clean?KBranchVD(looper->JetsAK8Clean_softDropMass,"JetsAK8Clean_softDropMass"):KBranchVD(NULL,"")),
				new KLinkedBranchVVL(KBranchVVL(looper->JetsAK8_subjets,"JetsAK8_subjets"),vtype==clean?KBranchVVL(looper->JetsAK8Clean_subjets,"JetsAK8Clean_subjets"):KBranchVVL(NULL,"")),
				//some only for clean var
				new KLinkedBranchI(KBranchI(&looper->isoElectronTracks,"isoElectronTracks"),vtype==clean?KBranchI(&looper->isoElectronTracksclean,"isoElectronTracksclean"):KBranchI(NULL,"")),
				new KLinkedBranchI(KBranchI(&looper->isoMuonTracks,"isoMuonTracks"),vtype==clean?KBranchI(&looper->isoMuonTracksclean,"isoMuonTracksclean"):KBranchI(NULL,"")),
				new KLinkedBranchI(KBranchI(&looper->isoPionTracks,"isoPionTracks"),vtype==clean?KBranchI(&looper->isoPionTracksclean,"isoPionTracksclean"):KBranchI(NULL,"")),
				new KLinkedBranchD(KBranchD(&looper->MET,"MET"),vtype==clean?KBranchD(&looper->METclean,"METclean"):KBranchD(NULL,"")),
				new KLinkedBranchD(KBranchD(&looper->METPhi,"METPhi"),vtype==clean?KBranchD(&looper->METPhiclean,"METPhiclean"):KBranchD(NULL,"")),
				new KLinkedBranchD(KBranchD(&looper->METSignificance,"METSignificance"),vtype==clean?KBranchD(&looper->METSignificanceclean,"METSignificanceclean"):KBranchD(NULL,"")),
			};
			
			for(auto& branch : branches){
				//enable varied branches which might be disabled by default
				branch->Enable(looper->fChain,1);
				branch->Check(looper->fChain);
			}
		}
		//functions
		virtual void DoVariation() {
			//special case: if Jetsclean is empty, no reclustering was done (nothing removed from event), don't bother varying
			if(looper->Jetsclean->size()==0) return;
			for(auto& branch : branches){
				//store original values
				branch->Store();
				//set to variation
				branch->Vary();
			}
		}
		virtual void UndoVariation() {
			//special case: if Jetsclean is empty, no reclustering was done (nothing removed from event), don't bother restoring
			if(looper->Jetsclean->size()==0) return;
			//restore original values
			for(auto& branch : branches){
				branch->Restore();
			}
		}
		
		//member variables
		vartypes vtype;
};
REGISTER_VARIATOR(Jet);
*/

class KGenMHTVariator : public KVariator {
	public:
		//constructor
		KGenMHTVariator() : KVariator() { }
		KGenMHTVariator(string name_, OptionMap* localOpt_) : KVariator(name_,localOpt_) { }
		virtual void CheckBranches(){
			branches = {
				new KLinkedBranchD(KBranchD(&looper->MHT,"MHT"),KBranchD(&looper->GenMHT,"GenMHT")),
				new KLinkedBranchD(KBranchD(&looper->HT,"HT"),KBranchD(&looper->GenHT,"GenHT"))
			};
			
			for(auto& branch : branches){
				branch->Check(looper->fChain);
			}
		}
		virtual void DoVariation() {
			for(auto& branch : branches){
				//store original values
				branch->Store();
				//set to gen vars
				branch->Vary();
			}
		}
		virtual void UndoVariation(){
			//restore original values
			for(auto& branch : branches){
				branch->Restore();
			}
		}		
};
REGISTER_VARIATOR(GenMHT);

class KJetLeptonVariator : public KVariator {
	public:
		//constructor
		KJetLeptonVariator() : KVariator() {
			clear();
		}
		KJetLeptonVariator(string name_, OptionMap* localOpt_) : KVariator(name_,localOpt_) {
			clear();
		}
		virtual void CheckBranches(){
			branches = {
				new KLinkedBranchVL(KBranchVL(looper->Jets,"Jets"),KBranchVL(&Jets,"Jets_")),
				new KLinkedBranchVB(KBranchVB(looper->Jets_HTMask,"Jets_HTMask"),KBranchVB(&Jets_HTMask,"Jets_HTMask_")),
				new KLinkedBranchVB(KBranchVB(looper->Jets_MHTMask,"Jets_MHTMask"),KBranchVB(&Jets_MHTMask,"Jets_MHTMask_")),
				new KLinkedBranchVB(KBranchVB(looper->Jets_ID,"Jets_ID"),KBranchVB(&Jets_ID,"Jets_ID_")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_bDiscriminatorCSV,"Jets_bDiscriminatorCSV"),KBranchVD(&Jets_bDiscriminatorCSV,"Jets_bDiscriminatorCSV_")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_muonEnergyFraction,"Jets_muonEnergyFraction"),KBranchVD(&Jets_muonEnergyFraction,"Jets_muonEnergyFraction_")),
				new KLinkedBranchB(KBranchB(&looper->JetID,"JetID"),KBranchB(&JetID,"JetID_")),
				new KLinkedBranchI(KBranchI(&looper->NJets,"NJets"),KBranchI(&NJets,"NJets_")),
				new KLinkedBranchI(KBranchI(&looper->BTags,"BTags"),KBranchI(&BTags,"BTags_")),
				new KLinkedBranchD(KBranchD(&looper->MHT,"MHT"),KBranchD(&MHT,"MHT_")),
				new KLinkedBranchD(KBranchD(&looper->MHTPhi,"MHTPhi"),KBranchD(&MHTPhi,"MHTPhi_")),
				new KLinkedBranchD(KBranchD(&looper->HT,"HT"),KBranchD(&HT,"HT_")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi1,"DeltaPhi1"),KBranchD(&DeltaPhi1,"DeltaPhi1_")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi2,"DeltaPhi2"),KBranchD(&DeltaPhi2,"DeltaPhi2_")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi3,"DeltaPhi3"),KBranchD(&DeltaPhi3,"DeltaPhi3_")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi4,"DeltaPhi4"),KBranchD(&DeltaPhi4,"DeltaPhi4_"))
			};
			
			//only check branch0 since branch1 does not come from tree
			for(auto& branch : branches){
				branch->Check(looper->fChain,0);
			}
		}
		virtual void DoVariation() {
			for(auto& branch : branches){
				//store original values
				branch->Store();
			}
			
			//clear temp branches
			clear();
			TLorentzVector MHTvec; MHTvec.SetPtEtaPhiE(0,0,0,0);
			
			vector<TLorentzVector>* handles[] = {looper->Muons,looper->Electrons};
			for(unsigned j = 0; j < looper->Jets->size(); ++j){
				bool skip = false;
				for(unsigned h = 0; h < 2; ++h){
					for(unsigned ih = 0; ih < handles[h]->size(); ++ih){
						if( abs(looper->Jets->at(j).Pt() - handles[h]->at(ih).Pt())/handles[h]->at(ih).Pt() < 1 && looper->Jets->at(j).DeltaR(handles[h]->at(ih)) < 0.4) {
							skip = true;
							break;
						}
					}
					if(skip) break;
				}
				if(!skip){
					Jets.push_back(looper->Jets->at(j));
					Jets_HTMask.push_back(looper->Jets_HTMask->at(j));
					Jets_MHTMask.push_back(looper->Jets_MHTMask->at(j));
					Jets_ID.push_back(looper->Jets_ID->at(j));
					Jets_muonEnergyFraction.push_back(looper->Jets_muonEnergyFraction->at(j));
					Jets_bDiscriminatorCSV.push_back(looper->Jets_bDiscriminatorCSV->at(j));
					if(looper->Jets->at(j).Pt() > 30) JetID &= looper->Jets_ID->at(j);
					if(looper->Jets_HTMask->at(j)){
						HT += looper->Jets->at(j).Pt();
						++NJets;
						if(looper->Jets_bDiscriminatorCSV->at(j)>0.8484) ++BTags;
					}
					if(looper->Jets_MHTMask->at(j)){
						MHTvec -= looper->Jets->at(j);
					}
				}
			}
			
			MHT = MHTvec.Pt();
			MHTPhi = MHTvec.Phi();
			
			for(unsigned j = 0; j < min((unsigned)4,(unsigned)Jets.size()); ++j){
				//recalc delta phi
				if(j==0) DeltaPhi1 = abs(KMath::DeltaPhi(Jets.at(j).Phi(),MHTPhi));
				else if(j==1) DeltaPhi2 = abs(KMath::DeltaPhi(Jets.at(j).Phi(),MHTPhi));
				else if(j==2) DeltaPhi3 = abs(KMath::DeltaPhi(Jets.at(j).Phi(),MHTPhi));
				else if(j==3) DeltaPhi4 = abs(KMath::DeltaPhi(Jets.at(j).Phi(),MHTPhi));
			}
			
			for(auto& branch : branches){
				//set to new vars
				branch->Vary();
			}
		}
		virtual void UndoVariation(){
			//restore original values
			for(auto& branch : branches){
				branch->Restore();
			}
		}
		
		//helper
		void clear(){
			Jets.clear();
			Jets_HTMask.clear();
			Jets_MHTMask.clear();
			Jets_ID.clear();
			Jets_muonEnergyFraction.clear();
			Jets_bDiscriminatorCSV.clear();
			JetID = true;
			NJets = 0;
			BTags = 0;
			MHT = 0;
			MHTPhi = 0;
			HT = 0;
			DeltaPhi1 = 0;
			DeltaPhi2 = 0;
			DeltaPhi3 = 0;
			DeltaPhi4 = 0;
		}
		
		//member variables
		vector<TLorentzVector> Jets;
		vector<bool> Jets_HTMask;
		vector<bool> Jets_MHTMask;
		vector<bool> Jets_ID;
		vector<double> Jets_bDiscriminatorCSV;
		vector<double> Jets_muonEnergyFraction;
		Bool_t          JetID;
		Int_t           NJets;
		Int_t           BTags;
		Double_t        MHT;
		Double_t        MHTPhi;
		Double_t        HT;
		Double_t        DeltaPhi1;
		Double_t        DeltaPhi2;
		Double_t        DeltaPhi3;
		Double_t        DeltaPhi4;
};
REGISTER_VARIATOR(JetLepton);

#endif

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

//----------------------------------------------------
//variation of jet-related variables for Zinv cleaning
class KCleanVariator : public KVariator {
	public:
		//constructor
		KCleanVariator() : KVariator() { }
		KCleanVariator(string name_, OptionMap* localOpt_) : KVariator(name_,localOpt_)
		{
			//check options
		}
		virtual void CheckBranches(){
			//set up linked branches for clean variation
			branches = {
				new KLinkedBranchI(KBranchI(&looper->BTags,"BTags"),KBranchI(&looper->BTagsclean,"BTagsclean")),
				new KLinkedBranchI(KBranchI(&looper->BTagsDeepCSV,"BTagsDeepCSV"),KBranchI(&looper->BTagsDeepCSVclean,"BTagsDeepCSVclean")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi1,"DeltaPhi1"),KBranchD(&looper->DeltaPhi1clean,"DeltaPhi1clean")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi1Orig,"DeltaPhi1Orig"),KBranchD(&looper->DeltaPhi1cleanOrig,"DeltaPhi1cleanOrig")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi2,"DeltaPhi2"),KBranchD(&looper->DeltaPhi2clean,"DeltaPhi2clean")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi2Orig,"DeltaPhi2Orig"),KBranchD(&looper->DeltaPhi2cleanOrig,"DeltaPhi2cleanOrig")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi3,"DeltaPhi3"),KBranchD(&looper->DeltaPhi3clean,"DeltaPhi3clean")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi3Orig,"DeltaPhi3Orig"),KBranchD(&looper->DeltaPhi3cleanOrig,"DeltaPhi3cleanOrig")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi4,"DeltaPhi4"),KBranchD(&looper->DeltaPhi4clean,"DeltaPhi4clean")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi4Orig,"DeltaPhi4Orig"),KBranchD(&looper->DeltaPhi4cleanOrig,"DeltaPhi4cleanOrig")),
				new KLinkedBranchD(KBranchD(&looper->HT5,"HT5"),KBranchD(&looper->HT5clean,"HT5clean")),
				new KLinkedBranchD(KBranchD(&looper->HT,"HT"),KBranchD(&looper->HTclean,"HTclean")),
				new KLinkedBranchI(KBranchI(&looper->isoElectronTracks,"isoElectronTracks"),KBranchI(&looper->isoElectronTracksclean,"isoElectronTracksclean")),
				new KLinkedBranchI(KBranchI(&looper->isoMuonTracks,"isoMuonTracks"),KBranchI(&looper->isoMuonTracksclean,"isoMuonTracksclean")),
				new KLinkedBranchI(KBranchI(&looper->isoPionTracks,"isoPionTracks"),KBranchI(&looper->isoPionTracksclean,"isoPionTracksclean")),
				new KLinkedBranchB(KBranchB(&looper->JetIDAK8,"JetIDAK8"),KBranchB(&looper->JetIDAK8Clean,"JetIDAK8Clean")),
				new KLinkedBranchB(KBranchB(&looper->JetID,"JetID"),KBranchB(&looper->JetIDclean,"JetIDclean")),
				new KLinkedBranchVL(KBranchVL(looper->JetsAK8,"JetsAK8"),KBranchVL(looper->JetsAK8Clean,"JetsAK8Clean")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_doubleBDiscriminator,"JetsAK8_doubleBDiscriminator"),KBranchVD(looper->JetsAK8Clean_doubleBDiscriminator,"JetsAK8Clean_doubleBDiscriminator")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_ecfN2b1,"JetsAK8_ecfN2b1"),KBranchVD(looper->JetsAK8Clean_ecfN2b1,"JetsAK8Clean_ecfN2b1")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_ecfN2b2,"JetsAK8_ecfN2b2"),KBranchVD(looper->JetsAK8Clean_ecfN2b2,"JetsAK8Clean_ecfN2b2")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_ecfN3b1,"JetsAK8_ecfN3b1"),KBranchVD(looper->JetsAK8Clean_ecfN3b1,"JetsAK8Clean_ecfN3b1")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_ecfN3b2,"JetsAK8_ecfN3b2"),KBranchVD(looper->JetsAK8Clean_ecfN3b2,"JetsAK8Clean_ecfN3b2")),
				new KLinkedBranchVB(KBranchVB(looper->JetsAK8_ID,"JetsAK8_ID"),KBranchVB(looper->JetsAK8Clean_ID,"JetsAK8Clean_ID")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_NsubjettinessTau1,"JetsAK8_NsubjettinessTau1"),KBranchVD(looper->JetsAK8Clean_NsubjettinessTau1,"JetsAK8Clean_NsubjettinessTau1")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_NsubjettinessTau2,"JetsAK8_NsubjettinessTau2"),KBranchVD(looper->JetsAK8Clean_NsubjettinessTau2,"JetsAK8Clean_NsubjettinessTau2")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_NsubjettinessTau3,"JetsAK8_NsubjettinessTau3"),KBranchVD(looper->JetsAK8Clean_NsubjettinessTau3,"JetsAK8Clean_NsubjettinessTau3")),
				new KLinkedBranchVI(KBranchVI(looper->JetsAK8_NumBhadrons,"JetsAK8_NumBhadrons"),KBranchVI(looper->JetsAK8Clean_NumBhadrons,"JetsAK8Clean_NumBhadrons")),
				new KLinkedBranchVI(KBranchVI(looper->JetsAK8_NumChadrons,"JetsAK8_NumChadrons"),KBranchVI(looper->JetsAK8Clean_NumChadrons,"JetsAK8Clean_NumChadrons")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_prunedMass,"JetsAK8_prunedMass"),KBranchVD(looper->JetsAK8Clean_prunedMass,"JetsAK8Clean_prunedMass")),
				new KLinkedBranchVD(KBranchVD(looper->JetsAK8_softDropMass,"JetsAK8_softDropMass"),KBranchVD(looper->JetsAK8Clean_softDropMass,"JetsAK8Clean_softDropMass")),
				new KLinkedBranchVVL(KBranchVVL(looper->JetsAK8_subjets,"JetsAK8_subjets"),KBranchVVL(looper->JetsAK8Clean_subjets,"JetsAK8Clean_subjets")),
				new KLinkedBranchVVD(KBranchVVD(looper->JetsAK8_subjets_bDiscriminatorCSV,"JetsAK8_subjets_bDiscriminatorCSV"),KBranchVVD(looper->JetsAK8Clean_subjets_bDiscriminatorCSV,"JetsAK8Clean_subjets_bDiscriminatorCSV")),
				new KLinkedBranchVL(KBranchVL(looper->Jets,"Jets"),KBranchVL(looper->Jetsclean,"Jetsclean")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_bDiscriminatorCSV,"Jets_bDiscriminatorCSV"),KBranchVD(looper->Jetsclean_bDiscriminatorCSV,"Jetsclean_bDiscriminatorCSV")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_bJetTagDeepCSVBvsAll,"Jets_bJetTagDeepCSVBvsAll"),KBranchVD(looper->Jetsclean_bJetTagDeepCSVBvsAll,"Jetsclean_bJetTagDeepCSVBvsAll")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_bJetTagDeepCSVCvsB,"Jets_bJetTagDeepCSVCvsB"),KBranchVD(looper->Jetsclean_bJetTagDeepCSVCvsB,"Jetsclean_bJetTagDeepCSVCvsB")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_bJetTagDeepCSVCvsL,"Jets_bJetTagDeepCSVCvsL"),KBranchVD(looper->Jetsclean_bJetTagDeepCSVCvsL,"Jetsclean_bJetTagDeepCSVCvsL")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_bJetTagDeepCSVprobb,"Jets_bJetTagDeepCSVprobb"),KBranchVD(looper->Jetsclean_bJetTagDeepCSVprobb,"Jetsclean_bJetTagDeepCSVprobb")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_bJetTagDeepCSVprobbb,"Jets_bJetTagDeepCSVprobbb"),KBranchVD(looper->Jetsclean_bJetTagDeepCSVprobbb,"Jetsclean_bJetTagDeepCSVprobbb")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_bJetTagDeepCSVprobc,"Jets_bJetTagDeepCSVprobc"),KBranchVD(looper->Jetsclean_bJetTagDeepCSVprobc,"Jetsclean_bJetTagDeepCSVprobc")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_bJetTagDeepCSVprobudsg,"Jets_bJetTagDeepCSVprobudsg"),KBranchVD(looper->Jetsclean_bJetTagDeepCSVprobudsg,"Jetsclean_bJetTagDeepCSVprobudsg")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_bJetTagDeepFlavourprobb,"Jets_bJetTagDeepFlavourprobb"),KBranchVD(looper->Jetsclean_bJetTagDeepFlavourprobb,"Jetsclean_bJetTagDeepFlavourprobb")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_bJetTagDeepFlavourprobbb,"Jets_bJetTagDeepFlavourprobbb"),KBranchVD(looper->Jetsclean_bJetTagDeepFlavourprobbb,"Jetsclean_bJetTagDeepFlavourprobbb")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_bJetTagDeepFlavourprobc,"Jets_bJetTagDeepFlavourprobc"),KBranchVD(looper->Jetsclean_bJetTagDeepFlavourprobc,"Jetsclean_bJetTagDeepFlavourprobc")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_bJetTagDeepFlavourprobg,"Jets_bJetTagDeepFlavourprobg"),KBranchVD(looper->Jetsclean_bJetTagDeepFlavourprobg,"Jetsclean_bJetTagDeepFlavourprobg")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_bJetTagDeepFlavourproblepb,"Jets_bJetTagDeepFlavourproblepb"),KBranchVD(looper->Jetsclean_bJetTagDeepFlavourproblepb,"Jetsclean_bJetTagDeepFlavourproblepb")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_bJetTagDeepFlavourprobuds,"Jets_bJetTagDeepFlavourprobuds"),KBranchVD(looper->Jetsclean_bJetTagDeepFlavourprobuds,"Jetsclean_bJetTagDeepFlavourprobuds")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_chargedHadronEnergyFraction,"Jets_chargedHadronEnergyFraction"),KBranchVD(looper->Jetsclean_chargedHadronEnergyFraction,"Jetsclean_chargedHadronEnergyFraction")),
				new KLinkedBranchVI(KBranchVI(looper->Jets_hadronFlavor,"Jets_hadronFlavor"),KBranchVI(looper->Jetsclean_hadronFlavor,"Jetsclean_hadronFlavor")),
				new KLinkedBranchVB(KBranchVB(looper->Jets_HTMask,"Jets_HTMask"),KBranchVB(looper->Jetsclean_HTMask,"Jetsclean_HTMask")),
				new KLinkedBranchVB(KBranchVB(looper->Jets_ID,"Jets_ID"),KBranchVB(looper->Jetsclean_ID,"Jetsclean_ID")),
				new KLinkedBranchVB(KBranchVB(looper->Jets_ISRMask,"Jets_ISRMask"),KBranchVB(looper->Jetsclean_ISRMask,"Jetsclean_ISRMask")),
				new KLinkedBranchVB(KBranchVB(looper->Jets_MHTMask,"Jets_MHTMask"),KBranchVB(looper->Jetsclean_MHTMask,"Jetsclean_MHTMask")),
				new KLinkedBranchVB(KBranchVB(looper->Jets_MHTOrigMask,"Jets_MHTOrigMask"),KBranchVB(looper->Jetsclean_MHTOrigMask,"Jetsclean_MHTOrigMask")),
				new KLinkedBranchVD(KBranchVD(looper->Jets_muonEnergyFraction,"Jets_muonEnergyFraction"),KBranchVD(looper->Jetsclean_muonEnergyFraction,"Jetsclean_muonEnergyFraction")),
				new KLinkedBranchVI(KBranchVI(looper->Jets_partonFlavor,"Jets_partonFlavor"),KBranchVI(looper->Jetsclean_partonFlavor,"Jetsclean_partonFlavor")),
				new KLinkedBranchD(KBranchD(&looper->MET,"MET"),KBranchD(&looper->METclean,"METclean")),
				new KLinkedBranchD(KBranchD(&looper->METOrig,"METOrig"),KBranchD(&looper->METcleanOrig,"METcleanOrig")),
				new KLinkedBranchD(KBranchD(&looper->METPhi,"METPhi"),KBranchD(&looper->METPhiclean,"METPhiclean")),
				new KLinkedBranchD(KBranchD(&looper->METPhiOrig,"METPhiOrig"),KBranchD(&looper->METPhicleanOrig,"METPhicleanOrig")),
				new KLinkedBranchD(KBranchD(&looper->METSignificance,"METSignificance"),KBranchD(&looper->METSignificanceclean,"METSignificanceclean")),
				new KLinkedBranchD(KBranchD(&looper->MHT,"MHT"),KBranchD(&looper->MHTclean,"MHTclean")),
				new KLinkedBranchD(KBranchD(&looper->MHTOrig,"MHTOrig"),KBranchD(&looper->MHTcleanOrig,"MHTcleanOrig")),
				new KLinkedBranchD(KBranchD(&looper->MHTPhi,"MHTPhi"),KBranchD(&looper->MHTPhiclean,"MHTPhiclean")),
				new KLinkedBranchD(KBranchD(&looper->MHTPhiOrig,"MHTPhiOrig"),KBranchD(&looper->MHTPhicleanOrig,"MHTPhicleanOrig")),
				new KLinkedBranchI(KBranchI(&looper->NJets,"NJets"),KBranchI(&looper->NJetsclean,"NJetsclean")),
				new KLinkedBranchI(KBranchI(&looper->NJetsISR,"NJetsISR"),KBranchI(&looper->NJetsISRclean,"NJetsISRclean"))
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
};
REGISTER_VARIATOR(Clean);

//----------------------------------------------------
//variation of jet-related variables
//for: jet systematics (JECs, JERs)
class KJetVariator : public KVariator {
	public:
		enum vartypes { NoVar=0, JECup=1, JECdown=2, JERup=3, JERdown=4 };
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
		}
		virtual void CheckBranches(){
			//set up linked branches for all variations
			branches = {
				//4-vector modified "by hand"
				new KLinkedBranchVL(KBranchVL(looper->Jets,"Jets"),KBranchVL(&Jets,"Jets_")),
				//scalars get replaced
				new KLinkedBranchI(KBranchI(&looper->BTagsDeepCSV,"BTagsDeepCSV"),vtype==JECup?KBranchI(&looper->BTagsDeepCSVJECup,"BTagsDeepCSVJECup"):vtype==JECdown?KBranchI(&looper->BTagsDeepCSVJECdown,"BTagsDeepCSVJECdown"):vtype==JERup?KBranchI(&looper->BTagsDeepCSVJERup,"BTagsDeepCSVJERup"):vtype==JERdown?KBranchI(&looper->BTagsDeepCSVJERdown,"BTagsDeepCSVJERdown"):KBranchI(NULL,"")),
				new KLinkedBranchI(KBranchI(&looper->BTags,"BTags"),vtype==JECup?KBranchI(&looper->BTagsJECup,"BTagsJECup"):vtype==JECdown?KBranchI(&looper->BTagsJECdown,"BTagsJECdown"):vtype==JERup?KBranchI(&looper->BTagsJERup,"BTagsJERup"):vtype==JERdown?KBranchI(&looper->BTagsJERdown,"BTagsJERdown"):KBranchI(NULL,"")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi1,"DeltaPhi1"),vtype==JECup?KBranchD(&looper->DeltaPhi1JECup,"DeltaPhi1JECup"):vtype==JECdown?KBranchD(&looper->DeltaPhi1JECdown,"DeltaPhi1JECdown"):vtype==JERup?KBranchD(&looper->DeltaPhi1JERup,"DeltaPhi1JERup"):vtype==JERdown?KBranchD(&looper->DeltaPhi1JERdown,"DeltaPhi1JERdown"):KBranchD(NULL,"")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi2,"DeltaPhi2"),vtype==JECup?KBranchD(&looper->DeltaPhi2JECup,"DeltaPhi2JECup"):vtype==JECdown?KBranchD(&looper->DeltaPhi2JECdown,"DeltaPhi2JECdown"):vtype==JERup?KBranchD(&looper->DeltaPhi2JERup,"DeltaPhi2JERup"):vtype==JERdown?KBranchD(&looper->DeltaPhi2JERdown,"DeltaPhi2JERdown"):KBranchD(NULL,"")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi3,"DeltaPhi3"),vtype==JECup?KBranchD(&looper->DeltaPhi3JECup,"DeltaPhi3JECup"):vtype==JECdown?KBranchD(&looper->DeltaPhi3JECdown,"DeltaPhi3JECdown"):vtype==JERup?KBranchD(&looper->DeltaPhi3JERup,"DeltaPhi3JERup"):vtype==JERdown?KBranchD(&looper->DeltaPhi3JERdown,"DeltaPhi3JERdown"):KBranchD(NULL,"")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi4,"DeltaPhi4"),vtype==JECup?KBranchD(&looper->DeltaPhi4JECup,"DeltaPhi4JECup"):vtype==JECdown?KBranchD(&looper->DeltaPhi4JECdown,"DeltaPhi4JECdown"):vtype==JERup?KBranchD(&looper->DeltaPhi4JERup,"DeltaPhi4JERup"):vtype==JERdown?KBranchD(&looper->DeltaPhi4JERdown,"DeltaPhi4JERdown"):KBranchD(NULL,"")),
				new KLinkedBranchD(KBranchD(&looper->HT5,"HT5"),vtype==JECup?KBranchD(&looper->HT5JECup,"HT5JECup"):vtype==JECdown?KBranchD(&looper->HT5JECdown,"HT5JECdown"):vtype==JERup?KBranchD(&looper->HT5JERup,"HT5JERup"):vtype==JERdown?KBranchD(&looper->HT5JERdown,"HT5JERdown"):KBranchD(NULL,"")),
				new KLinkedBranchD(KBranchD(&looper->HT,"HT"),vtype==JECup?KBranchD(&looper->HTJECup,"HTJECup"):vtype==JECdown?KBranchD(&looper->HTJECdown,"HTJECdown"):vtype==JERup?KBranchD(&looper->HTJERup,"HTJERup"):vtype==JERdown?KBranchD(&looper->HTJERdown,"HTJERdown"):KBranchD(NULL,"")),
				new KLinkedBranchB(KBranchB(&looper->JetID,"JetID"),vtype==JECup?KBranchB(&looper->JetIDJECup,"JetIDJECup"):vtype==JECdown?KBranchB(&looper->JetIDJECdown,"JetIDJECdown"):vtype==JERup?KBranchB(&looper->JetIDJERup,"JetIDJERup"):vtype==JERdown?KBranchB(&looper->JetIDJERdown,"JetIDJERdown"):KBranchB(NULL,"")),
				new KLinkedBranchD(KBranchD(&looper->MHT,"MHT"),vtype==JECup?KBranchD(&looper->MHTJECup,"MHTJECup"):vtype==JECdown?KBranchD(&looper->MHTJECdown,"MHTJECdown"):vtype==JERup?KBranchD(&looper->MHTJERup,"MHTJERup"):vtype==JERdown?KBranchD(&looper->MHTJERdown,"MHTJERdown"):KBranchD(NULL,"")),
				new KLinkedBranchD(KBranchD(&looper->MHTPhi,"MHTPhi"),vtype==JECup?KBranchD(&looper->MHTPhiJECup,"MHTPhiJECup"):vtype==JECdown?KBranchD(&looper->MHTPhiJECdown,"MHTPhiJECdown"):vtype==JERup?KBranchD(&looper->MHTPhiJERup,"MHTPhiJERup"):vtype==JERdown?KBranchD(&looper->MHTPhiJERdown,"MHTPhiJERdown"):KBranchD(NULL,"")),
				new KLinkedBranchI(KBranchI(&looper->NJetsISR,"NJetsISR"),vtype==JECup?KBranchI(&looper->NJetsISRJECup,"NJetsISRJECup"):vtype==JECdown?KBranchI(&looper->NJetsISRJECdown,"NJetsISRJECdown"):vtype==JERup?KBranchI(&looper->NJetsISRJERup,"NJetsISRJERup"):vtype==JERdown?KBranchI(&looper->NJetsISRJERdown,"NJetsISRJERdown"):KBranchI(NULL,"")),
				new KLinkedBranchI(KBranchI(&looper->NJets,"NJets"),vtype==JECup?KBranchI(&looper->NJetsJECup,"NJetsJECup"):vtype==JECdown?KBranchI(&looper->NJetsJECdown,"NJetsJECdown"):vtype==JERup?KBranchI(&looper->NJetsJERup,"NJetsJERup"):vtype==JERdown?KBranchI(&looper->NJetsJERdown,"NJetsJERdown"):KBranchI(NULL,"")),
				//vectors get reordered
				new KReorderedBranchVD(KBranchVD(looper->Jets_axismajor,"Jets_axismajor"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_axisminor,"Jets_axisminor"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_bDiscriminatorCSV,"Jets_bDiscriminatorCSV"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_bJetTagDeepCSVBvsAll,"Jets_bJetTagDeepCSVBvsAll"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_bJetTagDeepCSVCvsB,"Jets_bJetTagDeepCSVCvsB"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_bJetTagDeepCSVCvsL,"Jets_bJetTagDeepCSVCvsL"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_bJetTagDeepCSVprobb,"Jets_bJetTagDeepCSVprobb"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_bJetTagDeepCSVprobbb,"Jets_bJetTagDeepCSVprobbb"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_bJetTagDeepCSVprobc,"Jets_bJetTagDeepCSVprobc"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_bJetTagDeepCSVprobudsg,"Jets_bJetTagDeepCSVprobudsg"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_bJetTagDeepFlavourprobb,"Jets_bJetTagDeepFlavourprobb"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_bJetTagDeepFlavourprobbb,"Jets_bJetTagDeepFlavourprobbb"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_bJetTagDeepFlavourprobc,"Jets_bJetTagDeepFlavourprobc"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_bJetTagDeepFlavourprobg,"Jets_bJetTagDeepFlavourprobg"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_bJetTagDeepFlavourproblepb,"Jets_bJetTagDeepFlavourproblepb"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_bJetTagDeepFlavourprobuds,"Jets_bJetTagDeepFlavourprobuds"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_chargedEmEnergyFraction,"Jets_chargedEmEnergyFraction"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_chargedHadronEnergyFraction,"Jets_chargedHadronEnergyFraction"),order),
				new KReorderedBranchVI(KBranchVI(looper->Jets_chargedHadronMultiplicity,"Jets_chargedHadronMultiplicity"),order),
				new KReorderedBranchVI(KBranchVI(looper->Jets_chargedMultiplicity,"Jets_chargedMultiplicity"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_electronEnergyFraction,"Jets_electronEnergyFraction"),order),
				new KReorderedBranchVI(KBranchVI(looper->Jets_electronMultiplicity,"Jets_electronMultiplicity"),order),
				new KReorderedBranchVI(KBranchVI(looper->Jets_hadronFlavor,"Jets_hadronFlavor"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_hfEMEnergyFraction,"Jets_hfEMEnergyFraction"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_hfHadronEnergyFraction,"Jets_hfHadronEnergyFraction"),order),
				new KReorderedBranchVB(KBranchVB(looper->Jets_HTMask,"Jets_HTMask"),order),
				new KReorderedBranchVB(KBranchVB(looper->Jets_ID,"Jets_ID"),order),
				new KReorderedBranchVB(KBranchVB(looper->Jets_ISRMask,"Jets_ISRMask"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_jecFactor,"Jets_jecFactor"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_jecUnc,"Jets_jecUnc"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_jerFactor,"Jets_jerFactor"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_jerFactorDown,"Jets_jerFactorDown"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_jerFactorUp,"Jets_jerFactorUp"),order),
				new KReorderedBranchVB(KBranchVB(looper->Jets_LeptonMask,"Jets_LeptonMask"),order),
				new KReorderedBranchVB(KBranchVB(looper->Jets_MHTMask,"Jets_MHTMask"),order),
				new KReorderedBranchVB(KBranchVB(looper->Jets_MHTOrigMask,"Jets_MHTOrigMask"),order),
				new KReorderedBranchVI(KBranchVI(looper->Jets_multiplicity,"Jets_multiplicity"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_muonEnergyFraction,"Jets_muonEnergyFraction"),order),
				new KReorderedBranchVI(KBranchVI(looper->Jets_muonMultiplicity,"Jets_muonMultiplicity"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_neutralEmEnergyFraction,"Jets_neutralEmEnergyFraction"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_neutralHadronEnergyFraction,"Jets_neutralHadronEnergyFraction"),order),
				new KReorderedBranchVI(KBranchVI(looper->Jets_neutralHadronMultiplicity,"Jets_neutralHadronMultiplicity"),order),
				new KReorderedBranchVI(KBranchVI(looper->Jets_neutralMultiplicity,"Jets_neutralMultiplicity"),order),
				new KReorderedBranchVI(KBranchVI(looper->Jets_partonFlavor,"Jets_partonFlavor"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_photonEnergyFraction,"Jets_photonEnergyFraction"),order),
				new KReorderedBranchVI(KBranchVI(looper->Jets_photonMultiplicity,"Jets_photonMultiplicity"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_ptD,"Jets_ptD"),order),
				new KReorderedBranchVD(KBranchVD(looper->Jets_qgLikelihood,"Jets_qgLikelihood"),order),
			};
			
			for(auto& branch : branches){
				//enable varied branches which might be disabled by default
				branch->Enable(looper->fChain,1);
				branch->Check(looper->fChain);
			}
			
			//used for calculations
			vector<string> unc_branches = {
				"Jets_origIndex",
				"Jets_jerFactor",
			};
			if(vtype==JECup) {
				unc_branches.insert(
					unc_branches.end(), {
						"JetsJECup_origIndex",
						"JetsJECup_jerFactor",
						"Jets_jecUnc"
					}
				);
			}
			else if(vtype==JECdown) {
				unc_branches.insert(
					unc_branches.end(), {
						"JetsJECdown_origIndex",
						"JetsJECdown_jerFactor",
						"Jets_jecUnc"
					}
				);
			}
			else if(vtype==JERup) {
				unc_branches.insert(
					unc_branches.end(), {
						"JetsJERup_origIndex",
						"Jets_jerFactorUp"
					}
				);
			}
			else if(vtype==JERdown) {
				unc_branches.insert(
					unc_branches.end(), {
						"JetsJERdown_origIndex",
						"Jets_jerFactorDown"
					}
				);
			}
			
			for(auto& branch : unc_branches){
				//enable unc branches which might be disabled by default
				looper->fChain->SetBranchStatus(branch.c_str(),1);
			}
		}
		//functions
		virtual void DoVariation() {
			for(auto& branch : branches){
				//store original values
				branch->Store();
			}
			
			//recompute 4-vector w/ desired unc
			Jets.clear();
			order.clear();
			
			//index of common ancestor (before JER smearing)
			const auto& Jets_origIndex = *looper->Jets_origIndex;
			vector<int> newIndex(Jets_origIndex.size(),-1);
			for(unsigned k = 0; k < Jets_origIndex.size(); ++k){
				//reverse the index vector
				newIndex[Jets_origIndex[k]] = k;
			}
			
			const auto& Jets_orig = *looper->Jets;
			const auto& Jets_jerFactor = *looper->Jets_jerFactor;
			Jets.reserve(Jets_orig.size());
			order.reserve(Jets_orig.size());
			
			//get branches depending on which uncertainty type is chosen
			const auto& JetsUnc_origIndex = vtype==JECup?*looper->JetsJECup_origIndex:vtype==JECdown?*looper->JetsJECdown_origIndex:vtype==JERup?*looper->JetsJERup_origIndex:vtype==JERdown?*looper->JetsJERdown_origIndex:*looper->Jets_origIndex; //last one is a dummy value
			const auto& JetsUnc_jerFactor = vtype==JECup?*looper->JetsJECup_jerFactor:vtype==JECdown?*looper->JetsJECdown_jerFactor:*looper->Jets_jerFactor; //last one is a dummy value
			const auto& Jets_unc = vtype==JECup?*looper->Jets_jecUnc:vtype==JECdown?*looper->Jets_jecUnc:vtype==JERup?*looper->Jets_jerFactorUp:vtype==JERdown?*looper->Jets_jerFactorDown:*looper->Jets_jecFactor; //last one is a dummy value
			for(unsigned j = 0; j < JetsUnc_origIndex.size(); ++j){
				//Jets[Unc]_origIndex is sorted in the final order after uncertainty variation is applied
				//go up to common ancestor, then down to central smeared collection
				int i = newIndex[JetsUnc_origIndex[j]];
				order.push_back(i);
				
				//undo central smearing, apply JEC unc, redo smearing w/ new smearing factor
				     if(vtype==JECup)   Jets.push_back(Jets_orig[i]*(1./Jets_jerFactor[i])*(1+Jets_unc[i])*JetsUnc_jerFactor[j]);
				else if(vtype==JECdown) Jets.push_back(Jets_orig[i]*(1./Jets_jerFactor[i])*(1-Jets_unc[i])*JetsUnc_jerFactor[j]);
				else if(vtype==JERup)   Jets.push_back(Jets_orig[i]*(1./Jets_jerFactor[i])*Jets_unc[i]);
				else if(vtype==JERdown) Jets.push_back(Jets_orig[i]*(1./Jets_jerFactor[i])*Jets_unc[i]);
			}
			
			for(auto& branch : branches){
				//set to variation
				branch->Vary();
			}
		}
		virtual void UndoVariation() {
			//restore original values
			for(auto& branch : branches){
				branch->Restore();
			}
		}
		
		//member variables
		vartypes vtype;
		vector<unsigned> order;
		vector<TLorentzVector> Jets;
};
REGISTER_VARIATOR(Jet);

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

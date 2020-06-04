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
		virtual void ListBranches(){
			//set up linked branches for clean variation
			linkbranches = {
				new KLinkedBranchI(KBranchI(&looper->BTags,"BTags"),KBranchI(&looper->BTagsclean,"BTagsclean")),
				new KLinkedBranchI(KBranchI(&looper->BTagsDeepCSV,"BTagsDeepCSV"),KBranchI(&looper->BTagsDeepCSVclean,"BTagsDeepCSVclean")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi1,"DeltaPhi1"),KBranchD(&looper->DeltaPhi1clean,"DeltaPhi1clean")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi2,"DeltaPhi2"),KBranchD(&looper->DeltaPhi2clean,"DeltaPhi2clean")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi3,"DeltaPhi3"),KBranchD(&looper->DeltaPhi3clean,"DeltaPhi3clean")),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi4,"DeltaPhi4"),KBranchD(&looper->DeltaPhi4clean,"DeltaPhi4clean")),
				new KLinkedBranchD(KBranchD(&looper->HT5,"HT5"),KBranchD(&looper->HT5clean,"HT5clean")),
				new KLinkedBranchD(KBranchD(&looper->HT,"HT"),KBranchD(&looper->HTclean,"HTclean")),
				new KLinkedBranchI(KBranchI(&looper->isoElectronTracks,"isoElectronTracks"),KBranchI(&looper->isoElectronTracksclean,"isoElectronTracksclean")),
				new KLinkedBranchI(KBranchI(&looper->isoMuonTracks,"isoMuonTracks"),KBranchI(&looper->isoMuonTracksclean,"isoMuonTracksclean")),
				new KLinkedBranchI(KBranchI(&looper->isoPionTracks,"isoPionTracks"),KBranchI(&looper->isoPionTracksclean,"isoPionTracksclean")),
				new KLinkedBranchB(KBranchB(&looper->JetIDAK8,"JetIDAK8"),KBranchB(&looper->JetIDAK8Clean,"JetIDAK8Clean")),
				new KLinkedBranchB(KBranchB(&looper->JetID,"JetID"),KBranchB(&looper->JetIDclean,"JetIDclean")),
				new KLinkedBranchVL(KBranchVL(&looper->JetsAK8,"JetsAK8"),KBranchVL(&looper->JetsAK8Clean,"JetsAK8Clean")),
				new KLinkedBranchVD(KBranchVD(&looper->JetsAK8_doubleBDiscriminator,"JetsAK8_doubleBDiscriminator"),KBranchVD(&looper->JetsAK8Clean_doubleBDiscriminator,"JetsAK8Clean_doubleBDiscriminator")),
				new KLinkedBranchVB(KBranchVB(&looper->JetsAK8_ID,"JetsAK8_ID"),KBranchVB(&looper->JetsAK8Clean_ID,"JetsAK8Clean_ID")),
				new KLinkedBranchVD(KBranchVD(&looper->JetsAK8_softDropMass,"JetsAK8_softDropMass"),KBranchVD(&looper->JetsAK8Clean_softDropMass,"JetsAK8Clean_softDropMass")),
				new KLinkedBranchVVL(KBranchVVL(&looper->JetsAK8_subjets,"JetsAK8_subjets"),KBranchVVL(&looper->JetsAK8Clean_subjets,"JetsAK8Clean_subjets")),
				new KLinkedBranchVL(KBranchVL(&looper->Jets,"Jets"),KBranchVL(&looper->Jetsclean,"Jetsclean")),
				new KLinkedBranchVD(KBranchVD(&looper->Jets_bDiscriminatorCSV,"Jets_bDiscriminatorCSV"),KBranchVD(&looper->Jetsclean_bDiscriminatorCSV,"Jetsclean_bDiscriminatorCSV")),
				new KLinkedBranchVD(KBranchVD(&looper->Jets_bJetTagDeepCSVBvsAll,"Jets_bJetTagDeepCSVBvsAll"),KBranchVD(&looper->Jetsclean_bJetTagDeepCSVBvsAll,"Jetsclean_bJetTagDeepCSVBvsAll")),
				new KLinkedBranchVI(KBranchVI(&looper->Jets_hadronFlavor,"Jets_hadronFlavor"),KBranchVI(&looper->Jetsclean_hadronFlavor,"Jetsclean_hadronFlavor")),
				new KLinkedBranchVB(KBranchVB(&looper->Jets_HTMask,"Jets_HTMask"),KBranchVB(&looper->Jetsclean_HTMask,"Jetsclean_HTMask")),
				new KLinkedBranchVB(KBranchVB(&looper->Jets_ID,"Jets_ID"),KBranchVB(&looper->Jetsclean_ID,"Jetsclean_ID")),
				new KLinkedBranchVI(KBranchVI(&looper->Jets_partonFlavor,"Jets_partonFlavor"),KBranchVI(&looper->Jetsclean_partonFlavor,"Jetsclean_partonFlavor")),
				new KLinkedBranchD(KBranchD(&looper->MET,"MET"),KBranchD(&looper->METclean,"METclean")),
				new KLinkedBranchD(KBranchD(&looper->METPhi,"METPhi"),KBranchD(&looper->METPhiclean,"METPhiclean")),
				new KLinkedBranchD(KBranchD(&looper->MHT,"MHT"),KBranchD(&looper->MHTclean,"MHTclean")),
				new KLinkedBranchD(KBranchD(&looper->MHTPhi,"MHTPhi"),KBranchD(&looper->MHTPhiclean,"MHTPhiclean")),
				new KLinkedBranchI(KBranchI(&looper->NJets,"NJets"),KBranchI(&looper->NJetsclean,"NJetsclean")),
			};
		}
		//functions
		virtual void DoVariation() {
			//special case: if Jetsclean is empty, no reclustering was done (nothing removed from event), don't bother varying
			if(looper->Jetsclean->size()==0) return;
			for(auto& branch : linkbranches){
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
			for(auto& branch : linkbranches){
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
		enum vartypes { NoVar=0, JECup=1, JECdown=2, JERup=3, JERdown=4, JESup=5, JESdown=6 };
		//constructor
		KJetVariator() : KVariator() { }
		KJetVariator(string name_, OptionMap* localOpt_) : KVariator(name_,localOpt_), vtype(NoVar), Jets(new vector<TLorentzVector>), JetsAK8(new vector<TLorentzVector>)
		{
			//check options
			string vname = "";
			localOpt->Get("vartype",vname);
			if(vname=="JECup") vtype = JECup;
			else if(vname=="JECdown") vtype = JECdown;
			else if(vname=="JERup") vtype = JERup;
			else if(vname=="JERdown") vtype = JERdown;
			else if(vname=="JESup") vtype = JESup;
			else if(vname=="JESdown") vtype = JESdown;

			//check JES-specific options
			if(vtype==JESup or vtype==JESdown){
				localOpt->Get("AK8factor",AK8factor);
				localOpt->Get("AK4factor",AK4factor);
				localOpt->Get("njets",njets);
			}
		}
		~KJetVariator() {
			delete Jets;
		}
		virtual void ListBranches(){
			//set up linked branches for all variations
			linkbranches = {
				//4-vector modified "by hand"
				new KLinkedBranchVL(KBranchVL(&looper->Jets,"Jets"),KBranchVL(&Jets)),
				//scalars get replaced (todo: implement JES recalculation for these)
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
				new KReorderedBranchVD(KBranchVD(&looper->Jets_axismajor,"Jets_axismajor"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_axisminor,"Jets_axisminor"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_bDiscriminatorCSV,"Jets_bDiscriminatorCSV"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_bJetTagDeepCSVBvsAll,"Jets_bJetTagDeepCSVBvsAll"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_chargedEmEnergyFraction,"Jets_chargedEmEnergyFraction"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_chargedHadronEnergyFraction,"Jets_chargedHadronEnergyFraction"),order),
				new KReorderedBranchVI(KBranchVI(&looper->Jets_chargedHadronMultiplicity,"Jets_chargedHadronMultiplicity"),order),
				new KReorderedBranchVI(KBranchVI(&looper->Jets_chargedMultiplicity,"Jets_chargedMultiplicity"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_electronEnergyFraction,"Jets_electronEnergyFraction"),order),
				new KReorderedBranchVI(KBranchVI(&looper->Jets_electronMultiplicity,"Jets_electronMultiplicity"),order),
				new KReorderedBranchVI(KBranchVI(&looper->Jets_hadronFlavor,"Jets_hadronFlavor"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_hfEMEnergyFraction,"Jets_hfEMEnergyFraction"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_hfHadronEnergyFraction,"Jets_hfHadronEnergyFraction"),order),
				new KReorderedBranchVB(KBranchVB(&looper->Jets_HTMask,"Jets_HTMask"),order),
				new KReorderedBranchVB(KBranchVB(&looper->Jets_ID,"Jets_ID"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_jecFactor,"Jets_jecFactor"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_jecUnc,"Jets_jecUnc"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_jerFactor,"Jets_jerFactor"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_jerFactorDown,"Jets_jerFactorDown"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_jerFactorUp,"Jets_jerFactorUp"),order),
				new KReorderedBranchVB(KBranchVB(&looper->Jets_LeptonMask,"Jets_LeptonMask"),order),
				new KReorderedBranchVB(KBranchVB(&looper->Jets_MHTMask,"Jets_MHTMask"),order),
				new KReorderedBranchVI(KBranchVI(&looper->Jets_multiplicity,"Jets_multiplicity"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_muonEnergyFraction,"Jets_muonEnergyFraction"),order),
				new KReorderedBranchVI(KBranchVI(&looper->Jets_muonMultiplicity,"Jets_muonMultiplicity"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_neutralEmEnergyFraction,"Jets_neutralEmEnergyFraction"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_neutralHadronEnergyFraction,"Jets_neutralHadronEnergyFraction"),order),
				new KReorderedBranchVI(KBranchVI(&looper->Jets_neutralHadronMultiplicity,"Jets_neutralHadronMultiplicity"),order),
				new KReorderedBranchVI(KBranchVI(&looper->Jets_neutralMultiplicity,"Jets_neutralMultiplicity"),order),
				new KReorderedBranchVI(KBranchVI(&looper->Jets_partonFlavor,"Jets_partonFlavor"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_photonEnergyFraction,"Jets_photonEnergyFraction"),order),
				new KReorderedBranchVI(KBranchVI(&looper->Jets_photonMultiplicity,"Jets_photonMultiplicity"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_ptD,"Jets_ptD"),order),
				new KReorderedBranchVD(KBranchVD(&looper->Jets_qgLikelihood,"Jets_qgLikelihood"),order),
				//4-vector modified "by hand"
				new KLinkedBranchVL(KBranchVL(&looper->JetsAK8,"JetsAK8"),KBranchVL(&JetsAK8)),
				//scalars get replaced
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi1_AK8,"DeltaPhi1_AK8"),KBranchD(&DeltaPhi1_AK8)),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi2_AK8,"DeltaPhi2_AK8"),KBranchD(&DeltaPhi2_AK8)),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhiMin_AK8,"DeltaPhiMin_AK8"),KBranchD(&DeltaPhiMin_AK8)),
				new KLinkedBranchD(KBranchD(&looper->MJJ_AK8,"MJJ_AK8"),KBranchD(&MJJ_AK8)),
				new KLinkedBranchD(KBranchD(&looper->MT_AK8,"MT_AK8"),KBranchD(&MT_AK8)),
				new KLinkedBranchD(KBranchD(&looper->Mmc_AK8,"Mmc_AK8"),KBranchD(&Mmc_AK8)),
				new KLinkedBranchD(KBranchD(&looper->MET,"MET"),KBranchD(&MET)),
				new KLinkedBranchD(KBranchD(&looper->METPhi,"METPhi"),KBranchD(&METPhi)),
				new KLinkedBranchB(KBranchB(&looper->JetIDAK8,"JetIDAK8"),vtype==JECup?KBranchB(&looper->JetIDAK8JECup,"JetIDAK8JECup"):vtype==JECdown?KBranchB(&looper->JetIDAK8JECdown,"JetIDAK8JECdown"):vtype==JERup?KBranchB(&looper->JetIDAK8JERup,"JetIDAK8JERup"):vtype==JERdown?KBranchB(&looper->JetIDAK8JERdown,"JetIDAK8JERdown"):KBranchB(NULL,"")),
				//vectors get reordered
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_axismajor,"JetsAK8_axismajor"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_axisminor,"JetsAK8_axisminor"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_chargedEmEnergyFraction,"JetsAK8_chargedEmEnergyFraction"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_chargedHadronEnergyFraction,"JetsAK8_chargedHadronEnergyFraction"),orderAK8),
				new KReorderedBranchVI(KBranchVI(&looper->JetsAK8_chargedHadronMultiplicity,"JetsAK8_chargedHadronMultiplicity"),orderAK8),
				new KReorderedBranchVI(KBranchVI(&looper->JetsAK8_chargedMultiplicity,"JetsAK8_chargedMultiplicity"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepMassDecorrelTagbbvsLight,"JetsAK8_DeepMassDecorrelTagbbvsLight"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepMassDecorrelTagHbbvsQCD,"JetsAK8_DeepMassDecorrelTagHbbvsQCD"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepMassDecorrelTagTvsQCD,"JetsAK8_DeepMassDecorrelTagTvsQCD"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepMassDecorrelTagWvsQCD,"JetsAK8_DeepMassDecorrelTagWvsQCD"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepMassDecorrelTagZbbvsQCD,"JetsAK8_DeepMassDecorrelTagZbbvsQCD"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepMassDecorrelTagZHbbvsQCD,"JetsAK8_DeepMassDecorrelTagZHbbvsQCD"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepMassDecorrelTagZvsQCD,"JetsAK8_DeepMassDecorrelTagZvsQCD"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepTagHbbvsQCD,"JetsAK8_DeepTagHbbvsQCD"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepTagTvsQCD,"JetsAK8_DeepTagTvsQCD"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepTagWvsQCD,"JetsAK8_DeepTagWvsQCD"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepTagZbbvsQCD,"JetsAK8_DeepTagZbbvsQCD"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepTagZvsQCD,"JetsAK8_DeepTagZvsQCD"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_doubleBDiscriminator,"JetsAK8_doubleBDiscriminator"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_ecfN2b1,"JetsAK8_ecfN2b1"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_ecfN2b2,"JetsAK8_ecfN2b2"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_ecfN3b1,"JetsAK8_ecfN3b1"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_ecfN3b2,"JetsAK8_ecfN3b2"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_electronEnergyFraction,"JetsAK8_electronEnergyFraction"),orderAK8),
				new KReorderedBranchVI(KBranchVI(&looper->JetsAK8_electronMultiplicity,"JetsAK8_electronMultiplicity"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_girth,"JetsAK8_girth"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_hfEMEnergyFraction,"JetsAK8_hfEMEnergyFraction"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_hfHadronEnergyFraction,"JetsAK8_hfHadronEnergyFraction"),orderAK8),
				new KReorderedBranchVB(KBranchVB(&looper->JetsAK8_ID,"JetsAK8_ID"),orderAK8),
				new KReorderedBranchVB(KBranchVB(&looper->JetsAK8_isHV,"JetsAK8_isHV"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_jecFactor,"JetsAK8_jecFactor"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_jecUnc,"JetsAK8_jecUnc"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_jerFactor,"JetsAK8_jerFactor"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_jerFactorDown,"JetsAK8_jerFactorDown"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_jerFactorUp,"JetsAK8_jerFactorUp"),orderAK8),
				new KReorderedBranchVI(KBranchVI(&looper->JetsAK8_multiplicity,"JetsAK8_multiplicity"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_muonEnergyFraction,"JetsAK8_muonEnergyFraction"),orderAK8),
				new KReorderedBranchVI(KBranchVI(&looper->JetsAK8_muonMultiplicity,"JetsAK8_muonMultiplicity"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_neutralEmEnergyFraction,"JetsAK8_neutralEmEnergyFraction"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_neutralHadronEnergyFraction,"JetsAK8_neutralHadronEnergyFraction"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_neutralHadronMultiplicity,"JetsAK8_neutralHadronMultiplicity"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_neutralMultiplicity,"JetsAK8_neutralMultiplicity"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_NsubjettinessTau1,"JetsAK8_NsubjettinessTau1"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_NsubjettinessTau2,"JetsAK8_NsubjettinessTau2"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_NsubjettinessTau3,"JetsAK8_NsubjettinessTau3"),orderAK8),
				new KReorderedBranchVI(KBranchVI(&looper->JetsAK8_NumBhadrons,"JetsAK8_NumBhadrons"),orderAK8),
				new KReorderedBranchVI(KBranchVI(&looper->JetsAK8_NumChadrons,"JetsAK8_NumChadrons"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb,"JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_photonEnergyFraction,"JetsAK8_photonEnergyFraction"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_photonMultiplicity,"JetsAK8_photonMultiplicity"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_ptD,"JetsAK8_ptD"),orderAK8),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_softDropMass,"JetsAK8_softDropMass"),orderAK8),
				new KReorderedBranchVVL(KBranchVVL(&looper->JetsAK8_subjets,"JetsAK8_subjets"),orderAK8),
				new KReorderedBranchVVD(KBranchVVD(&looper->JetsAK8_subjets_axismajor,"JetsAK8_subjets_axismajor"),orderAK8),
				new KReorderedBranchVVD(KBranchVVD(&looper->JetsAK8_subjets_axisminor,"JetsAK8_subjets_axisminor"),orderAK8),
				new KReorderedBranchVVD(KBranchVVD(&looper->JetsAK8_subjets_bDiscriminatorCSV,"JetsAK8_subjets_bDiscriminatorCSV"),orderAK8),
				new KReorderedBranchVVI(KBranchVVI(&looper->JetsAK8_subjets_multiplicity,"JetsAK8_subjets_multiplicity"),orderAK8),
				new KReorderedBranchVVD(KBranchVVD(&looper->JetsAK8_subjets_ptD,"JetsAK8_subjets_ptD"),orderAK8),
			};
			
			//used for calculations
			branches = {
				"Jets_origIndex",
				"Jets_jerFactor",
				"JetsAK8_origIndex",
				"JetsAK8_jerFactor",
			};
			if(vtype==JECup) {
				branches.insert(
					branches.end(), {
						"JetsJECup_origIndex",
						"JetsJECup_jerFactor",
						"Jets_jecUnc",
						"JetsAK8JECup_origIndex",
						"JetsAK8JECup_jerFactor",
						"JetsAK8_jecUnc",
						"METUp",
						"METPhiUp",
					}
				);
			}
			else if(vtype==JECdown) {
				branches.insert(
					branches.end(), {
						"JetsJECdown_origIndex",
						"JetsJECdown_jerFactor",
						"Jets_jecUnc",
						"JetsAK8JECdown_origIndex",
						"JetsAK8JECdown_jerFactor",
						"JetsAK8_jecUnc",
						"METDown",
						"METPhiDown",
					}
				);
			}
			else if(vtype==JERup) {
				branches.insert(
					branches.end(), {
						"JetsJERup_origIndex",
						"Jets_jerFactorUp",
						"JetsAK8JERup_origIndex",
						"JetsAK8_jerFactorUp",
						"METUp",
						"METPhiUp",
					}
				);
			}
			else if(vtype==JERdown) {
				branches.insert(
					branches.end(), {
						"JetsJERdown_origIndex",
						"Jets_jerFactorDown",
						"JetsAK8JERdown_origIndex",
						"JetsAK8_jerFactorDown",
						"METDown",
						"METPhiDown",
					}
				);
			}
		}
		//functions
		virtual void DoVariation() {
			for(auto& branch : linkbranches){
				//store original values
				branch->Store();
			}

			clear();

			const auto& Jets_orig = *looper->Jets;
			const auto& JetsAK8_orig = *looper->JetsAK8;
			if(vtype==JESup or vtype==JESdown){
				TLorentzVector METVec; METVec.SetPtEtaPhiE(looper->MET,0,looper->METPhi,looper->MET);

				ScaleJetsMET(Jets_orig, AK4factor, njets, Jets, order, METVec, true);
				//only modify MET based on AK4 jets
				ScaleJetsMET(JetsAK8_orig, AK8factor, njets, JetsAK8, orderAK8, METVec);

				MET = METVec.Pt();
				METPhi = METVec.Phi();
			}
			else {
				//recompute 4-vector w/ desired unc
				const auto& Jets_origIndex = *looper->Jets_origIndex;
				const auto& Jets_jerFactor = *looper->Jets_jerFactor;

				//get branches depending on which uncertainty type is chosen
				const auto& JetsUnc_origIndex = vtype==JECup?*looper->JetsJECup_origIndex:vtype==JECdown?*looper->JetsJECdown_origIndex:vtype==JERup?*looper->JetsJERup_origIndex:vtype==JERdown?*looper->JetsJERdown_origIndex:*looper->Jets_origIndex; //last one is a dummy value
				const auto& JetsUnc_jerFactor = vtype==JECup?*looper->JetsJECup_jerFactor:vtype==JECdown?*looper->JetsJECdown_jerFactor:*looper->Jets_jerFactor; //last one is a dummy value
				const auto& Jets_unc = vtype==JECup?*looper->Jets_jecUnc:vtype==JECdown?*looper->Jets_jecUnc:vtype==JERup?*looper->Jets_jerFactorUp:vtype==JERdown?*looper->Jets_jerFactorDown:*looper->Jets_jecFactor; //last one is a dummy value

				RecomputeJets(Jets_origIndex, Jets_jerFactor, Jets_orig, JetsUnc_origIndex, JetsUnc_jerFactor, Jets_unc, Jets, order);

				//now the same for AK8
				const auto& JetsAK8_origIndex = *looper->JetsAK8_origIndex;
				const auto& JetsAK8_jerFactor = *looper->JetsAK8_jerFactor;

				//get branches depending on which uncertainty type is chosen
				const auto& JetsAK8Unc_origIndex = vtype==JECup?*looper->JetsAK8JECup_origIndex:vtype==JECdown?*looper->JetsAK8JECdown_origIndex:vtype==JERup?*looper->JetsAK8JERup_origIndex:vtype==JERdown?*looper->JetsAK8JERdown_origIndex:*looper->JetsAK8_origIndex; //last one is a dummy value
				const auto& JetsAK8Unc_jerFactor = vtype==JECup?*looper->JetsAK8JECup_jerFactor:vtype==JECdown?*looper->JetsAK8JECdown_jerFactor:*looper->JetsAK8_jerFactor; //last one is a dummy value
				const auto& JetsAK8_unc = vtype==JECup?*looper->JetsAK8_jecUnc:vtype==JECdown?*looper->JetsAK8_jecUnc:vtype==JERup?*looper->JetsAK8_jerFactorUp:vtype==JERdown?*looper->JetsAK8_jerFactorDown:*looper->JetsAK8_jecFactor; //last one is a dummy value

				RecomputeJets(JetsAK8_origIndex, JetsAK8_jerFactor, JetsAK8_orig, JetsAK8Unc_origIndex, JetsAK8Unc_jerFactor, JetsAK8_unc, JetsAK8, orderAK8);

				//vary MET coherently			
				MET = vtype==JECup?looper->METUp->at(1):vtype==JECdown?looper->METDown->at(1):vtype==JERup?looper->METUp->at(0):vtype==JERdown?looper->METDown->at(0):looper->MET;
				METPhi = vtype==JECup?looper->METPhiUp->at(1):vtype==JECdown?looper->METPhiDown->at(1):vtype==JERup?looper->METPhiUp->at(0):vtype==JERdown?looper->METPhiDown->at(0):looper->METPhi;
			}

			//recompute scalars by hand
			TLorentzVector vjj;
			int counter = 0;
			for(unsigned j = 0; j < JetsAK8->size(); ++j){
				if(counter>=2) break;
				if(counter<2) {
					vjj += JetsAK8->at(j);
					++counter;
				}
				if(DeltaPhi1_AK8>9) DeltaPhi1_AK8 = abs(KMath::DeltaPhi(JetsAK8->at(j).Phi(),METPhi));
				else if(DeltaPhi2_AK8>9) DeltaPhi2_AK8 = abs(KMath::DeltaPhi(JetsAK8->at(j).Phi(),METPhi));
			}

			//check for 2 jets
			DeltaPhiMin_AK8 = min(DeltaPhi1_AK8,DeltaPhi2_AK8);
			if(counter>=2){
				MJJ_AK8 = vjj.M();
				MT_AK8 = KMath::TransverseMass(vjj.Px(),vjj.Py(),vjj.M(),MET*cos(METPhi),MET*sin(METPhi),0);
				//skipping Mmc for now
			}

			for(auto& branch : linkbranches){
				//set to variation
				branch->Vary();
			}
		}
		virtual void ScaleJetsMET(const vector<TLorentzVector>& Jets_orig, double factor, int n,
								  vector<TLorentzVector>* theJets, vector<unsigned>& theOrder, TLorentzVector& theMET, bool doMET=false)
		{
			theJets->reserve(Jets_orig.size());
			theOrder.reserve(Jets_orig.size());
			TLorentzVector v_old, v_new;
			if(n<0) n = Jets_orig.size();
			for(unsigned j = 0; j < Jets_orig.size(); ++j){
				if(j<unsigned(n)){
					theJets->push_back(Jets_orig[j]*factor);
					v_old += Jets_orig[j];
					v_new += theJets->back();
				}
				else theJets->push_back(Jets_orig[j]);
			}

			//get sorted indices
			theOrder = vector<unsigned>(theJets->size());
			iota(theOrder.begin(),theOrder.end(),0);
			sort(theOrder.begin(),theOrder.end(), [&](unsigned i, unsigned j){ return theJets->at(i).Pt() > theJets->at(j).Pt(); });

			//apply sort to jets
			KMath::apply_permutation_in_place(*theJets, theOrder);

			//propagate to MET
			if(doMET) theMET = theMET + v_old - v_new;
		}
		virtual void RecomputeJets(const vector<int>& Jets_origIndex, const vector<double>& Jets_jerFactor, const vector<TLorentzVector>& Jets_orig,
								   const vector<int>& JetsUnc_origIndex, const vector<double>& JetsUnc_jerFactor, const vector<double>& Jets_unc,
								   vector<TLorentzVector>* theJets, vector<unsigned>& theOrder)
		{
			//index of common ancestor (before JER smearing)
			vector<int> newIndex(Jets_origIndex.size(),-1);
			for(unsigned k = 0; k < Jets_origIndex.size(); ++k){
				//reverse the index vector
				newIndex[Jets_origIndex[k]] = k;
			}
			
			theJets->reserve(Jets_orig.size());
			theOrder.reserve(Jets_orig.size());
			
			for(unsigned j = 0; j < JetsUnc_origIndex.size(); ++j){
				//Jets[Unc]_origIndex is sorted in the final order after uncertainty variation is applied
				//go up to common ancestor, then down to central smeared collection
				int i = newIndex[JetsUnc_origIndex[j]];
				theOrder.push_back(i);
				
				//undo central smearing, apply JEC unc, redo smearing w/ new smearing factor
				     if(vtype==JECup)   theJets->push_back(Jets_orig[i]*(1./Jets_jerFactor[i])*(1+Jets_unc[i])*JetsUnc_jerFactor[j]);
				else if(vtype==JECdown) theJets->push_back(Jets_orig[i]*(1./Jets_jerFactor[i])*(1-Jets_unc[i])*JetsUnc_jerFactor[j]);
				else if(vtype==JERup)   theJets->push_back(Jets_orig[i]*(1./Jets_jerFactor[i])*Jets_unc[i]);
				else if(vtype==JERdown) theJets->push_back(Jets_orig[i]*(1./Jets_jerFactor[i])*Jets_unc[i]);
			}
			
		}
		virtual void UndoVariation() {
			//restore original values
			for(auto& branch : linkbranches){
				branch->Restore();
			}
		}
		//helper
		void clear(){
			Jets->clear();
			order.clear();
		
			JetsAK8->clear();
			orderAK8.clear();
		
			DeltaPhi1_AK8 = 10;
			DeltaPhi2_AK8 = 10;
			DeltaPhiMin_AK8 = 10;
			MJJ_AK8 = 0;
			MT_AK8 = 0;
			Mmc_AK8 = 0;
			MET = 0;
			METPhi = 0;
		}
		
		//member variables
		vartypes vtype;
		double AK8factor, AK4factor;
		int njets;
		vector<unsigned> order, orderAK8;
		vector<TLorentzVector> *Jets, *JetsAK8;
		double DeltaPhi1_AK8;
		double DeltaPhi2_AK8;
		double DeltaPhiMin_AK8;
		double MJJ_AK8;
		double Mmc_AK8;
		double MT_AK8;
		double MET;
		double METPhi;
};
REGISTER_VARIATOR(Jet);

class KLepFracVariator : public KVariator {
	public:
		//constructor
		KLepFracVariator() : KVariator() { }
		KLepFracVariator(string name_, OptionMap* localOpt_) : KVariator(name_,localOpt_),
			JetsAK8_chargedHadronEnergyFraction(new vector<double>),
			JetsAK8_photonEnergyFraction(new vector<double>),
			JetsAK8_muonEnergyFraction(new vector<double>),
			JetsAK8_electronEnergyFraction(new vector<double>)
		{
			loose = localOpt->Get("loose",false);
		}
		~KLepFracVariator() {
			delete JetsAK8_chargedHadronEnergyFraction;
			delete JetsAK8_photonEnergyFraction;
			delete JetsAK8_muonEnergyFraction;
			delete JetsAK8_electronEnergyFraction;
		}
		virtual void ListBranches(){
			linkbranches = {
				new KLinkedBranchVD(KBranchVD(&looper->JetsAK8_chargedHadronEnergyFraction,"JetsAK8_chargedHadronEnergyFraction"),KBranchVD(&JetsAK8_chargedHadronEnergyFraction)),
				new KLinkedBranchVD(KBranchVD(&looper->JetsAK8_photonEnergyFraction,"JetsAK8_photonEnergyFraction"),KBranchVD(&JetsAK8_photonEnergyFraction)),
				new KLinkedBranchVD(KBranchVD(&looper->JetsAK8_muonEnergyFraction,"JetsAK8_muonEnergyFraction"),KBranchVD(&JetsAK8_muonEnergyFraction)),
				new KLinkedBranchVD(KBranchVD(&looper->JetsAK8_electronEnergyFraction,"JetsAK8_electronEnergyFraction"),KBranchVD(&JetsAK8_electronEnergyFraction)),
			};
			//used for calculations
			branches = {
				"Muons",
				"Muons_passIso",
				"Muons_mediumID",
				"Muons_iso",
				"Electrons_passIso",
				"JetsAK8",
				"JetsAK8_jecFactor",
				"JetsAK8_jerFactor"
			};
		}
		virtual void CheckBranches() {
			KVariator::CheckBranches();
			//check JER status (not present for data)
			auto jer_branch = KBranchVD(&looper->JetsAK8_jerFactor,"JetsAK8_jerFactor");
			jer_branch.Check(looper->fChain);
			has_jer = jer_branch.status;
		}
		virtual void DoVariation() {
			for(auto& branch : linkbranches){
				//store original values
				branch->Store();
			}

			//clear temp branches
			clear();

			//copy values
			*JetsAK8_chargedHadronEnergyFraction = *looper->JetsAK8_chargedHadronEnergyFraction;
			*JetsAK8_photonEnergyFraction = *looper->JetsAK8_photonEnergyFraction;
			*JetsAK8_muonEnergyFraction = *looper->JetsAK8_muonEnergyFraction;
			*JetsAK8_electronEnergyFraction = *looper->JetsAK8_electronEnergyFraction;

			//find leptons
			vector<bool> Muons_mask(looper->Muons->size(),false);
			if(loose){
				for(unsigned m = 0; m < looper->Muons->size(); ++m){
					if(looper->Muons_iso->at(m) < 0.4) Muons_mask[m] = true;
				}
			}
			else {
				Muons_mask = KMath::vector_and(*looper->Muons_passIso,*looper->Muons_mediumID);
			}
			vector<bool> Electrons_mask = *looper->Electrons_passIso;

			//fix energy fractions as though found leptons were lost
			//match reco lepton to jet, take fraction (using raw jet p4), subtract from relevant lep frac, add half to chHad and half to photon
			for(unsigned j = 0; j < looper->JetsAK8->size(); ++j){
				const auto& jet = looper->JetsAK8->at(j);

				//use raw jet (uncorrected, unsmeared) for energy fraction denominator
				double rawEnergy = jet.E();
				rawEnergy /= looper->JetsAK8_jecFactor->at(j);
				if(has_jer) rawEnergy /= looper->JetsAK8_jerFactor->at(j);

				//find highest-pt lepton w/ dR < 0.8
				double match_pt = 0;
				int match_muon = -1;
				int match_electron = -1;
				matchLep(*looper->Muons,Muons_mask,jet,match_muon,match_pt);
				matchLep(*looper->Electrons,Electrons_mask,jet,match_electron,match_pt);
				//an electron will only match if it is higher pt than any matched muon
				if(match_electron>=0) match_muon = -1;

				//modify fractions
				fixFracs(match_muon,*looper->Muons,JetsAK8_muonEnergyFraction,rawEnergy,j);
				fixFracs(match_electron,*looper->Electrons,JetsAK8_electronEnergyFraction,rawEnergy,j);
			}				

			for(auto& branch : linkbranches){
				//set to fixed vars
				branch->Vary();
			}
		}
		virtual void UndoVariation(){
			//restore original values
			for(auto& branch : linkbranches){
				branch->Restore();
			}
		}
		//helpers
		void matchLep(const vector<TLorentzVector>& leptons, const vector<bool>& mask, const TLorentzVector& jet, int& match_index, double& match_pt){
			for(unsigned l = 0; l < leptons.size(); ++l){
				if(!mask[l]) continue;
				const auto& lepton = leptons[l];
				if(lepton.Pt() > match_pt and lepton.DeltaR(jet)<0.8){
					match_pt = lepton.Pt();
					match_index = l;
				}
			}			
		}
		void fixFracs(int match_index, const vector<TLorentzVector>& leptons, vector<double>* leptonFraction, double rawEnergy, unsigned jet_index){
			if(match_index<0) return;

			const auto& lepton = leptons[match_index];
			//calculate frac w/ appropriate bound
			double matched_frac = min(lepton.E()/rawEnergy,leptonFraction->at(jet_index));
			//subtract from lepton frac
			leptonFraction->at(jet_index) -= matched_frac;
			//add half to chHad and half to photon
			JetsAK8_chargedHadronEnergyFraction->at(jet_index) += 0.5*matched_frac;
			JetsAK8_photonEnergyFraction->at(jet_index) += 0.5*matched_frac;
		}
		void clear(){
			JetsAK8_chargedHadronEnergyFraction->clear();
			JetsAK8_photonEnergyFraction->clear();
			JetsAK8_muonEnergyFraction->clear();
			JetsAK8_electronEnergyFraction->clear();
		}

		//member variables
		bool loose, has_jer;
		vector<double> *JetsAK8_chargedHadronEnergyFraction, *JetsAK8_photonEnergyFraction, *JetsAK8_muonEnergyFraction, *JetsAK8_electronEnergyFraction;
};
REGISTER_VARIATOR(LepFrac);

class KGenMHTVariator : public KVariator {
	public:
		//constructor
		KGenMHTVariator() : KVariator() { }
		KGenMHTVariator(string name_, OptionMap* localOpt_) : KVariator(name_,localOpt_) { }
		virtual void ListBranches(){
			linkbranches = {
				new KLinkedBranchD(KBranchD(&looper->MHT,"MHT"),KBranchD(&looper->GenMHT,"GenMHT")),
				new KLinkedBranchD(KBranchD(&looper->HT,"HT"),KBranchD(&looper->GenHT,"GenHT"))
			};
		}
		virtual void DoVariation() {
			for(auto& branch : linkbranches){
				//store original values
				branch->Store();
				//set to gen vars
				branch->Vary();
			}
		}
		virtual void UndoVariation(){
			//restore original values
			for(auto& branch : linkbranches){
				branch->Restore();
			}
		}
};
REGISTER_VARIATOR(GenMHT);

class KGenJetVariator : public KVariator {
	public:
		//constructor
		KGenJetVariator() : KVariator() { }
		KGenJetVariator(string name_, OptionMap* localOpt_) : KVariator(name_,localOpt_),
			placeholderB(true), placeholderN(0), placeholderF(1), JetsAK8_ID(new vector<bool>)
		{
			clear();
		}
		~KGenJetVariator() {
			delete JetsAK8_ID;
		}
		virtual void ListBranches(){
			//set up linked branches for all variations
			linkbranches = {
				//scalars get recalculated and replaced
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi1_AK8,"DeltaPhi1_AK8"),KBranchD(&DeltaPhi1_AK8)),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi2_AK8,"DeltaPhi2_AK8"),KBranchD(&DeltaPhi2_AK8)),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhiMin_AK8,"DeltaPhiMin_AK8"),KBranchD(&DeltaPhiMin_AK8)),
				new KLinkedBranchD(KBranchD(&looper->MJJ_AK8,"MJJ_AK8"),KBranchD(&MJJ_AK8)),
				new KLinkedBranchD(KBranchD(&looper->MT_AK8,"MT_AK8"),KBranchD(&MT_AK8)),
				new KLinkedBranchD(KBranchD(&looper->Mmc_AK8,"Mmc_AK8"),KBranchD(&Mmc_AK8)),
				new KLinkedBranchVB(KBranchVB(&looper->JetsAK8_ID,"JetsAK8_ID"),KBranchVB(&JetsAK8_ID)),
				//filters replaced with default placeholder values
				new KLinkedBranchB(KBranchB(&looper->JetIDAK8,"JetIDAK8"),KBranchB(&placeholderB)),
				new KLinkedBranchB(KBranchB(&looper->BadChargedCandidateFilter,"BadChargedCandidateFilter"),KBranchB(&placeholderB)),
				new KLinkedBranchB(KBranchB(&looper->BadPFMuonFilter,"BadPFMuonFilter"),KBranchB(&placeholderB)),
				new KLinkedBranchI(KBranchI(&looper->CSCTightHaloFilter,"CSCTightHaloFilter"),KBranchI(&placeholderF)),
				new KLinkedBranchI(KBranchI(&looper->ecalBadCalibFilter,"ecalBadCalibFilter"),KBranchI(&placeholderF)),
				new KLinkedBranchI(KBranchI(&looper->EcalDeadCellTriggerPrimitiveFilter,"EcalDeadCellTriggerPrimitiveFilter"),KBranchI(&placeholderF)),
				new KLinkedBranchI(KBranchI(&looper->eeBadScFilter,"eeBadScFilter"),KBranchI(&placeholderF)),
				new KLinkedBranchI(KBranchI(&looper->globalTightHalo2016Filter,"globalTightHalo2016Filter"),KBranchI(&placeholderF)),
				new KLinkedBranchI(KBranchI(&looper->HBHEIsoNoiseFilter,"HBHEIsoNoiseFilter"),KBranchI(&placeholderF)),
				new KLinkedBranchI(KBranchI(&looper->HBHENoiseFilter,"HBHENoiseFilter"),KBranchI(&placeholderF)),
				new KLinkedBranchI(KBranchI(&looper->PrimaryVertexFilter,"PrimaryVertexFilter"),KBranchI(&placeholderF)),
				new KLinkedBranchI(KBranchI(&looper->NVtx,"NVtx"),KBranchI(&placeholderF)),
				new KLinkedBranchI(KBranchI(&looper->NElectrons,"NElectrons"),KBranchI(&placeholderN)),
				new KLinkedBranchI(KBranchI(&looper->NMuons,"NMuons"),KBranchI(&placeholderN)),
				//"top-level" quantities get replaced
				new KLinkedBranchVL(KBranchVL(&looper->JetsAK8,"JetsAK8"),KBranchVL(&looper->GenJetsAK8)),
				new KLinkedBranchD(KBranchD(&looper->MET,"MET"),KBranchD(&looper->GenMET)),
				new KLinkedBranchD(KBranchD(&looper->METPhi,"METPhi"),KBranchD(&looper->GenMETPhi))
			};
		}
		virtual void DoVariation(){
			for(auto& branch : linkbranches){
				//store original values
				branch->Store();
			}
			
			//clear temp branches
			clear();

			//recalculate scalars from gen
			TLorentzVector vjj;
			const auto& JetsAK8 = *looper->GenJetsAK8;
			double MET = looper->GenMET;
			double METPhi = looper->GenMETPhi;
			int counter = 0;
			for(unsigned j = 0; j < JetsAK8.size(); ++j){
				if(counter<2){
					vjj += JetsAK8[j];
					if(DeltaPhi1_AK8>9) DeltaPhi1_AK8 = abs(KMath::DeltaPhi(JetsAK8[j].Phi(),METPhi));
					else if(DeltaPhi2_AK8>9) DeltaPhi2_AK8 = abs(KMath::DeltaPhi(JetsAK8[j].Phi(),METPhi));
					++counter;
				}
				JetsAK8_ID->push_back(true);
			}

			//check for 2 jets
			DeltaPhiMin_AK8 = min(DeltaPhi1_AK8,DeltaPhi2_AK8);
			if(JetsAK8.size()>=2){
				MJJ_AK8 = vjj.M();
				MT_AK8 = KMath::TransverseMass(vjj.Px(),vjj.Py(),vjj.M(),MET*cos(METPhi),MET*sin(METPhi),0);
				//skipping Mmc for now
			}

			for(auto& branch : linkbranches){
				//set to new vars
				branch->Vary();
			}
		}
		virtual void UndoVariation(){
			//restore original values
			for(auto& branch : linkbranches){
				branch->Restore();
			}
		}

		//helper
		void clear(){
			JetsAK8_ID->clear();
			DeltaPhi1_AK8 = 10;
			DeltaPhi2_AK8 = 10;
			DeltaPhiMin_AK8 = 10;
			MJJ_AK8 = 0;
			MT_AK8 = 0;
			Mmc_AK8 = 0;
		}

		//member variables
		bool placeholderB;
		int placeholderN, placeholderF;
		vector<bool>* JetsAK8_ID;
		double DeltaPhi1_AK8;
		double DeltaPhi2_AK8;
		double DeltaPhiMin_AK8;
		double MJJ_AK8;
		double Mmc_AK8;
		double MT_AK8;
};
REGISTER_VARIATOR(GenJet);

class KCentralAK8Variator : public KVariator {
	public:
		//constructor
		KCentralAK8Variator() : KVariator() { }
		KCentralAK8Variator(string name_, OptionMap* localOpt_) : KVariator(name_,localOpt_) { }
		virtual void ListBranches(){
			//set up linked branches for all variations
			linkbranches = {
				//scalars get recalculated and replaced
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi1_AK8,"DeltaPhi1_AK8"),KBranchD(&DeltaPhi1_AK8)),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi2_AK8,"DeltaPhi2_AK8"),KBranchD(&DeltaPhi2_AK8)),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhiMin_AK8,"DeltaPhiMin_AK8"),KBranchD(&DeltaPhiMin_AK8)),
				new KLinkedBranchD(KBranchD(&looper->MJJ_AK8,"MJJ_AK8"),KBranchD(&MJJ_AK8)),
				new KLinkedBranchD(KBranchD(&looper->MT_AK8,"MT_AK8"),KBranchD(&MT_AK8)),
				new KLinkedBranchD(KBranchD(&looper->Mmc_AK8,"Mmc_AK8"),KBranchD(&Mmc_AK8)),
				new KLinkedBranchB(KBranchB(&looper->JetIDAK8,"JetIDAK8"),KBranchB(&JetIDAK8)),
				//vectors get reordered (unwanted entries removed)
				new KReorderedBranchVL(KBranchVL(&looper->JetsAK8,"JetsAK8"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_axismajor,"JetsAK8_axismajor"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_axisminor,"JetsAK8_axisminor"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepMassDecorrelTagbbvsLight,"JetsAK8_DeepMassDecorrelTagbbvsLight"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepMassDecorrelTagHbbvsQCD,"JetsAK8_DeepMassDecorrelTagHbbvsQCD"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepMassDecorrelTagTvsQCD,"JetsAK8_DeepMassDecorrelTagTvsQCD"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepMassDecorrelTagWvsQCD,"JetsAK8_DeepMassDecorrelTagWvsQCD"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepMassDecorrelTagZbbvsQCD,"JetsAK8_DeepMassDecorrelTagZbbvsQCD"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepMassDecorrelTagZHbbvsQCD,"JetsAK8_DeepMassDecorrelTagZHbbvsQCD"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepMassDecorrelTagZvsQCD,"JetsAK8_DeepMassDecorrelTagZvsQCD"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepTagHbbvsQCD,"JetsAK8_DeepTagHbbvsQCD"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepTagTvsQCD,"JetsAK8_DeepTagTvsQCD"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepTagWvsQCD,"JetsAK8_DeepTagWvsQCD"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepTagZbbvsQCD,"JetsAK8_DeepTagZbbvsQCD"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_DeepTagZvsQCD,"JetsAK8_DeepTagZvsQCD"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_doubleBDiscriminator,"JetsAK8_doubleBDiscriminator"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_ecfN2b1,"JetsAK8_ecfN2b1"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_ecfN2b2,"JetsAK8_ecfN2b2"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_ecfN3b1,"JetsAK8_ecfN3b1"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_ecfN3b2,"JetsAK8_ecfN3b2"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_girth,"JetsAK8_girth"),order),
				new KReorderedBranchVB(KBranchVB(&looper->JetsAK8_ID,"JetsAK8_ID"),order),
				new KReorderedBranchVI(KBranchVI(&looper->JetsAK8_multiplicity,"JetsAK8_multiplicity"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_NsubjettinessTau1,"JetsAK8_NsubjettinessTau1"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_NsubjettinessTau2,"JetsAK8_NsubjettinessTau2"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_NsubjettinessTau3,"JetsAK8_NsubjettinessTau3"),order),
				new KReorderedBranchVI(KBranchVI(&looper->JetsAK8_NumBhadrons,"JetsAK8_NumBhadrons"),order),
				new KReorderedBranchVI(KBranchVI(&looper->JetsAK8_NumChadrons,"JetsAK8_NumChadrons"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb,"JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_ptD,"JetsAK8_ptD"),order),
				new KReorderedBranchVD(KBranchVD(&looper->JetsAK8_softDropMass,"JetsAK8_softDropMass"),order),
				new KReorderedBranchVVL(KBranchVVL(&looper->JetsAK8_subjets,"JetsAK8_subjets"),order),
				new KReorderedBranchVVD(KBranchVVD(&looper->JetsAK8_subjets_axismajor,"JetsAK8_subjets_axismajor"),order),
				new KReorderedBranchVVD(KBranchVVD(&looper->JetsAK8_subjets_axisminor,"JetsAK8_subjets_axisminor"),order),
				new KReorderedBranchVVD(KBranchVVD(&looper->JetsAK8_subjets_bDiscriminatorCSV,"JetsAK8_subjets_bDiscriminatorCSV"),order),
				new KReorderedBranchVVI(KBranchVVI(&looper->JetsAK8_subjets_multiplicity,"JetsAK8_subjets_multiplicity"),order),
				new KReorderedBranchVVD(KBranchVVD(&looper->JetsAK8_subjets_ptD,"JetsAK8_subjets_ptD"),order),
			};
		}

		virtual void DoVariation(){
			for(auto& branch : linkbranches){
				//store original values
				branch->Store();
			}
			
			//clear temp branches
			clear();

			//remove high-eta jets and recalculate scalars
			TLorentzVector vjj;
			const auto& JetsAK8 = *looper->JetsAK8;
			int counter = 0;
			for(unsigned j = 0; j < JetsAK8.size(); ++j){
				if(abs(JetsAK8[j].Eta())<2.4){
					order.push_back(j);
					if(counter<2) {
						vjj += JetsAK8[j];
						++counter;
					}
					JetIDAK8 &= looper->JetsAK8_ID->at(j);
					if(DeltaPhi1_AK8>9) DeltaPhi1_AK8 = abs(KMath::DeltaPhi(JetsAK8[j].Phi(),looper->METPhi));
					else if(DeltaPhi2_AK8>9) DeltaPhi2_AK8 = abs(KMath::DeltaPhi(JetsAK8[j].Phi(),looper->METPhi));
				}
			}

			//check for 2 jets
			DeltaPhiMin_AK8 = min(DeltaPhi1_AK8,DeltaPhi2_AK8);
			if(order.size()>=2){
				MJJ_AK8 = vjj.M();
				MT_AK8 = KMath::TransverseMass(vjj.Px(),vjj.Py(),vjj.M(),looper->MET*cos(looper->METPhi),looper->MET*sin(looper->METPhi),0);
				//skipping Mmc for now
			}

			for(auto& branch : linkbranches){
				//set to new vars
				branch->Vary();
			}
		}
		virtual void UndoVariation(){
			//restore original values
			for(auto& branch : linkbranches){
				branch->Restore();
			}
		}

		//helper
		void clear(){
			order.clear();
			DeltaPhi1_AK8 = 10;
			DeltaPhi2_AK8 = 10;
			DeltaPhiMin_AK8 = 10;
			JetIDAK8 = true;
			MJJ_AK8 = 0;
			MT_AK8 = 0;
			Mmc_AK8 = 0;
		}

		//member variables
		vector<unsigned> order;
		double DeltaPhi1_AK8;
		double DeltaPhi2_AK8;
		double DeltaPhiMin_AK8;
		bool JetIDAK8;
		double MJJ_AK8;
		double Mmc_AK8;
		double MT_AK8;

};
REGISTER_VARIATOR(CentralAK8);

class KJetLeptonVariator : public KVariator {
	public:
		//constructor
		KJetLeptonVariator() : KVariator(),
			Jets(new vector<TLorentzVector>),
			Jets_HTMask(new vector<bool>),
			Jets_MHTMask(new vector<bool>),
			Jets_ID(new vector<bool>),
			Jets_bDiscriminatorCSV(new vector<double>),
			Jets_muonEnergyFraction(new vector<double>)
		{
			clear();
		}
		KJetLeptonVariator(string name_, OptionMap* localOpt_) : KVariator(name_,localOpt_),
			Jets(new vector<TLorentzVector>),
			Jets_HTMask(new vector<bool>),
			Jets_MHTMask(new vector<bool>),
			Jets_ID(new vector<bool>),
			Jets_bDiscriminatorCSV(new vector<double>),
			Jets_muonEnergyFraction(new vector<double>)
		{
			clear();
		}
		~KJetLeptonVariator() {
			delete Jets;
			delete Jets_HTMask;
			delete Jets_MHTMask;
			delete Jets_ID;
			delete Jets_bDiscriminatorCSV;
			delete Jets_muonEnergyFraction;
		}
		virtual void CheckBranches(){
			ListBranches();
			//only check branch0 since branch1 does not come from tree
			for(auto& branch : linkbranches){
				branch->Check(looper->fChain,0);
			}
		}
		virtual void ListBranches(){
			linkbranches = {
				new KLinkedBranchVL(KBranchVL(&looper->Jets,"Jets"),KBranchVL(&Jets)),
				new KLinkedBranchVB(KBranchVB(&looper->Jets_HTMask,"Jets_HTMask"),KBranchVB(&Jets_HTMask)),
				new KLinkedBranchVB(KBranchVB(&looper->Jets_MHTMask,"Jets_MHTMask"),KBranchVB(&Jets_MHTMask)),
				new KLinkedBranchVB(KBranchVB(&looper->Jets_ID,"Jets_ID"),KBranchVB(&Jets_ID)),
				new KLinkedBranchVD(KBranchVD(&looper->Jets_bDiscriminatorCSV,"Jets_bDiscriminatorCSV"),KBranchVD(&Jets_bDiscriminatorCSV)),
				new KLinkedBranchVD(KBranchVD(&looper->Jets_muonEnergyFraction,"Jets_muonEnergyFraction"),KBranchVD(&Jets_muonEnergyFraction)),
				new KLinkedBranchB(KBranchB(&looper->JetID,"JetID"),KBranchB(&JetID)),
				new KLinkedBranchI(KBranchI(&looper->NJets,"NJets"),KBranchI(&NJets)),
				new KLinkedBranchI(KBranchI(&looper->BTags,"BTags"),KBranchI(&BTags)),
				new KLinkedBranchD(KBranchD(&looper->MHT,"MHT"),KBranchD(&MHT)),
				new KLinkedBranchD(KBranchD(&looper->MHTPhi,"MHTPhi"),KBranchD(&MHTPhi)),
				new KLinkedBranchD(KBranchD(&looper->HT,"HT"),KBranchD(&HT)),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi1,"DeltaPhi1"),KBranchD(&DeltaPhi1)),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi2,"DeltaPhi2"),KBranchD(&DeltaPhi2)),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi3,"DeltaPhi3"),KBranchD(&DeltaPhi3)),
				new KLinkedBranchD(KBranchD(&looper->DeltaPhi4,"DeltaPhi4"),KBranchD(&DeltaPhi4))
			};
		}
		virtual void DoVariation() {
			for(auto& branch : linkbranches){
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
					Jets->push_back(looper->Jets->at(j));
					Jets_HTMask->push_back(looper->Jets_HTMask->at(j));
					Jets_MHTMask->push_back(looper->Jets_MHTMask->at(j));
					Jets_ID->push_back(looper->Jets_ID->at(j));
					Jets_muonEnergyFraction->push_back(looper->Jets_muonEnergyFraction->at(j));
					Jets_bDiscriminatorCSV->push_back(looper->Jets_bDiscriminatorCSV->at(j));
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
			
			for(unsigned j = 0; j < min((unsigned)4,(unsigned)Jets->size()); ++j){
				//recalc delta phi
				if(j==0) DeltaPhi1 = abs(KMath::DeltaPhi(Jets->at(j).Phi(),MHTPhi));
				else if(j==1) DeltaPhi2 = abs(KMath::DeltaPhi(Jets->at(j).Phi(),MHTPhi));
				else if(j==2) DeltaPhi3 = abs(KMath::DeltaPhi(Jets->at(j).Phi(),MHTPhi));
				else if(j==3) DeltaPhi4 = abs(KMath::DeltaPhi(Jets->at(j).Phi(),MHTPhi));
			}
			
			for(auto& branch : linkbranches){
				//set to new vars
				branch->Vary();
			}
		}
		virtual void UndoVariation(){
			//restore original values
			for(auto& branch : linkbranches){
				branch->Restore();
			}
		}
		
		//helper
		void clear(){
			Jets->clear();
			Jets_HTMask->clear();
			Jets_MHTMask->clear();
			Jets_ID->clear();
			Jets_muonEnergyFraction->clear();
			Jets_bDiscriminatorCSV->clear();
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
		vector<TLorentzVector>* Jets;
		vector<bool>* Jets_HTMask;
		vector<bool>* Jets_MHTMask;
		vector<bool>* Jets_ID;
		vector<double>* Jets_bDiscriminatorCSV;
		vector<double>* Jets_muonEnergyFraction;
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

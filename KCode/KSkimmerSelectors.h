#ifndef KSKIMMERSELECTORS_H
#define KSKIMMERSELECTORS_H

//custom headers
#include "KSelection.h"
#include "KSkimmer.h"
#include "KMath.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>

//STL headers
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

//base class for Selectors is in KSelection.h

//----------------------------------------------------
//selects events based on number of jets
class KNJetSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KNJetSelector() : KSelector<KSkimmer>() { }
		KNJetSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), njet(4) { 
			//check for option
			localOpt->Get("njet",njet);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->NJets >= njet;
		}
		
		//member variables
		int njet;
};

//----------------------------------------------------
//selects events based on HT value
class KHTSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KHTSelector() : KSelector<KSkimmer>() { }
		KHTSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), HTmin(500) { 
			//check for option
			localOpt->Get("HTmin",HTmin);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->HT > HTmin;
		}
		
		//member variables
		double HTmin;
};

//----------------------------------------------------
//selects events based on MHT value
class KMHTSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KMHTSelector() : KSelector<KSkimmer>() { }
		KMHTSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), MHTmin(200) { 
			//check for option
			localOpt->Get("MHTmin",MHTmin);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->MHT > MHTmin;
		}
		
		//member variables
		double MHTmin;
};

//-------------------------------------------------------------
//vetos events with muons
class KMuonVetoSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KMuonVetoSelector() : KSelector<KSkimmer>() { }
		KMuonVetoSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->Muons->size()==0;
		}
		
		//member variables
};

//-------------------------------------------------------------
//vetos events with electrons
class KElectronVetoSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KElectronVetoSelector() : KSelector<KSkimmer>() { }
		KElectronVetoSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->Electrons->size()==0;
		}
		
		//member variables
};

//------------------------------------------------------
//single muon selector
class KMuonSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KMuonSelector() : KSelector<KSkimmer>() { }
		KMuonSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(looper->Muons->size()!=1) return false;
			double mT = sqrt(2*looper->METPt*looper->Muons->at(0).Pt()*(1-cos(KMath::DeltaPhi(looper->Muons->at(0).Phi(),looper->METPhi))));
			return mT<100;
		}
		
		//member variables
};

//------------------------------------------------------
//single electron selector
class KElectronSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KElectronSelector() : KSelector<KSkimmer>() { }
		KElectronSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(looper->Electrons->size()!=1) return false;
			double mT = sqrt(2*looper->METPt*looper->Electrons->at(0).Pt()*(1-cos(KMath::DeltaPhi(looper->Electrons->at(0).Phi(),looper->METPhi))));
			return mT<100;
		}
		
		//member variables
};

//------------------------------------------------------
//single photon selector
class KPhotonSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KPhotonSelector() : KSelector<KSkimmer>() { }
		KPhotonSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->NumPhotons==1;
		}
		
		//member variables
};

//------------------------------------------------------
//dimuon selector
class KDiMuonSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KDiMuonSelector() : KSelector<KSkimmer>() { }
		KDiMuonSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//todo: add mass cut?
			return looper->Muons->size() == 2 && looper->MuonCharge->at(0) != looper->MuonCharge->at(1);
		}
		
		//member variables
};

//------------------------------------------------------
//dielectron selector
class KDiElectronSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KDiElectronSelector() : KSelector<KSkimmer>() { }
		KDiElectronSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//todo: add mass cut?
			return looper->Electrons->size() == 2 && looper->ElectronCharge->at(0) != looper->ElectronCharge->at(1);
		}
		
		//member variables
};

//----------------------------------------------------
//selects events based on minDeltaPhiN value
class KMinDeltaPhiNSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KMinDeltaPhiNSelector() : KSelector<KSkimmer>() { }
		KMinDeltaPhiNSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), minDeltaPhiN(6.), invert(false) { 
			//check for option
			localOpt->Get("minDeltaPhiN",minDeltaPhiN);
			invert = localOpt->Get("invert",false);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(invert) return looper->minDeltaPhiN < minDeltaPhiN;
			else return looper->minDeltaPhiN > minDeltaPhiN;
		}
		
		//member variables
		double minDeltaPhiN;
		bool invert;
};

//-------------------------------------------------------------
//vetos events with isolated electron tracks
class KIsoElectronTrackVetoSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KIsoElectronTrackVetoSelector() : KSelector<KSkimmer>() { }
		KIsoElectronTrackVetoSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->isoElectronTracks==0;
		}
		
		//member variables
};

//-------------------------------------------------------------
//vetos events with isolated muon tracks
class KIsoMuonTrackVetoSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KIsoMuonTrackVetoSelector() : KSelector<KSkimmer>() { }
		KIsoMuonTrackVetoSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->isoMuonTracks==0;
		}
		
		//member variables
};

//-------------------------------------------------------------
//vetos events with isolated charged hadron tracks
class KIsoPionTrackVetoSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KIsoPionTrackVetoSelector() : KSelector<KSkimmer>() { }
		KIsoPionTrackVetoSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->isoPionTracks==0;
		}
		
		//member variables
};

//-------------------------------------------------------------
//vetos events with bad jets (including PFJetID & PBNR)
class KEventCleaningSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KEventCleaningSelector() : KSelector<KSkimmer>() { }
		KEventCleaningSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->JetID==1;
		}
		
		//member variables
};

//----------------------------------------------------
//selects events based on number of b-jets
class KNBJetSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KNBJetSelector() : KSelector<KSkimmer>() { }
		KNBJetSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), nbjet_min(-1), nbjet_max(-1) { 
			//check for option
			localOpt->Get("nbjet_min",nbjet_min);
			localOpt->Get("nbjet_max",nbjet_max);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return ( (nbjet_min==-1 || looper->BTags >= nbjet_min) && (nbjet_max==-1 || looper->BTags <= nbjet_max) );
		}
		
		//member variables
		int nbjet_min, nbjet_max;
};

//----------------------------------------------------
//chains together several inclusive b-tag bins
class KNBJetBinSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KNBJetBinSelector() : KSelector<KSkimmer>() { }
		KNBJetBinSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), initialized(false) { 
			//check for option
			localOpt->Get("nbjet_min",nbjet_min);
			localOpt->Get("nbjet_max",nbjet_max);
		}
		void Initialize(){
			if(initialized) return;
			
			//make individual bin selectors
			//after member ptrs get added to this
			for(unsigned b = 0; b < nbjet_min.size(); b++){
				OptionMap* omap = new OptionMap();
				omap->Set<int>("nbjet_min",nbjet_min[b]);
				omap->Set<int>("nbjet_max",nbjet_max[b]);
				
				stringstream sb;
				sb << "NBJet";
				if(nbjet_min[b]==nbjet_max[b]) sb << nbjet_min[b];
				else if(nbjet_min[b]==-1&&nbjet_max[b]>-1) sb << "<=" << nbjet_max[b];
				else if(nbjet_max[b]==-1&&nbjet_min[b]>-1) sb << ">=" << nbjet_min[b];
				else if(nbjet_max[b]>-1&&nbjet_min[b]>-1) sb << nbjet_min[b] << "-" << nbjet_max[b];
				else {}
				
				KSelector<KSkimmer>* srtmp = new KNBJetSelector(sb.str(),omap);
				srtmp->SetSelection(sel);
				srtmp->SetLooper(looper);
				bjet_sel.push_back(srtmp);
			}
			
			initialized = true;
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			Initialize();
			//pass any event that falls into any available bin
			bool goodB = false;
			for(unsigned b = 0; b < bjet_sel.size(); b++){
				goodB |= bjet_sel[b]->Select();
			}
			
			return goodB;
		}
		
		virtual void PrintEfficiency(vector<int>& widths, int prev_counter, int nentries){
			for(unsigned b = 0; b < bjet_sel.size(); b++){
				bjet_sel[b]->PrintEfficiency(widths,prev_counter,nentries);
			}
		}
		
		//member variables
		bool initialized;
		vector<int> nbjet_min, nbjet_max;
		vector<KSelector<KSkimmer>*> bjet_sel;
};

//-------------------------------------------------------------
//addition to KParser to create selectors
namespace KParser {
	template <>
	KSelector<KSkimmer>* processSelector<KSkimmer>(KNamed* tmp){
		KSelector<KSkimmer>* srtmp = 0;
		string sname = tmp->first;
		OptionMap* omap = tmp->second;
		
		//check for all known selectors
		if(sname=="NJet") srtmp = new KNJetSelector(sname,omap);
		else if(sname=="HT") srtmp = new KHTSelector(sname,omap);
		else if(sname=="MHT") srtmp = new KMHTSelector(sname,omap);
		else if(sname=="MuonVeto") srtmp = new KMuonVetoSelector(sname,omap);
		else if(sname=="ElectronVeto") srtmp = new KElectronVetoSelector(sname,omap);
		else if(sname=="Muon") srtmp = new KMuonSelector(sname,omap);
		else if(sname=="Electron") srtmp = new KElectronSelector(sname,omap);
		else if(sname=="Photon") srtmp = new KPhotonSelector(sname,omap);
		else if(sname=="DiMuon") srtmp = new KDiMuonSelector(sname,omap);
		else if(sname=="DiElectron") srtmp = new KDiElectronSelector(sname,omap);
		else if(sname=="IsoElectronTrackVeto") srtmp = new KIsoElectronTrackVetoSelector(sname,omap);
		else if(sname=="IsoMuonTrackVeto") srtmp = new KIsoMuonTrackVetoSelector(sname,omap);
		else if(sname=="IsoPionTrackVeto") srtmp = new KIsoPionTrackVetoSelector(sname,omap);
		else if(sname=="MinDeltaPhiN") srtmp = new KMinDeltaPhiNSelector(sname,omap);
		else if(sname=="EventCleaning") srtmp = new KEventCleaningSelector(sname,omap);
		else if(sname=="NBJetBin") srtmp = new KNBJetBinSelector(sname,omap);
		else {} //skip unknown selectors
		
		if(!srtmp) cout << "Input error: unknown selector " << sname << ". This selector will be skipped." << endl;
		
		return srtmp;
	}
}

#endif
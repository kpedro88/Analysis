#ifndef KSELECTORS_H
#define KSELECTORS_H

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
class KNJetSelector : public KSelector {
	public:
		//constructor
		KNJetSelector() : KSelector() { }
		KNJetSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), njet(4) { 
			//check for option
			localOpt->Get("njet",njet);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return sk->NJets >= njet;
		}
		
		//member variables
		int njet;
};

//----------------------------------------------------
//selects events based on HT value
class KHTSelector : public KSelector {
	public:
		//constructor
		KHTSelector() : KSelector() { }
		KHTSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), HTmin(500) { 
			//check for option
			localOpt->Get("HTmin",HTmin);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return sk->HT > HTmin;
		}
		
		//member variables
		double HTmin;
};

//----------------------------------------------------
//selects events based on MHT value
class KMHTSelector : public KSelector {
	public:
		//constructor
		KMHTSelector() : KSelector() { }
		KMHTSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), MHTmin(200) { 
			//check for option
			localOpt->Get("MHTmin",MHTmin);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return sk->MHT > MHTmin;
		}
		
		//member variables
		double MHTmin;
};

//-------------------------------------------------------------
//vetos events with muons
class KMuonVetoSelector : public KSelector {
	public:
		//constructor
		KMuonVetoSelector() : KSelector() { }
		KMuonVetoSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return sk->MuonsNum==0;
		}
		
		//member variables
};

//-------------------------------------------------------------
//vetos events with electrons
class KElectronVetoSelector : public KSelector {
	public:
		//constructor
		KElectronVetoSelector() : KSelector() { }
		KElectronVetoSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return sk->ElectronsNum==0;
		}
		
		//member variables
};

//----------------------------------------------------
//selects events based on minDeltaPhiN value
class KMinDeltaPhiNSelector : public KSelector {
	public:
		//constructor
		KMinDeltaPhiNSelector() : KSelector() { }
		KMinDeltaPhiNSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), minDeltaPhiN(4.) { 
			//check for option
			localOpt->Get("minDeltaPhiN",minDeltaPhiN);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return sk->minDeltaPhiN > minDeltaPhiN;
		}
		
		//member variables
		double minDeltaPhiN;
};

//-------------------------------------------------------------
//vetos events with isolated tracks
class KIsoTrackVetoSelector : public KSelector {
	public:
		//constructor
		KIsoTrackVetoSelector() : KSelector() { }
		KIsoTrackVetoSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return sk->isoTracks==0;
		}
		
		//member variables
};

//----------------------------------------------------
//selects events based on number of b-jets
class KNBJetSelector : public KSelector {
	public:
		//constructor
		KNBJetSelector() : KSelector() { }
		KNBJetSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), nbjet_min(-1), nbjet_max(-1) { 
			//check for option
			localOpt->Get("nbjet_min",nbjet_min);
			localOpt->Get("nbjet_max",nbjet_max);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return ( (nbjet_min==-1 || sk->BTags >= nbjet_min) && (nbjet_max==-1 || sk->BTags <= nbjet_max) );
		}
		
		//member variables
		int nbjet_min, nbjet_max;
};

//----------------------------------------------------
//chains together several inclusive b-tag bins
class KNBJetBinSelector : public KSelector {
	public:
		//constructor
		KNBJetBinSelector() : KSelector() { }
		KNBJetBinSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), initialized(false) { 
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
				
				KSelector* srtmp = new KNBJetSelector(sb.str(),omap);
				srtmp->SetSelection(sel);
				srtmp->SetSkimmer(sk);
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
		
		virtual void PrintEfficiency(vector<int>& widths, string sname, int prev_counter, int nentries){
			for(unsigned b = 0; b < bjet_sel.size(); b++){
				bjet_sel[b]->PrintEfficiency(widths,sname,prev_counter,nentries);
			}
		}
		
		//member variables
		bool initialized;
		vector<int> nbjet_min, nbjet_max;
		vector<KSelector*> bjet_sel;
};

//-------------------------------------------------------------
//addition to KParser to create selectors
namespace KParser {
	KSelector* processSelector(KNamed* tmp){
		KSelector* srtmp = 0;
		string sname = tmp->first;
		OptionMap* omap = tmp->second;
		
		//check for all known selectors
		if(sname=="NJet") srtmp = new KNJetSelector(sname,omap);
		else if(sname=="HT") srtmp = new KHTSelector(sname,omap);
		else if(sname=="MHT") srtmp = new KMHTSelector(sname,omap);
		else if(sname=="MuonVeto") srtmp = new KMuonVetoSelector(sname,omap);
		else if(sname=="ElectronVeto") srtmp = new KElectronVetoSelector(sname,omap);
		else if(sname=="IsoTrackVeto") srtmp = new KIsoTrackVetoSelector(sname,omap);
		else if(sname=="MinDeltaPhiN") srtmp = new KMinDeltaPhiNSelector(sname,omap);
		else if(sname=="NBJetBin") srtmp = new KNBJetBinSelector(sname,omap);
		else {} //skip unknown selectors
		
		if(!srtmp) cout << "Input error: unknown selector " << sname << ". This selector will be skipped." << endl;
		
		return srtmp;
	}
}

#endif
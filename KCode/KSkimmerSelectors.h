#ifndef KSKIMMERSELECTORS_H
#define KSKIMMERSELECTORS_H

//custom headers
#include "KSelection.h"
#include "KSkimmer.h"
#include "KMath.h"
#include "KPlot.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>

//STL headers
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

//implementation of NtupleClass-member-dependent KSkimmer function
int KSkimmer::GetEventSign() {
	if(Weight<0) return -1;
	else return 1;
}

//base class for Selectors is in KSelection.h

//----------------------------------------------------
//selects events based on HLT line
class KHLTSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KHLTSelector() : KSelector<KSkimmer>() { }
		KHLTSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { 
			//get selected line from options
			localOpt->Get("HLTLines",HLTLines);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//skip if no line provided
			if(HLTLines.size()==0) return true;
			
			//initial loop over trigger names to find indices (position is consistent for all events)
			if(HLTIndices.empty()){
				for(unsigned h = 0; h < HLTLines.size(); h++){
					vector<string>::iterator lb = lower_bound(looper->TriggerNames->begin(),looper->TriggerNames->end(),HLTLines[h]);
					if(lb != looper->TriggerNames->end() && lb->find(HLTLines[h]) != std::string::npos){
						HLTIndices.push_back(distance(looper->TriggerNames->begin(),lb));
					}
				}
			}
			
			//loop over trigger names
			bool goodTrigger = false;
			for(unsigned h = 0; h < HLTIndices.size(); h++){
				unsigned index = HLTIndices[h];
				//check:
				//1) if the decision was true (the line fired)
				//2) if the line was not prescaled (currently ignored)
				if(looper->TriggerPass->at(index)==1) {
					goodTrigger = true;
					break;
				}
			}
			//skip event if finished searching and no HLT lines found
			return goodTrigger;
		}
		
		//member variables
		vector<string> HLTLines;
		vector<unsigned> HLTIndices;
};

//------------------------------------------------------
//selects events based on run number (for blinding data)
class KBlindSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KBlindSelector() : KSelector<KSkimmer>() { }
		KBlindSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), lastUnblindRun(257599) { 
			//check for option
			localOpt->Get("run",lastUnblindRun);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->RunNum <= lastUnblindRun;
		}
		
		//member variables
		int lastUnblindRun;
};

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
		KMHTSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), MHTmin(200), debug(false), minPtMHT(30.), maxEtaMHT(5.) { 
			//check for option
			localOpt->Get("MHTmin",MHTmin);
			debug = localOpt->Get("debug",false);
			localOpt->Get("minPtMHT",minPtMHT);
			localOpt->Get("maxEtaMHT",maxEtaMHT);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->MHT > MHTmin;
		}
		
		//member variables
		double MHTmin;
		bool debug;
		double minPtMHT, maxEtaMHT;
};

//----------------------------------------------------
//low MHT selection for single photon test
class KLowMHTSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KLowMHTSelector() : KSelector<KSkimmer>() { }
		KLowMHTSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), MHTmax(200) { 
			//check for option
			localOpt->Get("MHTmax",MHTmax);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->MHTclean < MHTmax;
		}
		
		//member variables
		double MHTmax;
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

//-------------------------------------------------------------
//vetos events with photons
class KPhotonVetoSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KPhotonVetoSelector() : KSelector<KSkimmer>() { }
		KPhotonVetoSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->NumPhotons==0;
		}
		
		//member variables
};

//------------------------------------------------------
//single muon selector
class KMuonSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KMuonSelector() : KSelector<KSkimmer>() { }
		KMuonSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) {
			//check for option
			doMTcut = localOpt->Get("doMTcut",true);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(looper->Muons->size()!=1) return false;
			
			if(doMTcut){
				double mT = KMath::TransverseMass(looper->METPt,looper->METPhi,looper->Muons->at(0).Pt(),looper->Muons->at(0).Phi());
				return mT<100;
			}
			else return true;
		}
		
		//member variables
		bool doMTcut;
};

//------------------------------------------------------
//single electron selector
class KElectronSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KElectronSelector() : KSelector<KSkimmer>() { }
		KElectronSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) {
			//check for option
			doMTcut = localOpt->Get("doMTcut",true);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(looper->Electrons->size()!=1) return false;
			
			if(doMTcut){
				double mT = KMath::TransverseMass(looper->METPt,looper->METPhi,looper->Electrons->at(0).Pt(),looper->Electrons->at(0).Phi());
				return mT<100;
			}
			else return true;
		}
		
		//member variables
		bool doMTcut;
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
//selects events based on minDeltaPhi value
class KMinDeltaPhiSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KMinDeltaPhiSelector() : KSelector<KSkimmer>() { }
		KMinDeltaPhiSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), minDeltaPhi(0.4), invert(false) { 
			//check for option
			localOpt->Get("minDeltaPhi",minDeltaPhi);
			invert = localOpt->Get("invert",false);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//min delta phi needs to be defined
			double looperMinDeltaPhi = min(looper->DeltaPhi1,min(looper->DeltaPhi2,looper->DeltaPhi3));
			if(invert) return looperMinDeltaPhi < minDeltaPhi;
			else return looperMinDeltaPhi > minDeltaPhi;
		}
		
		//member variables
		double minDeltaPhi;
		bool invert;
};

//----------------------------------------------------
//selects events based on DeltaPhi values
class KDeltaPhiSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KDeltaPhiSelector() : KSelector<KSkimmer>() { }
		KDeltaPhiSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), DeltaPhi(4,0), invert(false) { 
			//check for option
			DeltaPhi[0] = 0.5; DeltaPhi[1] = 0.5; DeltaPhi[2] = 0.3; DeltaPhi[3] = 0.3;
			localOpt->Get("DeltaPhi",DeltaPhi);
			invert = localOpt->Get("invert",false);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(invert) return looper->DeltaPhi1 < DeltaPhi[0] || looper->DeltaPhi2 < DeltaPhi[1] || looper->DeltaPhi3 < DeltaPhi[2] || looper->DeltaPhi4 < DeltaPhi[3];
			else return looper->DeltaPhi1 > DeltaPhi[0] && looper->DeltaPhi2 > DeltaPhi[1] && looper->DeltaPhi3 > DeltaPhi[2] && looper->DeltaPhi4 > DeltaPhi[3];
		}
		
		//member variables
		vector<double> DeltaPhi;
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
//vetos events with bad jets (using PFJetID loose WP)
class KEventCleaningSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KEventCleaningSelector() : KSelector<KSkimmer>() { }
		KEventCleaningSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->JetID;
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
		
		virtual void PrintEfficiency(vector<unsigned>& widths, int prev_counter, int nentries){
			for(unsigned b = 0; b < bjet_sel.size(); b++){
				bjet_sel[b]->PrintEfficiency(widths,prev_counter,nentries);
			}
		}
		
		//member variables
		bool initialized;
		vector<int> nbjet_min, nbjet_max;
		vector<KSelector<KSkimmer>*> bjet_sel;
};

//----------------------------------------------------
//calculate b-tagging & mistagging efficiencies
class KBTagEfficiencySelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KBTagEfficiencySelector() : KSelector<KSkimmer>() { }
		KBTagEfficiencySelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) {
			canfail = false;
			//initialize histograms using KPlot2D::CreateHist() method
			TH1::AddDirectory(kFALSE);
			KPlot2D* ptmp = new KPlot2D("n_eff_b","",localOpt,NULL);
			ptmp->CreateHist();
			n_eff_b = (TH2F*)ptmp->GetHisto();
			delete ptmp;
			//reset z-title
			n_eff_b->GetZaxis()->SetTitle("");
			//clone histo
			n_eff_c = (TH2F*)n_eff_b->Clone("n_eff_c");
			n_eff_udsg = (TH2F*)n_eff_b->Clone("n_eff_udsg");
			d_eff_b = (TH2F*)n_eff_b->Clone("d_eff_b");
			d_eff_c = (TH2F*)n_eff_b->Clone("d_eff_c");
			d_eff_udsg = (TH2F*)n_eff_b->Clone("d_eff_udsg");
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//loop over jets
			for(unsigned ja = 0; ja < looper->Jets->size(); ++ja){
				//HT jet cuts
				if(!looper->HTJetsMask->at(ja)) continue;
				
				//fill by flavor
				int flav = abs(looper->Jets_partonFlavor->at(ja));
				double csv = looper->Jets_bDiscriminatorCSV->at(ja);
				double pt = looper->Jets->at(ja).Pt();
				//use abs(eta) for now
				double eta = fabs(looper->Jets->at(ja).Eta());
				if(flav==5){
					d_eff_b->Fill(pt,eta);
					if(csv > 0.890) n_eff_b->Fill(pt,eta);
				}
				else if(flav==4){
					d_eff_c->Fill(pt,eta);
					if(csv > 0.890) n_eff_c->Fill(pt,eta);
				}
				else if(flav<4 || flav==21){
					d_eff_udsg->Fill(pt,eta);
					if(csv > 0.890) n_eff_udsg->Fill(pt,eta);
				}
			}
			
			return true;
		}
		
		virtual void Finalize(TFile* file){
			//calculate efficiency from numer and denom
			TH2F* h_eff_b = (TH2F*)n_eff_b->Clone("h_eff_b");
			TH2F* h_eff_c = (TH2F*)n_eff_c->Clone("h_eff_c");
			TH2F* h_eff_udsg = (TH2F*)n_eff_udsg->Clone("h_eff_udsg");
			h_eff_b->Divide(d_eff_b);
			h_eff_c->Divide(d_eff_c);
			h_eff_udsg->Divide(d_eff_udsg);
			
			//write to file
			file->cd();
			h_eff_b->Write();
			h_eff_c->Write();
			h_eff_udsg->Write();
		}
		
		//member variables
		TH2F *n_eff_b, *n_eff_c, *n_eff_udsg;
		TH2F *d_eff_b, *d_eff_c, *d_eff_udsg;
};

//-------------------------------------------------------------
//look at only a specific range of events
class KEventRangeSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KEventRangeSelector() : KSelector<KSkimmer>() { }
		KEventRangeSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), start(0), finish(0) { 
			//check for option
			localOpt->Get("start",start);
			localOpt->Get("finish",finish);
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(finish>=start && (looper->EvtNum < start || looper->EvtNum > finish)) return false;
			else return true;
		}
		
		//member variables
		unsigned start, finish;
};

//-------------------------------------------------------------
//special version of selector class for object sync
class KSyncSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KSyncSelector() : KSelector<KSkimmer>() { }
		KSyncSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), depname(""), prevSel(NULL), obj_counter(0), debug(false) { 
			//check for option
			localOpt->Get("dep",depname);
		}
		
		//override accessors
		virtual void CheckDeps(){
			debug = sel->GetGlobalOpt()->Get("debug",false);
			//set dependencies here
			if(depname.size()>0){
				prevSel = sel->Get<KSyncSelector*>(depname);
				if(!prevSel){
					cout << "Input error: dependency " << depname << " failed in " << name << "!" << endl;
					depfailed = true;
				}
			}
		}
		virtual bool Select(){
			if(depfailed) return false;
			bool result = dummy || Cut();
			if(result) counter++;
			return result;
		}
		virtual void PrintEfficiency(vector<unsigned>& widths, int prev_counter, int nentries){
			if(dummy || !canfail) return;
			cout << left << setw(widths[0]) << name;
			cout << "  " << right << setw(widths[1]) << counter;
			cout << "  " << right << setw(widths[2]) << ((double)counter/(double)nentries)*100;
			//no rel. eff. for first selector
			if(prev_counter>0) cout << "  " << right << setw(widths[3]) << ((double)counter/(double)prev_counter)*100;
			else cout << "  " << right << setw(widths[3]) << " ";
			//# of objects
			stringstream sobj;
			sobj << "(" << obj_counter << ")";
			cout << "  " << right << setw(widths[3]) << sobj.str();
			cout << endl;
		}
		
		//member variables
		vector<unsigned> goodObjects;
		string depname;
		KSyncSelector* prevSel;
		
	protected:
		//member variables
		int obj_counter;
		bool debug;
};

//-------------------------------------------------------------
//photon all selector for object sync
class KPhotonAllSelector : public KSyncSelector {
	public:
		//constructor
		KPhotonAllSelector() : KSyncSelector() { }
		KPhotonAllSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }

		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned p = 0; p < looper->photonCands->size(); ++p){
				goodObjects.push_back(p);
				++obj_counter;
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 photons" << endl; } return false; }
			else return true;
		}
		
		//member variables
};

//-------------------------------------------------------------
//photon eta selector for object sync
class KPhotonEtaSelector : public KSyncSelector {
	public:
		//constructor
		KPhotonEtaSelector() : KSyncSelector() { }
		KPhotonEtaSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }

		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned p = 0; p < looper->photonCands->size(); ++p){
				unsigned pp = prevSel->goodObjects[p];
				bool goodPhoton = abs(looper->photonCands->at(pp).Eta())<1.4442 || ((abs(looper->photonCands->at(pp).Eta())>1.566 && abs(looper->photonCands->at(pp).Eta())<2.5));
				if(goodPhoton) {
					goodObjects.push_back(pp);
					++obj_counter;
				}
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 photons" << endl; } return false; }
			else return true;
		}
		
		//member variables
};

//-------------------------------------------------------------
//photon pt selector for object sync
class KPhotonPtSelector : public KSyncSelector {
	public:
		//constructor
		KPhotonPtSelector() : KSyncSelector() { }
		KPhotonPtSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(debug) cout << "***** event " << looper->EvtNum << " *****" << endl;
			goodObjects.clear();
			for(unsigned p = 0; p < prevSel->goodObjects.size(); ++p){
				unsigned pp = prevSel->goodObjects[p];
				bool goodPhoton = looper->photonCands->at(pp).Pt() > 100;
				if(goodPhoton) {
					goodObjects.push_back(pp);
					++obj_counter;
					if(debug) {
						cout << "Photon " << pp << ": pt " << looper->photonCands->at(pp).Pt() << ", eta " << looper->photonCands->at(pp).Eta() << endl;
						cout << "sieie " << looper->photon_sigmaIetaIeta->at(pp) << ", hOverE " << looper->photon_hadTowOverEM->at(pp) << ", hasPixelSeed " << looper->photon_hasPixelSeed->at(pp) << endl;
						cout << "CH Iso " << looper->photon_pfChargedIsoRhoCorr->at(pp) << ", NH Iso " << looper->photon_pfNeutralIsoRhoCorr->at(pp) << ", PH Iso " << looper->photon_pfGammaIsoRhoCorr->at(pp) << endl;
					}
				}
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 photons" << endl; } return false; }
			else return true;
		}
};

//-------------------------------------------------------------
//photon ID selector for object sync
class KPhotonIDSelector : public KSyncSelector {
	public:
		//constructor
		KPhotonIDSelector() : KSyncSelector() { }
		KPhotonIDSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_), vetoType(0) { 
			//check for option
			localOpt->Get("vetoType",vetoType);
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned p = 0; p < prevSel->goodObjects.size(); ++p){
				unsigned pp = prevSel->goodObjects[p];
				//common cut
				bool goodPhoton = true;
				if(vetoType==1) goodPhoton = !looper->photon_hasPixelSeed->at(pp);
				else if(vetoType==2) goodPhoton = looper->photon_passElectronVeto->at(pp);
				if(looper->photon_isEB->at(pp)){ //barrel cuts
					goodPhoton &= looper->photon_hadTowOverEM->at(pp) < 0.028 && looper->photon_sigmaIetaIeta->at(pp) < 0.0107;
				}
				else { //endcap cuts
					goodPhoton &= looper->photon_hadTowOverEM->at(pp) < 0.093 && looper->photon_sigmaIetaIeta->at(pp) < 0.0272;
				}
				if(goodPhoton) {
					goodObjects.push_back(pp);
					++obj_counter;
					if(debug) {
						cout << "Photon " << pp << ": passed ID" << endl;
					}
				}
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 photons" << endl; } return false; }
			else return true;
		}
		
		//member variables
		int vetoType;
};

//-------------------------------------------------------------
//photon CH iso selector for object sync
class KPhotonCHIsoSelector : public KSyncSelector {
	public:
		//constructor
		KPhotonCHIsoSelector() : KSyncSelector() { }
		KPhotonCHIsoSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned p = 0; p < prevSel->goodObjects.size(); ++p){
				unsigned pp = prevSel->goodObjects[p];
				bool goodPhoton = false;
				if(looper->photon_isEB->at(pp)){ //barrel cuts
					goodPhoton = looper->photon_pfChargedIsoRhoCorr->at(pp) < 2.67;
				}
				else { //endcap cuts
					goodPhoton = looper->photon_pfChargedIsoRhoCorr->at(pp) < 1.79;
				}
				if(goodPhoton) {
					goodObjects.push_back(pp);
					++obj_counter;
				}
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 photons" << endl; } return false; }
			else return true;
		}
};

//-------------------------------------------------------------
//photon NH iso selector for object sync
class KPhotonNHIsoSelector : public KSyncSelector {
	public:
		//constructor
		KPhotonNHIsoSelector() : KSyncSelector() { }
		KPhotonNHIsoSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned p = 0; p < prevSel->goodObjects.size(); ++p){
				unsigned pp = prevSel->goodObjects[p];
				bool goodPhoton = false;
				if(looper->photon_isEB->at(pp)){ //barrel cuts
					goodPhoton = looper->photon_pfNeutralIsoRhoCorr->at(pp) < 7.23 + exp(0.0028*looper->photonCands->at(pp).Pt()+0.5408);
				}
				else { //endcap cuts
					goodPhoton = looper->photon_pfNeutralIsoRhoCorr->at(pp) < 8.89 + 0.01725*looper->photonCands->at(pp).Pt();
				}
				if(goodPhoton) {
					goodObjects.push_back(pp);
					++obj_counter;
				}
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 photons" << endl; } return false; }
			else return true;
		}
};

//-------------------------------------------------------------
//photon PH iso selector for object sync
class KPhotonPHIsoSelector : public KSyncSelector {
	public:
		//constructor
		KPhotonPHIsoSelector() : KSyncSelector() { }
		KPhotonPHIsoSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned p = 0; p < prevSel->goodObjects.size(); ++p){
				unsigned pp = prevSel->goodObjects[p];
				bool goodPhoton = false;
				if(looper->photon_isEB->at(pp)){ //barrel cuts
					goodPhoton = looper->photon_pfGammaIsoRhoCorr->at(pp) < 2.11 + 0.0014*looper->photonCands->at(pp).Pt();
				}
				else { //endcap cuts
					goodPhoton = looper->photon_pfGammaIsoRhoCorr->at(pp) < 3.09 + 0.0091*looper->photonCands->at(pp).Pt();
				}
				if(goodPhoton) {
					goodObjects.push_back(pp);
					++obj_counter;
					if(debug) {
						cout << "Photon " << pp << ": passed isolation" << endl;
					}
				}
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 photons" << endl; } return false; }
			else { if(debug){ cout << "found " << goodObjects.size() << " photons" << endl; } return true; }
		}
};

//-------------------------------------------------------------
//selects events with isolated electron tracks (for sync)
class KIsoElectronTrackSelector : public KSyncSelector {
	public:
		//constructor
		KIsoElectronTrackSelector() : KSyncSelector() { }
		KIsoElectronTrackSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			obj_counter += looper->isoElectronTracks;
			return looper->isoElectronTracks>0;
		}
		
		//member variables
};

//-------------------------------------------------------------
//selects events with isolated muon tracks (for sync)
class KIsoMuonTrackSelector : public KSyncSelector {
	public:
		//constructor
		KIsoMuonTrackSelector() : KSyncSelector() { }
		KIsoMuonTrackSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			obj_counter += looper->isoMuonTracks;
			return looper->isoMuonTracks>0;
		}
		
		//member variables
};

//-------------------------------------------------------------
//selects events with isolated charged hadron tracks (for sync)
class KIsoPionTrackSelector : public KSyncSelector {
	public:
		//constructor
		KIsoPionTrackSelector() : KSyncSelector() { }
		KIsoPionTrackSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			obj_counter += looper->isoPionTracks;
			return looper->isoPionTracks>0;
		}
		
		//member variables
};

/*
//isotrack sync selectors currently not used

//-------------------------------------------------------------
//isotrack pdgid selector for object sync
class KIsoTrackPDGSelector : public KSyncSelector {
	public:
		//constructor
		KIsoTrackPDGSelector() : KSyncSelector() { }
		KIsoTrackPDGSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_), pdg(0) { 
			//check for option
			localOpt->Get("pdg",pdg);
		}

		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned p = 0; p < looper->pfcands->size(); ++p){
				if(pdg!=0 && abs(looper->pfcands_id->at(p)) != pdg) continue;
				
				goodObjects.push_back(p);
				++obj_counter;
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 tracks" << endl; } return false; }
			else return true;
		}
		
		//member variables
		int pdg;
};

//-------------------------------------------------------------
//isotrack eta selector for object sync
class KIsoTrackEtaSelector : public KSyncSelector {
	public:
		//constructor
		KIsoTrackEtaSelector() : KSyncSelector() { }
		KIsoTrackEtaSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_), maxEta(2.5) { 
			//check for option
			localOpt->Get("maxEta",maxEta);
		}

		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned p = 0; p < prevSel->goodObjects.size(); ++p){
				unsigned pp = prevSel->goodObjects[p];
				if(fabs(looper->pfcands->at(pp).Eta()) > maxEta) continue;
				
				goodObjects.push_back(pp);
				++obj_counter;
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 tracks" << endl; } return false; }
			else return true;
		}
		
		//member variables
		double maxEta;
};

//-------------------------------------------------------------
//isotrack pt selector for object sync
class KIsoTrackPtSelector : public KSyncSelector {
	public:
		//constructor
		KIsoTrackPtSelector() : KSyncSelector() { }
		KIsoTrackPtSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_), minPt(5) { 
			//check for option
			localOpt->Get("minPt",minPt);
		}

		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			if(debug) cout << "***** event " << looper->EvtNum << " *****" << endl;
			for(unsigned p = 0; p < prevSel->goodObjects.size(); ++p){
				unsigned pp = prevSel->goodObjects[p];
				if(debug){ //apply separate pt cuts manually
					if( ( (abs(looper->pfcands_id->at(pp))==11 || abs(looper->pfcands_id->at(pp))==13) && looper->pfcands->at(pp).Pt() < 5.)
						|| (abs(looper->pfcands_id->at(pp))==211 && looper->pfcands->at(pp).Pt() < 10.) 
						|| looper->pfcands_chg->at(pp)==0 ) continue;
				}
				else{
					if(looper->pfcands->at(pp).Pt() < minPt) continue;
				}
				
				if(debug) {
					cout << fixed << "Itk " << pp << ": type=" << setprecision(0) << looper->pfcands_id->at(pp) << ", pt=" << setprecision(3) << looper->pfcands->at(pp).Pt() << ", dz=" << looper->pfcands_dzpv->at(pp) << endl;
					cout << "    " << "mT=" << looper->pfcands_mT->at(pp) << ", rel_iso=" << looper->pfcands_trkiso->at(pp) << endl;
				}
				
				goodObjects.push_back(pp);
				++obj_counter;
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 tracks" << endl; } return false; }
			else return true;
		}
		
		//member variables
		double minPt;
};

//-------------------------------------------------------------
//isotrack dz selector for object sync
class KIsoTrackDzSelector : public KSyncSelector {
	public:
		//constructor
		KIsoTrackDzSelector() : KSyncSelector() { }
		KIsoTrackDzSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_), dzCut(0.1) { 
			//check for option
			localOpt->Get("dzCut",dzCut);
		}

		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned p = 0; p < prevSel->goodObjects.size(); ++p){
				unsigned pp = prevSel->goodObjects[p];
				if(fabs(looper->pfcands_dzpv->at(pp)) > dzCut) continue;
				
				goodObjects.push_back(pp);
				++obj_counter;
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 tracks" << endl; } return false; }
			else return true;
		}
		
		//member variables
		double dzCut;
};

//-------------------------------------------------------------
//isotrack iso selector for object sync
class KIsoTrackIsoSelector : public KSyncSelector {
	public:
		//constructor
		KIsoTrackIsoSelector() : KSyncSelector() { }
		KIsoTrackIsoSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_), isoCut(0.2) { 
			//check for option
			localOpt->Get("isoCut",isoCut);
		}

		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned p = 0; p < prevSel->goodObjects.size(); ++p){
				unsigned pp = prevSel->goodObjects[p];
				if(looper->pfcands_trkiso->at(pp)/looper->pfcands->at(pp).Pt() > isoCut) continue;
				
				goodObjects.push_back(pp);
				++obj_counter;
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 tracks" << endl; } return false; }
			else return true;
		}
		
		//member variables
		double isoCut;
};

//-------------------------------------------------------------
//isotrack mT selector for object sync
class KIsoTrackMtSelector : public KSyncSelector {
	public:
		//constructor
		KIsoTrackMtSelector() : KSyncSelector() { }
		KIsoTrackMtSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_), mtCut(100) { 
			//check for option
			localOpt->Get("mtCut",mtCut);
		}

		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned p = 0; p < prevSel->goodObjects.size(); ++p){
				unsigned pp = prevSel->goodObjects[p];
				if(mtCut > 0.01 && looper->pfcands_mT->at(pp) > mtCut) continue;
				
				goodObjects.push_back(pp);
				++obj_counter;
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 tracks" << endl; } return false; }
			else return true;
		}
		
		//member variables
		double mtCut;
};

*/

//-------------------------------------------------------------
//addition to KParser to create selectors
namespace KParser {
	template <>
	KSelector<KSkimmer>* processSelector<KSkimmer>(KNamed* tmp){
		KSelector<KSkimmer>* srtmp = 0;
		string sname = tmp->first;
		OptionMap* omap = tmp->second;
		
		//check for all known selectors
		if(sname=="HLT") srtmp = new KHLTSelector(sname,omap);
		else if(sname=="Blind") srtmp = new KBlindSelector(sname,omap);
		else if(sname=="NJet") srtmp = new KNJetSelector(sname,omap);
		else if(sname=="HT") srtmp = new KHTSelector(sname,omap);
		else if(sname=="MHT") srtmp = new KMHTSelector(sname,omap);
		else if(sname=="LowMHT") srtmp = new KLowMHTSelector(sname,omap);
		else if(sname=="MuonVeto") srtmp = new KMuonVetoSelector(sname,omap);
		else if(sname=="ElectronVeto") srtmp = new KElectronVetoSelector(sname,omap);
		else if(sname=="PhotonVeto") srtmp = new KPhotonVetoSelector(sname,omap);
		else if(sname=="Muon") srtmp = new KMuonSelector(sname,omap);
		else if(sname=="Electron") srtmp = new KElectronSelector(sname,omap);
		else if(sname=="Photon") srtmp = new KPhotonSelector(sname,omap);
		else if(sname=="DiMuon") srtmp = new KDiMuonSelector(sname,omap);
		else if(sname=="DiElectron") srtmp = new KDiElectronSelector(sname,omap);
		else if(sname=="IsoElectronTrackVeto") srtmp = new KIsoElectronTrackVetoSelector(sname,omap);
		else if(sname=="IsoMuonTrackVeto") srtmp = new KIsoMuonTrackVetoSelector(sname,omap);
		else if(sname=="IsoPionTrackVeto") srtmp = new KIsoPionTrackVetoSelector(sname,omap);
		else if(sname=="MinDeltaPhi") srtmp = new KMinDeltaPhiSelector(sname,omap);
		else if(sname=="DeltaPhi") srtmp = new KDeltaPhiSelector(sname,omap);
		else if(sname=="EventCleaning") srtmp = new KEventCleaningSelector(sname,omap);
		else if(sname=="NBJetBin") srtmp = new KNBJetBinSelector(sname,omap);
		else if(sname=="BTagEfficiency") srtmp = new KBTagEfficiencySelector(sname,omap);
		else if(sname=="EventRange") srtmp = new KEventRangeSelector(sname,omap);
		else if(sname=="PhotonAll") srtmp = new KPhotonAllSelector(sname,omap);
		else if(sname=="PhotonEta") srtmp = new KPhotonEtaSelector(sname,omap);
		else if(sname=="PhotonPt") srtmp = new KPhotonPtSelector(sname,omap);
		else if(sname=="PhotonID") srtmp = new KPhotonIDSelector(sname,omap);
		else if(sname=="PhotonCHIso") srtmp = new KPhotonCHIsoSelector(sname,omap);
		else if(sname=="PhotonNHIso") srtmp = new KPhotonNHIsoSelector(sname,omap);
		else if(sname=="PhotonPHIso") srtmp = new KPhotonPHIsoSelector(sname,omap);
		else if(sname=="IsoElectronTrack") srtmp = new KIsoElectronTrackSelector(sname,omap);
		else if(sname=="IsoMuonTrack") srtmp = new KIsoMuonTrackSelector(sname,omap);
		else if(sname=="IsoPionTrack") srtmp = new KIsoPionTrackSelector(sname,omap);
		//else if(sname=="IsoTrackPDG") srtmp = new KIsoTrackPDGSelector(sname,omap);
		//else if(sname=="IsoTrackEta") srtmp = new KIsoTrackEtaSelector(sname,omap);
		//else if(sname=="IsoTrackPt") srtmp = new KIsoTrackPtSelector(sname,omap);
		//else if(sname=="IsoTrackDz") srtmp = new KIsoTrackDzSelector(sname,omap);
		//else if(sname=="IsoTrackIso") srtmp = new KIsoTrackIsoSelector(sname,omap);
		//else if(sname=="IsoTrackMt") srtmp = new KIsoTrackMtSelector(sname,omap);
		else {} //skip unknown selectors
		
		if(!srtmp) cout << "Input error: unknown selector " << sname << ". This selector will be skipped." << endl;
		
		return srtmp;
	}
}

#endif
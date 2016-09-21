#ifndef KSELECTION_H
#define KSELECTION_H

//custom headers
#include "KMap.h"
#include "KMath.h"
#include "KVariation.h"
#include "KCutflow.h"
#include "../corrections/EventListFilter.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>

//STL headers
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

//forward declaration
template <class T>
class KSelection;

//helper class for cutflow histos
class KCutflowEntry {
	public:
		//constructor
		KCutflowEntry() : name(""), raw(-1), rawE(-1), obj(-1), objE(-1) {}
		KCutflowEntry(string name_, int raw_, double rawE_, double obj_=-1, double objE_=-1)
			: name(name_), raw(raw_), rawE(rawE_), obj(obj_), objE(objE_) {}
	
		//member variables
		string name;
		int raw; double rawE;
		double obj, objE;
};

//----------------------------------------------------------------
//base class for Selectors, has standard functions defined
template <class T>
class KSelector {
	public:
		//constructor
		KSelector() : name(""), localOpt(0), sel(0), looper(0), tree(0), counter(0), dummy(0), canfail(1), depfailed(0) {
			//must always have local option map
			if(localOpt==0) localOpt = new OptionMap();
		}
		KSelector(string name_, OptionMap* localOpt_) : name(name_), localOpt(localOpt_), sel(0), looper(0), tree(0), counter(0), dummy(0), canfail(1), depfailed(0) {
			//must always have local option map
			if(localOpt==0) localOpt = new OptionMap();
			dummy = localOpt->Get("dummy",false);
		}
		//destructor
		virtual ~KSelector() {}
		//accessors
		string GetName() { return name; }
		virtual void SetSelection(KSelection<T>* sel_) { sel = sel_; CheckDeps(); } //set dependencies here if desired
		virtual void SetLooper(T* looper_) { looper = looper_; CheckLooper(); }
		virtual void SetTree(TTree* tree_) { tree = tree_; SetBranches(); } //set tree branches here if desired
		int GetCounter() { return counter; }
		bool Dummy() { return dummy; }
		bool CanFail() { return canfail; }
		bool FailedDependency() { return depfailed; }
		//selection function, checks dummy first
		virtual bool Select(){
			bool result = dummy || Cut();
			if(result) counter++;
			return result;
		}
		//to allow selectors to add objects to the file
		virtual void Finalize(TFile*) { }
		//used for non-dummy selectors
		virtual bool Cut() { return true; }
		virtual void GetEfficiency(vector<KCutflowEntry>& cuts){
			if(dummy || !canfail) return;
			cuts.push_back(KCutflowEntry(name, counter, KMath::PoissonErrorUp(counter)));
		}
		//to set tree branches
		virtual void SetBranches() {}
		//to check dependencies on other selectors
		virtual void CheckDeps() {}
		//to check dependencies on the looper
		virtual void CheckLooper() {}
		//to allow selectors to turn on branches they need
		virtual void CheckBranches() {}
		
	protected:
		//member variables
		string name;
		OptionMap* localOpt;
		KSelection<T>* sel;
		T* looper;
		TTree* tree;
		int counter;
		bool dummy, canfail, depfailed;
};

//----------------------------------------------------------------
//class to keep track of a list of Selectors
//has a vector for an ordered list and a map for a searchable list
template <class T>
class KSelection {
	public:
		//constructor
		KSelection() : name(""), variation(0), looper(0), file(0), tree(0), cutflowHist(0) {}
		KSelection(string name_, OptionMap* globalOpt_) : name(name_), globalOpt(globalOpt_), variation(0), looper(0), file(0), tree(0), cutflowHist(0), filter(0), eventlist("") {
			//must always have option map
			if(globalOpt==0) globalOpt = new OptionMap();
			globalOpt->Get("selectevents",eventlist);
			if(eventlist.size()>0) filter = new EventListFilter(eventlist);
		}
		//destructor
		virtual ~KSelection() {
			delete file;
		}
		//accessors
		string GetName() { return name; }
		OptionMap* GetGlobalOpt() { return globalOpt; }
		void SetVariation(KVariation<T>* varn) { variation = varn; }
		void AddSelector(KSelector<T>* sel_){
			selectorList.push_back(sel_);
			selectors.Add(sel_->GetName(),sel_);
			sel_->SetSelection(this);
		}
		void SetLooper(T* looper_){
			looper = looper_;
			for(unsigned s = 0; s < selectorList.size(); s++){
				selectorList[s]->SetLooper(looper_);
			}
			if(variation) variation->SetLooper(looper_);
			if(filter){
				looper->fChain->SetBranchStatus("RunNum",1);
				looper->fChain->SetBranchStatus("LumiBlockNum",1);
				looper->fChain->SetBranchStatus("EvtNum",1);
			}
		}
		void DoVariation() { if(variation) variation->DoVariation(); }
		void UndoVariation() { if(variation) variation->UndoVariation(); }
		template <class S> S Get(string name_){	return static_cast<S>(selectors.Get(name_)); }
		//setup output tree
		void MakeTree(string outdir, string filename, TTree* clone=NULL){
			//make sure the folder exists
			string treefolder = outdir + "_" + name;
			string syscmd = "mkdir -p " + treefolder;
			system(syscmd.c_str());
			
			//create output file
			string oname = treefolder + "/tree_" + filename + ".root";
			file = new TFile(oname.c_str(), "RECREATE");
			
			//create output tree
			if(clone){ //option to preserve all branches from input ntuple
				string treedesc = "all observables, " + name;
				tree = clone->CloneTree(0);
				tree->SetName("tree");
				tree->SetTitle(treedesc.c_str());
			}
			else{ //only add non-cloned trees to Selectors
				string treedesc = "selected observables, " + name;
				tree = new TTree("tree", treedesc.c_str());
				for(unsigned s = 0; s < selectorList.size(); s++){
					selectorList[s]->SetTree(tree);
				}
			}
		}
		//alternate output tree setup for scanning
		void ScanTree(TTree* clone){
			//create output file
			file = new TFile((name+".root").c_str(), "RECREATE");
			
			//create output tree, preserve all branches from input ntuple
			string treedesc = "all observables, " + name;
			tree = clone->CloneTree(0);
			tree->SetName("tree");
			tree->SetTitle(treedesc.c_str());
		}
		//perform all variations and selections on current event from skimmer
		bool DoSelection(){
			if(variation) variation->DoVariation();
			
			//filter returns false if event is in list
			bool selectedEvent = filter ? !filter->CheckEvent(looper->RunNum,looper->LumiBlockNum,looper->EvtNum) : true;
			bool result = selectedEvent;
			int lastSelector = -1;
			for(unsigned s = 0; s < selectorList.size(); s++){
				if(!result) break;
				result &= selectorList[s]->Select();
				lastSelector = s;
				if(!result) {
					break; //end loop as soon as selection fails
				}
			}
			if(filter and lastSelector>=0){
				cout << looper->RunNum << ":" << looper->LumiBlockNum << ":" << looper->EvtNum;
				if(!result) cout << " failed " << selectorList[lastSelector]->GetName();
				else cout << " passed";
				cout << " (" << looper->fChain->GetCurrentFile()->GetName() << ")" << endl;
				result = true;
			}
			
			//tree output
			if(result && tree) tree->Fill();

			//reset event
			if(variation) variation->UndoVariation();
			
			return result;
		}
		void GetEfficiency(){
			vector<KCutflowEntry> cuts;
			for(unsigned s = 0; s < selectorList.size(); s++){
				int prev_counter = 0;
				if(s>0) prev_counter = selectorList[s-1]->GetCounter();
				selectorList[s]->GetEfficiency(cuts);
			}
			
			//create histogram
			if(cutflowHist) { delete cutflowHist; cutflowHist = NULL; }
			cutflowHist = new TH1F("cutflow",name.c_str(),cuts.size(),0,cuts.size());
			for(unsigned c = 0; c < cuts.size(); c++){
				cutflowHist->GetXaxis()->SetBinLabel(c+1,cuts[c].name.c_str());
				cutflowHist->SetBinContent(c+1,cuts[c].raw);
				cutflowHist->SetBinError(c+1,cuts[c].rawE);
			}
			cutflowHist->GetXaxis()->SetNoAlphanumeric();
			
			//todo: add object ctr histogram for syncing
		}
		void PrintEfficiency(TH1F* nEventHist){
			if(!nEventHist) return;
			if(!cutflowHist) GetEfficiency();
			
			//check if error printing should be enabled
			bool printerrors = globalOpt->Get("printerrors",false);
			//use helper class to print
			KCutflow kcut(name,cutflowHist,nEventHist);
			kcut.PrintEfficiency(printerrors);
		}
		void PrintEfficiency(int nentries, double nentriesE=0){
			if(!cutflowHist) GetEfficiency();
			
			//check if error printing should be enabled
			bool printerrors = globalOpt->Get("printerrors",false);
			//use helper class to print
			KCutflow kcut(name,cutflowHist,nentries,nentriesE);
			kcut.PrintEfficiency(printerrors);
		}
		void Finalize(TH1F* nEventHist=NULL, TH1F* nEventNegHist=NULL){
			if(file){
				file->cd();
				if(nEventHist) nEventHist->Write();
				if(nEventNegHist) nEventNegHist->Write();
				if(cutflowHist) cutflowHist->Write();
				if(tree) tree->Write();
				
				//just in case selectors have something to add
				for(unsigned s = 0; s < selectorList.size(); s++){
					selectorList[s]->Finalize(file);
				}
				
				file->Close();
			}
		}
		//manually fill tree
		void FillTree(){
			if(tree) tree->Fill();
		}
		//check branches needed for selectors and variators
		void CheckBranches(){
			if(variation) variation->CheckBranches();
			for(unsigned s = 0; s < selectorList.size(); s++){
				if(!selectorList[s]->Dummy()) selectorList[s]->CheckBranches();
			}
		}
		
	private:
		//member variables
		string name;
		OptionMap* globalOpt;
		KVariation<T>* variation;
		T* looper;
		vector<KSelector<T>*> selectorList;
		KMap<KSelector<T>*> selectors;
		TFile* file;
		TTree* tree;
		TH1F* cutflowHist;
		EventListFilter* filter;
		string eventlist;
};

#endif
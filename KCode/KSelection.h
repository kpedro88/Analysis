#ifndef KSELECTION_H
#define KSELECTION_H

//custom headers
#include "KMap.h"
#include "KBase.h"
#include "KLooper.h"
#include "KMath.h"
#include "KVariation.h"
#include "KCutflow.h"

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

//forward declarations
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
class KSelector {
	public:
		//constructor
		KSelector() : name(""), localOpt(0), sel(0), looper(0), tree(0), counter(0), dummy(0), canfail(1), depfailed(0), forceadd(0), base(0) {
			//must always have local option map
			if(localOpt==0) localOpt = new OptionMap();
		}
		KSelector(string name_, OptionMap* localOpt_) : name(name_), localOpt(localOpt_), sel(0), looper(0), tree(0), counter(0), dummy(0), canfail(1), depfailed(0), forceadd(0), base(0) {
			//must always have local option map
			if(localOpt==0) localOpt = new OptionMap();
			dummy = localOpt->Get("dummy",false);
			forceadd = localOpt->Get("forceadd",false);
			//possible to change name from default
			localOpt->Get("name",name);
		}
		//destructor
		virtual ~KSelector() {}
		//accessors
		string GetName() { return name; }
		virtual void SetSelection(KSelection* sel_) { sel = sel_; CheckDeps(); } //set dependencies here if desired
		virtual void SetTree(TTree* tree_) { tree = tree_; SetBranches(); } //set tree branches here if desired
		virtual void SetBase(KBase* base_) { base = base_; looper = base->GetLooper(); CheckBase(); }
		int GetCounter() { return counter; }
		bool Dummy() { return dummy; }
		bool CanFail() { return canfail; }
		bool FailedDependency() { return depfailed; }
		bool ForceAdd() { return forceadd; }
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
		//to allow selectors to turn on branches they need
		virtual void CheckBranches() {}
		//to check dependencies on the base (and the looper)
		virtual void CheckBase() {}
		
	protected:
		//member variables
		string name;
		OptionMap* localOpt;
		KSelection* sel;
		KLooper* looper;
		TTree* tree;
		int counter;
		bool dummy, canfail, depfailed, forceadd;
		KBase* base;
};
typedef KFactory<KSelector,string,OptionMap*> KSelectorFactory;
#define REGISTER_SELECTOR(a) REGISTER_MACRO2(KSelectorFactory,K##a##Selector,a)
#define REGISTER_SELECTOR2(a,b) REGISTER_MACRO2(KSelectorFactory,K##a##Selector,b)

//----------------------------------------------------------------
//class to keep track of a list of Selectors
//has a vector for an ordered list and a map for a searchable list
class KSelection {
	public:
		//constructor
		KSelection() : name(""), variation(0), looper(0), file(0), tree(0), cutflowHist(0), base(0) {}
		KSelection(string name_, OptionMap* globalOpt_) : name(name_), globalOpt(globalOpt_), variation(0), looper(0), file(0), tree(0), cutflowHist(0), base(0) {
			//must always have option map
			if(globalOpt==0) globalOpt = new OptionMap();
		}
		//destructor
		virtual ~KSelection() {
			delete file;
		}
		//accessors
		string GetName() { return name; }
		OptionMap* GetGlobalOpt() { return globalOpt; }
		void SetVariation(KVariation* varn) { variation = varn; }
		void AddSelector(KSelector* sel_){
			selectorList.push_back(sel_);
			selectors.Add(sel_->GetName(),sel_);
			sel_->SetSelection(this);
		}
		//set base and looper together
		void SetBase(KBase* base_){
			base = base_;
			looper = base->GetLooper();
			for(unsigned s = 0; s < selectorList.size(); s++){
				selectorList[s]->SetBase(base_);
			}
			if(variation) variation->SetBase(base_);
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
			}
			//allow to force add branches even if cloned tree
			for(unsigned s = 0; s < selectorList.size(); s++){
				if(!clone or selectorList[s]->ForceAdd()){
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
			
			bool result = true;
			for(unsigned s = 0; s < selectorList.size(); s++){
				result &= selectorList[s]->Select();
				if(!result) break; //end loop as soon as selection fails
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
			KCutflow kcut("",cutflowHist,nEventHist);
			kcut.PrintEfficiency(printerrors);
		}
		void PrintEfficiency(int nentries, double nentriesE=0){
			if(!cutflowHist) GetEfficiency();
			
			//check if error printing should be enabled
			bool printerrors = globalOpt->Get("printerrors",false);
			//use helper class to print
			KCutflow kcut("",cutflowHist,nentries,nentriesE);
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
		KVariation* variation;
		KLooper* looper;
		vector<KSelector*> selectorList;
		KMap<KSelector*> selectors;
		TFile* file;
		TTree* tree;
		TH1F* cutflowHist;
		KBase* base;
};

//defined here to avoid circular dependency
void KBase::SetSelection(KSelection* sel_) { MySelection = sel_; MySelection->SetBase(this); }

//-------------------------------------------------------------
//addition to KParser to create selectors
namespace KParser {
	KSelector* processSelector(KNamed* tmp){
		string sname = tmp->fields[0];
		OptionMap* omap = tmp->localOpt();
		
		KSelector* srtmp = KSelectorFactory::GetFactory().construct(sname,sname,omap);
		
		if(!srtmp) cout << "Input error: unknown selector " << sname << ". This selector will be skipped." << endl;
		
		return srtmp;
	}
}

#endif

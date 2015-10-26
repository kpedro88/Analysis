#ifndef KSELECTION_H
#define KSELECTION_H

//custom headers
#include "KMap.h"
#include "KVariation.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>

//STL headers
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

//forward declaration
template <class T>
class KSelection;

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
		virtual void PrintEfficiency(vector<unsigned>& widths, int prev_counter, int nentries){
			if(dummy || !canfail) return;
			cout << left << setw(widths[0]) << name;
			cout << "  " << right << setw(widths[1]) << counter;
			cout << "  " << right << setw(widths[2]) << ((double)counter/(double)nentries)*100;
			//no rel. eff. for first selector
			if(prev_counter>0) cout  << "  " << right << setw(widths[3]) << ((double)counter/(double)prev_counter)*100;
			cout << endl;
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
		KSelection() : name(""), variation(0), looper(0), file(0), tree(0), widths(5,0), width1s(10) {}
		KSelection(string name_, OptionMap* globalOpt_) : name(name_), globalOpt(globalOpt_), variation(0), looper(0), file(0), tree(0), widths(5,0), width1s(10) {
			//must always have option map
			if(globalOpt==0) globalOpt = new OptionMap();
		}
		//destructor
		virtual ~KSelection() {}
		//accessors
		string GetName() { return name; }
		OptionMap* GetGlobalOpt() { return globalOpt; }
		void SetVariation(KVariation<T>* varn) { variation = varn; }
		void AddSelector(KSelector<T>* sel_){
			selectorList.push_back(sel_);
			selectors.Add(sel_->GetName(),sel_);
			sel_->SetSelection(this);
			if(!sel_->Dummy() && sel_->CanFail() && sel_->GetName().size()>width1s) width1s = sel_->GetName().size();
		}
		unsigned GetSelectorWidth() { return width1s; }
		void SetLooper(T* looper_){
			looper = looper_;
			for(unsigned s = 0; s < selectorList.size(); s++){
				selectorList[s]->SetLooper(looper_);
			}
			if(variation) variation->SetLooper(looper_);
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
		void PrintEfficiency(unsigned width1, int nentries=1){
			//width1s set when adding selectors, width1 set by skimmer to consider all selectors in all selections
			//default for width1s is 10 for "NEventProc"
			widths[0] = width1; widths[1] = widths[2] = widths[3] = 13;
			
			cout << string(widths[0]+widths[1]+widths[2]+widths[3]+widths[4]+2*(widths.size()-1),'-') << endl;
			cout << "Selection: " << name << endl;
			cout << left << setw(widths[0]) << "Selector" << "  " << " Raw # Events" << "  " << "Abs. Eff. (%)" << "  " << "Rel. Eff. (%)" << endl;
			cout << left << setw(widths[0]) << "NEventProc" << "  " << right << setw(widths[1]) << nentries << endl;
			for(unsigned s = 0; s < selectorList.size(); s++){
				int prev_counter = 0;
				if(s>0) prev_counter = selectorList[s-1]->GetCounter();
				selectorList[s]->PrintEfficiency(widths,prev_counter,nentries);
			}
		}
		void Finalize(TH1F* nEventHist=NULL, TH1F* nEventNegHist=NULL){
			if(file){
				file->cd();
				if(nEventHist) nEventHist->Write();
				if(nEventNegHist) nEventNegHist->Write();
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
		//check branches needed for selectors
		void CheckBranches(){
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
		vector<unsigned> widths;
		unsigned width1s;
};

#endif
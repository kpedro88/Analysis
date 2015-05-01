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
class KSelection;
class KSkimmer;

//----------------------------------------------------------------
//base class for Selectors, has standard functions defined
class KSelector {
	public:
		//constructor
		KSelector() : name(""), localOpt(0), sel(0), sk(0), tree(0), counter(0), dummy(0), canfail(1), depfailed(0) {
			//must always have local option map
			if(localOpt==0) localOpt = new OptionMap();
		}
		KSelector(string name_, OptionMap* localOpt_) : name(name_), localOpt(localOpt_), sel(0), sk(0), tree(0), counter(0), dummy(0), canfail(1), depfailed(0) {
			//must always have local option map
			if(localOpt==0) localOpt = new OptionMap();
			dummy = localOpt->Get("dummy",false);
		}
		//destructor
		virtual ~KSelector() {}
		//accessors
		string GetName() { return name; }
		virtual void SetSelection(KSelection* sel_) { sel = sel_; } //set dependencies here if desired
		virtual void SetSkimmer(KSkimmer* sk_) { sk = sk_; }
		virtual void SetTree(TTree* tree_) { tree = tree_; } //set tree branches here if desired
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
		//used for non-dummy selectors
		virtual bool Cut() { return true; }
		virtual void PrintEfficiency(vector<int>& widths, string sname, int prev_counter, int nentries){
			if(dummy || !canfail) return;
			cout << left << setw(widths[0]) << sname;
			cout << "  " << left << setw(widths[1]) << name;
			cout << "  " << right << setw(widths[2]) << counter;
			cout << "  " << right << setw(widths[3]) << ((double)counter/(double)nentries)*100;
			//no rel. eff. for first selector
			if(prev_counter>0) cout  << "  " << right << setw(widths[4]) << ((double)counter/(double)prev_counter)*100;
			cout << endl;
		}
		
	protected:
		//member variables
		string name;
		OptionMap* localOpt;
		KSelection* sel;
		KSkimmer* sk;
		TTree* tree;
		int counter;
		bool dummy, canfail, depfailed;
};

//----------------------------------------------------------------
//class to keep track of a list of Selectors
//has a vector for an ordered list and a map for a searchable list
class KSelection {
	public:
		//constructor
		KSelection() : name(""), variation(0), skimmer(0), file(0), tree(0), widths(5,0), width2s(0) {}
		KSelection(string name_) : name(name_), variation(0), skimmer(0), file(0), tree(0), widths(5,0), width2s(0) {}
		//destructor
		virtual ~KSelection() {}
		//accessors
		string GetName() { return name; }
		void SetVariation(KVariation* varn) { variation = varn; }
		void AddSelector(KSelector* sel_){
			selectorList.push_back(sel_);
			selectors.Add(sel_->GetName(),sel_);
			sel_->SetSelection(this);
			if(!sel_->Dummy() && sel_->CanFail() && sel_->GetName().size()>width2s) width2s = sel_->GetName().size();
		}
		int GetSelectorWidth() { return width2s; }
		void SetSkimmer(KSkimmer* skimmer_){
			skimmer = skimmer_;
			for(unsigned s = 0; s < selectorList.size(); s++){
				selectorList[s]->SetSkimmer(skimmer_);
			}
			if(variation) variation->SetSkimmer(skimmer_);
		}
		void DoVariation() { if(variation) variation->DoVariation(); }
		void UndoVariation() { if(variation) variation->UndoVariation(); }
		KSelector* operator[](int x){ return selectorList[x]; }
		template <class T> T Get(string name_){	return static_cast<T>(selectors.Get(name_)); }
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
		//perform all variations and selections on current event from skimmer
		bool DoSelection(){
			if(variation) variation->DoVariation();
			
			bool result = true;
			for(unsigned s = 0; s < selectorList.size(); s++){
				result &= selectorList[s]->Select();
				if(!result) break; //end loop as soon as selection fails
			}
			
			//tree output
			if(result) tree->Fill();

			//reset event
			if(variation) variation->UndoVariation();
			
			return result;
		}
		void PrintEfficiency(int width1, int width2, int nentries=1){
			//width1 set by skimmer when adding selections
			//width2s set when adding selectors, width2 set by skimmer to consider all selectors in all selections
			widths[0] = width1; widths[1] = width2; widths[2] = widths[3] = widths[4] = 13;
			bool started = false;
			
			cout << string(widths[0]+widths[1]+widths[2]+widths[3]+widths[4]+widths[5]+2*(widths.size()-1),'-') << endl;
			cout << left << setw(width1) << "Selection" << "  " << left << setw(width2) << "Selector" << "  " << " Raw # Events" << "  " << "Abs. Eff. (%)" << "  " << "Rel. Eff. (%)" << endl;
			for(unsigned s = 0; s < selectorList.size(); s++){
				//only print selection name the first time
				string sname = "";
				if(!started) { sname = name; started = true; }
				int prev_counter = 0;
				if(s>0) prev_counter = selectorList[s-1]->GetCounter();
				selectorList[s]->PrintEfficiency(widths,sname,prev_counter,nentries);
			}
		}
		void Finalize(TH1F* nEventHist=NULL){
			if(file){
				file->cd();
				if(nEventHist) nEventHist->Write();
				if(tree) tree->Write();
				file->Close();
			}
		}
		
	private:
		//member variables
		string name;
		KVariation* variation;
		KSkimmer* skimmer;
		vector<KSelector*> selectorList;
		SelectorMap selectors;
		TFile* file;
		TTree* tree;
		vector<int> widths;
		int width2s;
};

#endif
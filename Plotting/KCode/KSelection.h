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
		KSelector() : name(""), localOpt(0), sel(0), sk(0), tree(0), counter(0) {
			//must always have local option map
			if(localOpt==0) localOpt = new OptionMap();
		}
		KSelector(string name_, OptionMap* localOpt_) : name(name_), localOpt(localOpt_), sel(0), sk(0), tree(0), counter(0), dummy(0) {
			//must always have local option map
			if(localOpt==0) localOpt = new OptionMap();
			dummy = localOpt->Get("dummy",false);
		}
		//destructor
		virtual ~KSelection() {}
		//accessors
		string GetName() { return name; }
		virtual void SetSelection(KSelection* sel_) { sel = sel_; } //set dependencies here if desired
		virtual void SetSkimmer(KSkimmer* sk_) { sk = sk_; }
		virtual void SetTree(TTree* tree_) { tree = tree_; } //set tree branches here if desired
		int GetCounter() { return counter; }
		//selection function, checks dummy first
		virtual bool Select(){
			bool result = dummy || Cut();
			if(result) counter++;
			return result;
		}
		//used for non-dummy selectors
		virtual bool Cut() { return true; }
		//set variable values for tree
		virtual void Store() {}
		
	protected:
		//member variables
		string name;
		OptionMap* localOpt;
		KSelection* sel;
		KSkimmer* sk;
		TTree* tree;
		int counter;
		bool dummy;
};

//----------------------------------------------------------------
//class to keep track of a list of Selectors
//has a vector for an ordered list and a map for a searchable list
class KSelection {
	public:
		//constructor
		KSelection() : name(""), variation(0), skimmer(0), file(0), tree(0) {}
		KSelection(string name_) : name(name_), variation(0), skimmer(0), file(0), tree(0) {}
		//destructor
		virtual ~KSelection() {}
		//accessors
		string GetName() { return name; }
		void SetVariation(KVariation* varn) { variation = varn; }
		void AddSelector(KSelector* sel_){
			selectorList.push_back(sel_);
			selectors.Add(sel_->GetName(),sel_);
			sel_.SetSelection(this);
		}
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
		KSelector* Get(string name_){ return selectors.Get(name_); }
		//setup output tree
		void MakeTree(string outdir, string filename){
			//make sure the folder exists
			string treefolder = outdir + "_" + name;
			string syscmd = "mkdir -p " + treefolder;
			system(syscmd.c_str());
			
			//remove "skim" from filename if present
			if(filename.compare("skim",0,4)==0) filename.erase(0,4);
			
			//create output file
			string oname = treefolder + "/tree_" + filename;
			file = new TFile(oname.c_str(), "RECREATE");
			
			//create output tree
			if(skimmer->globalOpt->Get("doClone",false)){ //option to preserve all branches from input ntuple
				string treedesc = "all observables, " + name;
				tree = skimmer->fChain->CloneTree(0);
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
			if(result) {
				//store tree variables from selectors only if event is selected
				for(unsigned s = 0; s < selectorList.size(); s++){
					selectorList[s]->Store();
				}
				tree->Fill();
			}
			//reset event
			if(variation) variation->UndoVariation();
			
			return result;
		}
		void PrintEfficiency(){
			//todo: make the spacing & formatting nicer here
			cout << "Selection" << "\t" << "Selector" << "\t" << "Abs. Eff. (%)" << "\t" << "Rel. Eff. (%)" << endl;
			for(unsigned s = 0; s < selectorList.size(); s++){
				if(s==0) cout << name << "\t";
				else cout << "\t" << "\t";
				cout << selectorList[s]->GetName() << "\t" << (selectorList[s]->GetCounter()/((double)skimmer->nentries))*100 << "\t";
				//no rel. eff. for first selector
				if(s>0) cout << (selectorList[s]->GetCounter()/selectorList[s-1]->GetCounter())*100;
				cout << endl;
			}
			cout << string('-',60);
		}
		void Finalize(){
			if(file){
				file->cd();
				if(skimmer->nEventHist) skimmer->nEventHist->Write();
				if(tree) tree->Write();
				file->close();
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
};

#endif
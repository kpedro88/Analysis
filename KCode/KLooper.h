#ifndef KLOOPER_H
#define KLOOPER_H

#ifndef NtupleClass_cxx
#define NtupleClass_cxx
#endif

//custom headers
#include "THN.h"
#include "KMap.h"
#include "NtupleClass.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1.h>

//STL headers
#include <string>
#include <vector>
#include <exception>

//forward declaration
class KBase;

using namespace std;

void NtupleClass::Loop() {}

//helper
class KOpenerName {
	public:
		//constructor
		KOpenerName(OptionMap* localOpt, OptionMap* globalOpt) : name(""), filename(""), treedir(""), use_treedir(false), is_chain(false) {
			use_treedir = globalOpt->Get("treedir",treedir);
			if(localOpt->Get("chain",false)){
				is_chain = true;
				if(!localOpt->Get("filenames",chain) || chain.GetFiles().size()==0){
					throw runtime_error("Missing filenames for chain");
				}
				if(use_treedir) name = treedir+"_";
				name += chain.GetName();
			}
			else if(localOpt->Get("filename",filename)){
				if(use_treedir) name = treedir+"_";
				name += filename;
			}
			KParser::clean(name,'_');
		}
		
		//member variables
		string name;
		string filename;
		string treedir;
		KChain chain;
		bool use_treedir;
		bool is_chain;
};

//file opening base class
class KOpener {
	public:
		//constructors
		KOpener() : globalOpt(new OptionMap()), file(0), tree(0) {}
		KOpener(OptionMap* localOpt, OptionMap* globalOpt_) : 
			globalOpt(globalOpt_ ? globalOpt_ : new OptionMap()), file(0), tree(0)
		{
			//local map not stored, just used for initialization
			if(localOpt==0) localOpt = new OptionMap();

			//default list of hists to fetch
			vector<string> hnames{"nEventProc","nEventNeg"};
			//local overrides global
			globalOpt->Get("fetch_histos",hnames);
			localOpt->Get("fetch_histos",hnames);

			KOpenerName helper(localOpt,globalOpt);
			name = helper.name;
			string filename = helper.filename;
			string treedir = helper.treedir;
			bool use_treedir = helper.use_treedir;
			if(helper.is_chain){
				KChain& chain = helper.chain;
				const vector<string>& filenames = chain.GetFiles();
				
				//add filenames to chain
				tree = new TChain(("tree_"+name).c_str());
				string chainsuff = "";
				localOpt->Get("chainsuff",chainsuff);

				bool quickchain = globalOpt->Get("quickchain",false);
				if(quickchain){
					for(unsigned f = 0; f < filenames.size(); f++){
						filename = filenames[f];
						if(use_treedir) filename = treedir + "/" + filename;
						static_cast<TChain*>(tree)->Add((filename+chainsuff).c_str());
					}
				}
				else {
					for(unsigned f = 0; f < filenames.size(); f++){
						filename = filenames[f];
						if(use_treedir) filename = treedir + "/" + filename;

						//here we may want to construct a partial chain
						TFile* ftmp = KOpen(filename,"READ",false);
						if(!ftmp) {
							cout << "Warning: file " << filename << " cannot be found or opened. Object " << name << " will not be fully initialized." << endl;
							continue;
						}
						
						static_cast<TChain*>(tree)->Add((filename+chainsuff).c_str());

						//these aren't necessarily required
						for(const auto& hname : hnames){
							GetAndAdd(ftmp, hname);
						}

						ftmp->Close();
					}
				}
				if(tree->GetEntries()==0) throw runtime_error("no files could be opened. Object "+name+" will not be initialized.");
			}
			else{
				//get directory from global
				if(use_treedir) filename = treedir + "/" + filename;
				//open file
				file = KOpen(filename);

				//get tree
				string treename = "tree";
				localOpt->Get("treename",treename);
				tree = KGet<TTree>(file,treename); //this might cause an issue for ext

				//these aren't necessarily required
				for(const auto& hname : hnames){
					hists.Add(hname, KGetTHN(file,hname,false));
				}
			}
		}
		//destructor
		virtual ~KOpener() {
			for(auto& h : hists.GetTable()){
				if(h.second->TH1()) delete h.second->TH1();
				else if(h.second->THnSparse()) delete h.second->THnSparse();
				delete h.second;
			}
		}
		//accessors
		string GetName() { return name; }
		OptionMap* GetGlobalOpt() { return globalOpt; }
		void SetGlobalOpt(OptionMap* opt) { globalOpt = opt ? opt : new OptionMap(); } //must always have an option map
		TFile* GetFile() { return file; }
		virtual void CloseFile() { if(file) file->Close(); }
		virtual TTree* GetTree() { return tree; }
		virtual THN* GetHist(const string& hname) { return hists.Get(hname); }

	protected:
		//helper
		void GetAndAdd(TFile* ftmp, const string& hname){
			THN* htmp = KGetTHN(ftmp,hname,false);
			//sum up
			if(htmp) {
				THN* hist = GetHist(hname); 
				if(hist) hist->Add(htmp);
				else {
					hist = htmp->Clone(hname.c_str());
					hist->SetDirectory(0);
					hists.Add(hname,hist);
				}
			}
		}

		//member variables
		string name;
		OptionMap* globalOpt;
		TFile* file;
		TTree* tree;
		KMap<THN*> hists;
};

//use first base class to get tree before initializing MakeClass base
class KLooper : public KOpener, public NtupleClass {
	public:
		typedef KMap<KLooper*> LooperMap;
		//constructors
		KLooper() : KOpener(), NtupleClass() {}
		KLooper(OptionMap* localOpt, OptionMap* globalOpt_) : KOpener(localOpt, globalOpt_), NtupleClass(tree) {
			AddLooper(this);
		}
		//destructor
		virtual ~KLooper() {}
		//virtual functions
		virtual void Loop() {}
		virtual void AddBase(KBase* base) {}
		//helper functions
		virtual void EnableBranches(const vector<string>& branches){
			for(const auto& branch : branches){
				fChain->SetBranchStatus(branch.c_str(),1);
			}
		}
		virtual void DisableBranches(const vector<string>& branches){
			for(const auto& branch : branches){
				fChain->SetBranchStatus(branch.c_str(),0);
			}
		}
		//static functions
		static LooperMap& GetLooperMap(){
			static LooperMap lmap;
			return lmap;
		}
		static KLooper* FindLooper(OptionMap* localOpt, OptionMap* globalOpt){
			//standardize name processing
			KOpenerName helper(localOpt,globalOpt);
			return GetLooperMap().Get(helper.name);
		}
		static void AddLooper(KLooper* ltmp){
			GetLooperMap().Add(ltmp->GetName(),ltmp);
		}
		
	protected:
		//member variables
};

#endif

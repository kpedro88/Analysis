#ifndef KLOOPER_H
#define KLOOPER_H

#ifndef NtupleClass_cxx
#define NtupleClass_cxx
#endif

//custom headers
#include "KMap.h"
#include "NtupleClass.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1.h>

//forward declaration
class KBase;

void NtupleClass::Loop() {}

//helper
class KOpenerName {
	public:
		//constructor
		KOpenerName(OptionMap* localOpt, OptionMap* globalOpt) : name(""), filename(""), treedir(""), use_treedir(false), is_chain(false), failed_chain(false) {
			use_treedir = globalOpt->Get("treedir",treedir);
			if(localOpt->Get("chain",false)){
				is_chain = true;
				if(!localOpt->Get("filenames",chain) || chain.GetFiles().size()==0){
					failed_chain = true;
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
		bool is_chain, failed_chain;
};

//file opening base class
class KOpener {
	public:
		//constructors
		KOpener() : globalOpt(new OptionMap()), file(0), tree(0), nEventHist(0), nEventNegHist(0) {}
		KOpener(OptionMap* localOpt, OptionMap* globalOpt_) : 
			globalOpt(globalOpt_ ? globalOpt_ : new OptionMap()), file(0), tree(0), nEventHist(0), nEventNegHist(0)
		{
			//local map not stored, just used for initialization
			if(localOpt==0) localOpt = new OptionMap();
			
			KOpenerName helper(localOpt,globalOpt);
			name = helper.name;
			string filename = helper.filename;
			string treedir = helper.treedir;
			bool use_treedir = helper.use_treedir;
			if(helper.is_chain){
				KChain& chain = helper.chain;
				if(helper.failed_chain){
					cout << "Input error: filenames not specified for chain. Object " << chain.GetName() << " will not be initialized." << endl;
					return;
				}
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
						
						TFile* ftmp = TFile::Open(filename.c_str());
						if(!ftmp) {
							cout << "Input error: file " << filename << " cannot be found or opened. Object " << name << " will not be fully initialized." << endl;
							continue;
						}
						
						static_cast<TChain*>(tree)->Add((filename+chainsuff).c_str());
						TH1F* nEventHistTmp = (TH1F*)ftmp->Get("nEventProc");
						//sum up nEventProc histos
						if(nEventHistTmp) {
							if(nEventHist) nEventHist->Add(nEventHistTmp);
							else {
								nEventHist = (TH1F*)nEventHistTmp->Clone("nEventProc");
								nEventHist->SetDirectory(0);
							}
						}
						TH1F* nEventNegHistTmp = (TH1F*)ftmp->Get("nEventNeg");
						//sum up nEventNeg histos
						if(nEventNegHistTmp) {
							if(nEventNegHist) nEventNegHist->Add(nEventNegHistTmp);
							else {
								nEventNegHist = (TH1F*)nEventNegHistTmp->Clone("nEventNeg");
								nEventNegHist->SetDirectory(0);
							}
						}
						ftmp->Close();
					}
				}
				if(tree->GetEntries()==0){
					cout << "Input error: no files could be opened. Object " << name << " will not be initialized." << endl;
					delete tree;
					tree = NULL;
					return;
				}
			}
			else{
				//get directory from global
				if(use_treedir) filename = treedir + "/" + filename;
				//open file
				file = TFile::Open(filename.c_str());
				if(!file) {
					cout << "Input error: file " << filename << " cannot be found or opened. Object will not be initialized." << endl;
					return;
				}
				//get tree
				string treename = "tree";
				localOpt->Get("treename",treename);
				tree = (TTree*)file->Get(treename.c_str());
				
				nEventHist = (TH1F*)file->Get("nEventProc");
				nEventNegHist = (TH1F*)file->Get("nEventNeg");
			}
		}
		//destructor
		virtual ~KOpener() {}
		//accessors
		string GetName() { return name; }
		OptionMap* GetGlobalOpt() { return globalOpt; }
		void SetGlobalOpt(OptionMap* opt) { globalOpt = opt ? opt : new OptionMap(); } //must always have an option map
		TFile* GetFile() { return file; }
		virtual void CloseFile() { if(file) file->Close(); }
		virtual TTree* GetTree() { return tree; }
		virtual TH1F* GetNEventHist() { return nEventHist; }
		virtual TH1F* GetNEventNegHist() { return nEventNegHist; }

	protected:
		//member variables
		string name;
		OptionMap* globalOpt;
		TFile* file;
		TTree* tree;
		TH1F *nEventHist, *nEventNegHist;
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

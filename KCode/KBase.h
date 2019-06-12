#ifndef KBASE_H
#define KBASE_H

//custom headers
#include "KMap.h"
#include "KFactory.h"
#include "KLooper.h"
#include "KLegend.h"
#include "KStyle.h"
#include "KCutflow.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1.h>
#include <TH2.h>
#include <TH1F.h>
#include <TPad.h>
#include <TLegend.h>
#include <TMath.h>
#include <TStyle.h>
#include <TCollection.h>
#include <TKey.h>

//STL headers
#include <vector>
#include <string>
#include <iostream>
#include <exception>

using namespace std;

//forward declaration
class KSelection;

//------------------------------------------------------------------------------------------------------------------
//base class: virtual methods and info for bases, other base/set classes inherit from it
class KBase {
	public:
		//constructors
		KBase() :
			name(""), parent(0), localOpt(new OptionMap()), globalOpt(new OptionMap()), 
			file(0), tree(0), nEventHist(0), nEventNegHist(0), MyCutflow(0),
			MyLooper(0), MySelection(0), stmp(""), htmp(0), khtmp(0), etmp(0), isBuilt(false), MyStyle(0)
		{
			//enable histo errors
			TH1::SetDefaultSumw2(kTRUE);
		}
		KBase(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : 
			name(name_), parent(0), localOpt(localOpt_ ? localOpt_ : new OptionMap()), globalOpt(globalOpt_ ? globalOpt_ : new OptionMap()), 
			file(0), tree(0), nEventHist(0), nEventNegHist(0), MyCutflow(0), 
			MyLooper(0), MySelection(0), stmp(""), htmp(0), khtmp(0), etmp(0), isBuilt(false), MyStyle(0)
		{
			debugroc = globalOpt->Get("debugroc",false);
		}
		//subsequent initialization
		virtual void SetLooper(){
			//try to find existing looper
			MyLooper = KLooper::FindLooper(localOpt,globalOpt);
			//if none, make one
			if(!MyLooper) MyLooper = MakeLooper();
			else MyLooper->AddBase(this); //in case of KBuilder
			file = MyLooper->GetFile();
			tree = MyLooper->GetTree();
			nEventHist = MyLooper->GetNEventHist();
			nEventNegHist = MyLooper->GetNEventNegHist();

			//store this value (number of events processed) at the beginning so histo only has to be accessed once
			int nEventProc = 0;
			if(nEventHist and not localOpt->Get("nEventProc",nEventProc)) {
				nEventProc = nEventHist->GetBinContent(1);
				//for samples with negative weights, Neff = Npos - Nneg = Ntot - 2*Nneg
				if(nEventNegHist) nEventProc -= 2*(nEventNegHist->GetBinContent(1));
			}
			localOpt->Set("nEventProc",max(nEventProc,1));
		}
		virtual KLooper* MakeLooper() { return new KLooper(localOpt,globalOpt); }
		//destructor
		virtual ~KBase() {}

		//functions for histo creation
		virtual TGraphAsymmErrors* BuildErrorBand(){
			//make sim error band
			TGraphAsymmErrors* esim = new TGraphAsymmErrors(htmp->GetNbinsX()+2); //under- and overflow
			for(int b = 0; b < esim->GetN(); b++){
				if(htmp->GetBinContent(b)>0){
					esim->SetPoint(b,htmp->GetBinCenter(b),htmp->GetBinContent(b)); //set to y value of sim
					double width = htmp->GetBinWidth(b);
					esim->SetPointEXlow(b, width/2.);
					esim->SetPointEXhigh(b, width/2.);
					esim->SetPointEYlow(b, htmp->GetBinError(b));
					esim->SetPointEYhigh(b, htmp->GetBinError(b));
				}
			}
			esim->SetFillColor(kGray+1);
			//esim->SetFillStyle(3013);
			//esim->SetFillStyle(3001);
			esim->SetFillStyle(3005);
			
			MyErrorBands.Add(stmp,esim);
			etmp = esim;
			return esim;
		}
		
		//accessors
		string GetName() { return name; }
		void SetName(string n) { name = n; }
		string GetLegName() {
			string legname = name;
			localOpt->Get("legname",legname);
			return legname;
		}
		KLooper* GetLooper() { return MyLooper; }
		void SetSelection(KSelection* sel_); //defined in KSelection.h
		KSelection* GetSelection() { return MySelection; }
		//implemented in KHisto.h
		virtual TH1* AddHisto(string s, TH1* h, OptionMap* omap=NULL);
		//gets current histo
		virtual TH1* GetHisto(){ return htmp; }
		//gets current histo name
		virtual string GetHistoName() { return stmp; }
		//resets current name and histo
		virtual TH1* GetHisto(string hname) {
			TH1* hist = MyHistos.Get(hname);
			etmp = MyErrorBands.Get(hname); //it's okay for etmp to be null
			efftmp = MyEffs.Get(hname); //will be calculated later if needed
			if(hist) {
				stmp = hname;
				htmp = hist;
				khtmp = MyKHistos.Get(hname);
				return htmp;
			}
			else return NULL; //do not reset if the histo does not exist
		}
		virtual void SaveHisto(string pname, TFile* file){
			file->cd();
			string outname = name;
			localOpt->Get("outname",outname);
			string oname = pname + "_" + outname;
			htmp->SetName(oname.c_str());
			htmp->Write(oname.c_str());
		}
		virtual KHisto* GetKHisto(string hname){
			if(GetHisto(hname)) return khtmp;
			else return NULL;
		}
		virtual KHisto* GetKHisto(){ return khtmp; }
		//returns efficiency for cut on current histo qty
		//does calculation and stores result if necessary
		virtual vector<double>* GetEff(){
			if(efftmp) return efftmp;
			
			//calculate efficiencies
			//normal: yield(i,nbins)/yield(0,nbins)
			efftmp = new vector<double>(htmp->GetNbinsX()+1);
			double ydenom = htmp->Integral(0,htmp->GetNbinsX()+1);
			if(debugroc) cout << name << " " << stmp << ":";
			for(int b = 0; b < htmp->GetNbinsX()+1; b++){
				efftmp->at(b) = htmp->Integral(b,htmp->GetNbinsX()+1)/ydenom;
				if(debugroc) cout << " " << efftmp->at(b);
			}
			if(debugroc) cout << endl;
			
			MyEffs.Add(stmp,efftmp);
			return efftmp;
		}
		virtual map<string,TH1*>& GetTable() { return MyHistos.GetTable(); }
		virtual map<string,KHisto*>& GetKTable() { return MyKHistos.GetTable(); }
		KBase* GetParent() { return parent; }
		void SetParent(KBase* p) {
			parent = p;
			//cout << name << " set parent " << parent->GetName() << endl;
		}
		OptionMap* GetLocalOpt() { return localOpt; }
		void SetLocalOpt(OptionMap* opt) { localOpt = opt; if(localOpt==0) localOpt = new OptionMap(); } //must always have an option map
		OptionMap* GetGlobalOpt() { return globalOpt; }
		void SetGlobalOpt(OptionMap* opt) { globalOpt = opt; if(globalOpt==0) globalOpt = new OptionMap(); } //must always have an option map
		virtual void PrintYield() {
			double err = 0;
			double hint = htmp->IntegralAndError(0,htmp->GetNbinsX()+1,err);
			cout << name << ": " << hint << " +/- " << err << endl;
		}
		virtual double GetYield() { return htmp->Integral(0,htmp->GetNbinsX()+1); }
		TFile* GetFile() { return file; }
		virtual void CloseFile() { if(file) file->Close(); }
		//divide current histo by bin width, default implementation
		virtual void BinDivide(){
			for(int b = 1; b <= htmp->GetNbinsX(); b++){
				htmp->SetBinContent(b,htmp->GetBinContent(b)/htmp->GetBinWidth(b));
				htmp->SetBinError(b,htmp->GetBinError(b)/htmp->GetBinWidth(b));
			}
		}
		//implemented in KHisto.h
		virtual void Normalize(double nn, bool toYield=true);
		virtual void MakeCutflows(){
			MyCutflow = new KCutflow(name,file);
		}
		virtual TTree* GetTree() { return tree; }
		virtual TH1F* GetNEventHist() { return nEventHist; }
		virtual TH1F* GetNEventNegHist() { return nEventNegHist; }
		virtual TH1F* GetCutflow(KCutflow::CutflowType ct=KCutflow::CutRaw) {
			if(!MyCutflow) MakeCutflows();
			return (MyCutflow ? MyCutflow->GetEfficiency(ct,globalOpt->Get("cutflownorm",false)) : NULL);
		}
		virtual void SetStyle(KMap<string>& allStyles, string styleName="") {
			if(styleName.size()==0) return;
			
			//search the list of default styles
			if(allStyles.Has(styleName)){
				KNamed* ntmp = KParser::processNamed<1>(styleName+"\t"+allStyles.Get(styleName));
				MyStyle = new KStyle(ntmp->fields[0],ntmp->localOpt(),localOpt);
			}
		}
		virtual void SetStyle(const KStyle* orig){
			MyStyle = new KStyle(*orig);
			MyStyle->SetLocalOpt(localOpt);
		}
		virtual KStyle* GetStyle() { return MyStyle; }
		virtual bool IsData() { return localOpt->Get("data",false); }
		virtual bool IsMC() { return !localOpt->Get("data",true); }
		virtual void SetBuilt() { isBuilt = true; }
		
		//other virtual functions (unimplemented at this level)
		virtual void Draw(TPad*) {}
		virtual void AddToLegend(KLegend*) {}
		virtual void AddChild(KBase*) {}
		virtual void SetAddExt(bool) {}
		virtual void Build() {}
		virtual void Build(TH1*) {}
		
	protected:
		//member variables
		string name;
		KBase* parent;
		OptionMap* localOpt;
		OptionMap* globalOpt;
		TFile* file;
		TTree* tree;
		TH1F *nEventHist, *nEventNegHist;
		KCutflow* MyCutflow;
		KLooper* MyLooper;
		KSelection* MySelection;
		HistoMap MyHistos;
		KHistoMap MyKHistos;
		ErrorMap MyErrorBands;
		KMap<vector<double>*> MyEffs;
		string stmp;
		TH1* htmp;
		KHisto* khtmp;
		TGraphAsymmErrors* etmp;
		vector<double>* efftmp;
		bool isBuilt;
		bool debugroc;
		KStyle* MyStyle;
};
typedef KFactory<KBase,string,OptionMap*,OptionMap*> KBaseFactory;
#define REGISTER_SET(a,b,c) REGISTER_MACRO2(KBaseFactory,a,b##c)

//avoid circular dependency
void KChecker::SetBase(KBase* base_) { base = base_; looper = base->GetLooper(); CheckBase(); } //get looper from base

//-------------------------------------------
//extension of base class for external histos
class KBaseExt : public KBase {
	public:
		//constructors
		KBaseExt() : KBase(), add_ext(false) {}
		KBaseExt(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KBase(name_, localOpt_, globalOpt_), add_ext(false) {
			string filename;
			//first check for global file, then local file
			globalOpt->Get("extfilename",filename);
			localOpt->Get("extfilename",filename);
			if(filename.empty()) throw runtime_error("No extfilename specified for "+name);
			file = KOpen(filename);

			//check for importing histos automatically
			bool ext_auto = localOpt->Get("ext_auto",false);
			string hsuff;
			if(localOpt->Get("exthisto_suff",hsuff)) ext_auto = true;
			else hsuff = name;
			string hextra; localOpt->Get("exthisto_extra",hextra);
			//append extra suffix to default suffix
			if(!hextra.empty()) hsuff += "_"+hextra;

			add_ext = true;
			if(ext_auto){
				TKey* key;
				TIter next(file->GetListOfKeys());
				while((key = (TKey*)next())) {
					string ntmp = key->GetName();
					//look for names in the format histo_suff(_extra)
					if(ntmp.size() > hsuff.size() and ntmp.compare(ntmp.size()-hsuff.size(),hsuff.size(),hsuff)==0){
						auto extmp = KGet<TH1>(file,ntmp);
						AddHisto(ntmp.substr(0,ntmp.size()-hsuff.size()-1),extmp);
					}
				}
			}
			else {
				//check for specific histos to import
				vector<string> exthisto_in;
				vector<string> exthisto_out;
				localOpt->Get("exthisto_in",exthisto_in);
				localOpt->Get("exthisto_out",exthisto_out);
				if(exthisto_in.size() != exthisto_out.size()){
					throw runtime_error("vectors of external histo input and output names must have the same length (in "+name+")");
				}
				for(unsigned i = 0; i < exthisto_in.size(); i++){
					auto extmp = KGet<TH1>(file,exthisto_in[i]);
					AddHisto(exthisto_out[i],extmp);
				}
			}
			add_ext = false;
		}
		
		//change histo add mode
		void SetAddExt(bool ae) { add_ext = ae; }
		//histo add so external histos won't get overwritten
		virtual TH1* AddHisto(string s, TH1* h, OptionMap* omap=NULL){
			//set current name
			stmp = s;
			
			if(!add_ext){ //if the histo being added is not from ext, check to see if it is already added
				TH1* hist = MyHistos.Get(s);
				if(hist){ //if it is already added, just use it, do not overwrite it
					htmp = hist;
					return hist;
				}
			}
			
			//otherwise, set current histo the usual way
			htmp = (TH1*)h->Clone();
			MyHistos.Add(stmp,htmp);
			return htmp;
		}
		
		//external histos do not need to build
		using KBase::Build;
		virtual void Build() {}
		
	private:
		//member variables
		bool add_ext;
};
REGISTER_SET(KBaseExt,base,ext);

//------------------------------------------------
//sort children by peak height of a specific histo
class KComp {
	public:
		//constructors
		KComp() : name("") {}
		KComp(string name_) : name(name_) {}
		bool operator() (KBase* ch1, KBase* ch2) {
			//first safety check
			if(!ch1 || !ch2) return false;
		
			TH1* h1 = 0;
			TH1* h2 = 0;
			
			//no name - select current histo
			if(name.size()==0) {
				h1 = ch1->GetHisto();
				h2 = ch2->GetHisto();
			}
			//otherwise, reset current histo to this name
			else {
				h1 = ch1->GetHisto(name);
				h2 = ch2->GetHisto(name);
			}
			double p1, p2;
			p1 = p2 = 0; //equality by default
			
			//safety check
			if(h1 && h2) {
				p1 = h1->GetBinContent(h1->GetMaximumBin());
				p2 = h2->GetBinContent(h2->GetMaximumBin());
			}
			
			//other cases - if one histo DNE, it is lower than the other
			else if(h1 && !h2) { p1 = 1; p2 = 0; }
			else if(!h1 && h2) { p1 = 0; p2 = 1; }

			return (p1<p2);
		}
		
	protected:
		//member variable
		string name;
};

//extension of namespace for processing bases/sets
namespace KParser {
	KBase* processBase(KNamedBase* named, OptionMap* globalOpt){
		//universal variables
		string type = named->fields[0];
		string subtype = named->fields[1];
		string name = named->fields[2];
		OptionMap* omap = named->localOpt();
	
		//create object
		KBase* tmp = KBaseFactory::GetFactory().construct(type+subtype,name,omap,globalOpt);
		
		return tmp;
	}
	KBase* processBase(string line, OptionMap* globalOpt){
		return processBase(processNamed<3>(line),globalOpt);
	}
}

#endif

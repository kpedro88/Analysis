#ifndef KBASE_H
#define KBASE_H

//custom headers
#include "KMap.h"
#include "KLegend.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH1F.h>
#include <TPad.h>
#include <TLegend.h>
#include <TMath.h>
#include <TStyle.h>

//STL headers
#include <vector>
#include <string>
#include <iostream>

using namespace std;

//forward declaration of builder
class KBuilder;

//------------------------------------------------------------------------------------------------------------------
//base class: virtual methods and info for bases, other base/set classes inherit from it
class KBase {
	public:
		//constructors
		KBase() : name(""), parent(0), localOpt(0), globalOpt(0), file(0), tree(0), MyBuilder(0), stmp(""), htmp(0), etmp(0), isBuilt(false) {
			//enable histo errors
			TH1::SetDefaultSumw2(kTRUE);
			//must always have local & global option maps
			if(localOpt==0) localOpt = new OptionMap();
			if(globalOpt==0) globalOpt = new OptionMap();
		}
		KBase(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : name(name_), parent(0), localOpt(localOpt_), globalOpt(globalOpt_), file(0), tree(0), MyBuilder(0), stmp(""), 
																	htmp(0), etmp(0), isBuilt(false) {
			//must always have local & global option maps
			if(localOpt==0) localOpt = new OptionMap();
			if(globalOpt==0) globalOpt = new OptionMap();
			
			string filename;
			string treedir;
			if(localOpt->Get("filename",filename)){
				if(globalOpt->Get("treedir",treedir)) { //get directory from global
					filename = treedir + "/" + filename;
					localOpt->Set("filename",filename); //store full filename for future use
				}
				//open file
				file = TFile::Open(filename.c_str());
				if(!file) {
					cout << "Input error: file " << filename << " cannot be found or opened. Object " << name << " will not be fully initialized." << endl;
					return;
				}
				//get tree
				tree = (TTree*)file->Get("tree");
				//store this value (number of events processed) at the beginning so histo only has to be accessed once
				int nEventProc = 1;
				TH1F* nEventHist = (TH1F*)file->Get("nEventProc");
				if(nEventHist) nEventProc = nEventHist->GetBinContent(1);
				localOpt->Set("nEventProc",nEventProc);
			}
		}
		//destructor
		virtual ~KBase() {}

		//functions for histo creation
		virtual void Build(); //implemented in KBuilder.h to avoid circular dependency
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
			esim->SetFillStyle(3001);
			
			MyErrorBands.Add(stmp,esim);
			etmp = esim;
			return esim;
		}
		
		//accessors
		string GetName() { return name; }
		void SetName(string n) { name = n; }
		//add a blank histo for future building
		virtual TH1* AddHisto(string s, TH1* h){
			//sets current name and histo
			stmp = s;
			htmp = (TH1*)h->Clone();
			htmp->Sumw2();
			MyHistos.Add(stmp,htmp);
			return htmp;
		}
		//gets current histo
		virtual TH1* GetHisto(){ return htmp; }
		//gets current histo name
		virtual string GetHistoName() { return stmp; }
		//resets current name and histo
		virtual TH1* GetHisto(string hname) {
			TH1* hist = MyHistos.Get(hname);
			etmp = MyErrorBands.Get(hname); //it's okay for etmp to be null
			if(hist) {
				stmp = hname;
				htmp = hist;
				return htmp;
			}
			else return NULL; //do not reset if the histo does not exist
		}
		virtual map<string,TH1*>& GetTable() { return MyHistos.GetTable(); }
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
		virtual void CloseFile() { file->Close(); }
		//divide current histo by bin width, default implementation
		virtual void BinDivide(){
			for(int b = 1; b <= htmp->GetNbinsX(); b++){
				htmp->SetBinContent(b,htmp->GetBinContent(b)/htmp->GetBinWidth(b));
				htmp->SetBinError(b,htmp->GetBinError(b)/htmp->GetBinWidth(b));
			}
		}
		//in case of normalization to yield or other scaling
		virtual void Normalize(double nn, bool toYield=true){
			double simyield = htmp->Integral(0,htmp->GetNbinsX()+1);
			if(toYield) htmp->Scale(nn/simyield);
			else htmp->Scale(nn);
		}

		//other virtual functions (unimplemented at this level)
		virtual void Draw(TPad* pad) {}
		virtual void GetLegendInfo(KLegend* kleg) {}
		virtual void AddToLegend(TLegend* leg) {}
		virtual void AddChild(KBase* ch) {}
		virtual void SetAddExt(bool ae) {}
		virtual void Build(TH1* hrat_) {}
		
	protected:
		//member variables
		string name;
		KBase* parent;
		OptionMap* localOpt;
		OptionMap* globalOpt;
		TFile* file;
		TTree* tree;
		KBuilder* MyBuilder;
		HistoMap MyHistos;
		ErrorMap MyErrorBands;
		string stmp;
		TH1* htmp;
		TGraphAsymmErrors* etmp;
		bool isBuilt;
};

//---------------------------------------------------------------
//extension of base class for data - has default intlumi
class KBaseData : public KBase {
	public:
		//constructors
		KBaseData() : KBase() { localOpt->Set<double>("intlumi",0.0); }
		KBaseData(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KBase(name_, localOpt_, globalOpt_) { 
			if(!localOpt->Has("intlumi")) localOpt->Set<double>("intlumi",0.0);
		}
		//destructor
		virtual ~KBaseData() {}

		//functions for histo creation
		virtual void Build(); //implemented in KBuilder.h to avoid circular dependency
};

//--------------------------------------------------------------------------------
//extension of base class for MC - has error band calc, default cross section & norm type
class KBaseMC : public KBase {
	public:
		//constructors
		KBaseMC() : KBase() { 
			localOpt->Set<string>("normtype","MC");
			localOpt->Set<double>("xsection",0.0);
		}
		KBaseMC(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KBase(name_, localOpt_, globalOpt_) {
			if(!localOpt->Has("normtype")) localOpt->Set<string>("normtype","MC");
			if(!localOpt->Has("xsection")) localOpt->Set<double>("xsection",0.0); 
		}
		//destructor
		virtual ~KBaseMC() {}
		
		//functions for histo creation
		virtual void Build(); //implemented in KBuilder.h to avoid circular dependency

};

//-------------------------------------------
//extension of base class for external histos
class KBaseExt : public KBase {
	public:
		//constructors
		KBaseExt() : KBase(), add_ext(false) {}
		KBaseExt(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KBase(name_, localOpt_, globalOpt_), add_ext(false) {
			string filename;
			if(localOpt->Get("filename",filename)){
				//open file
				file = TFile::Open(filename.c_str());
				//check for specific histos to import
				vector<string> exthisto_in;
				vector<string> exthisto_out;
				localOpt->Get("exthisto_in",exthisto_in);
				localOpt->Get("exthisto_out",exthisto_out);
				if(exthisto_in.size() != exthisto_out.size()){
					cout << "Input error: vectors of external histo input and output names must have the same length. These external histos will not be used." << endl;
					return;
				}
				else{
					add_ext = true;
					for(int i = 0; i < exthisto_in.size(); i++){
						TH1* htmp = (TH1*)file->Get(exthisto_in[i].c_str());
						AddHisto(exthisto_out[i],htmp);
					}
					add_ext = false;
				}
			}
			
		}
		
		//change histo add mode
		void SetAddExt(bool ae) { add_ext = ae; }
		//histo add so external histos won't get overwritten
		TH1* AddHisto(string s, TH1* h){
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
		virtual void Build() {}
		
	private:
		//member variables
		bool add_ext;
};

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

#endif

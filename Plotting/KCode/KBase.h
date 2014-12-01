#ifndef KBASE_H
#define KBASE_H

//custom headers
#include "KMap.h"

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
		KBase() : name(""), filename(""),parent(0),option(0),file(0),tree(0),nEventProc(0),MyBuilder(0),stmp(""),htmp(0),etmp(0),isBuilt(false),doSubtract(false),intlumi(-1),normType("") {
			//enable histo errors
			TH1::SetDefaultSumw2(kTRUE);
			//must always have an option map
			if(option==0) option = new OptionMap();
		}
		KBase(string name_, string filename_, OptionMap* option_) : name(name_), filename(filename_), tree(0), parent(0), option(option_), MyBuilder(0), stmp(""), 
																	htmp(0), etmp(0), isBuilt(false), doSubtract(false), intlumi(-1), normType("") {
			//enable histo errors
			TH1::SetDefaultSumw2(kTRUE);
			
			if(filename.size()>0){
				//open file
				file = TFile::Open(filename.c_str());
				//get tree
				tree = (TTree*)file->Get("tree");
				//store this value (number of events processed) at the beginning so histo only has to be accessed once
				TH1F* nEventHist = (TH1F*)file->Get("nEventProc");
				if(nEventHist) nEventProc = nEventHist->GetBinContent(1);
				else nEventProc = 1; //default value;
			}
			
			//must always have an option map
			if(option==0) option = new OptionMap();
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
		string GetFileName() { return filename; }
		void SetName(string n) { name = n; }
		//add a blank histo for future building
		virtual TH1F* AddHisto(string s, TH1F* h){
			//sets current name and histo
			stmp = s;
			htmp = (TH1F*)h->Clone();
			htmp->Sumw2();
			MyHistos.Add(stmp,htmp);
			return htmp;
		}
		//gets current histo
		virtual TH1F* GetHisto(){ return htmp; }
		//gets current histo name
		virtual string GetHistoName() { return stmp; }
		//resets current name and histo
		virtual TH1F* GetHisto(string hname) {
			TH1F* hist = MyHistos.Get(hname);
			etmp = MyErrorBands.Get(hname); //it's okay for etmp to be null
			if(hist) {
				stmp = hname;
				htmp = hist;
				return htmp;
			}
			else return NULL; //do not reset if the histo does not exist
		}
		virtual map<string,TH1F*>& GetTable() { return MyHistos.GetTable(); }
		KBase* GetParent() { return parent; }
		void SetParent(KBase* p) {
			parent = p;
			//cout << name << " set parent " << parent->GetName() << endl;
		}
		virtual bool GetSubtract() { return doSubtract; }
		virtual void SetSubtract(bool s) { doSubtract = s; }
		OptionMap* GetOption() { return option; }
		void SetOption(OptionMap* opt) { option = opt; if(option==0) option = new OptionMap(); } //must always have an option map
		virtual void PrintYield() {
			double err = 0;
			double hint = htmp->IntegralAndError(0,htmp->GetNbinsX()+1,err);
			cout << name << ": " << hint << " +/- " << err << endl;
		}
		virtual int GetNEventProc(){ return nEventProc; }
		virtual void SetNEventProc(int n){ nEventProc = n; }
		virtual double GetYield() { return htmp->Integral(0,htmp->GetNbinsX()+1); }
		//norm type is an additional property, not included in constructor
		//it should be set separately if it needs to be changed
		string GetNormType() { return normType; }
		void SetNormType(string nt) { normType = nt; }
		virtual void CloseFile() { file->Close(); }
		//divide current histo by bin width, default implementation
		virtual void BinDivide(){
			for(int b = 1; b <= htmp->GetNbinsX(); b++){
				htmp->SetBinContent(b,htmp->GetBinContent(b)/htmp->GetBinWidth(b));
				htmp->SetBinError(b,htmp->GetBinError(b)/htmp->GetBinWidth(b));
			}
		}

		//other virtual functions (unimplemented at this level)
		virtual void Draw(TPad* pad) {}
		virtual int GetLegendInfo() {}
		virtual void AddToLegend(TLegend* leg) {}
		virtual void AddChild(KBase* ch) {}
		virtual void Normalize(double nn, bool toYield=true) {}
		//these default values allow to distinguish between data and MC...
		virtual double GetIntLumi() { return intlumi; }
		virtual double GetCrossSection() { return -1; }
		virtual Color_t GetColor() { }
		virtual void SetAddExt(bool ae) { }
		
	protected:
		//member variables
		string name, filename;
		KBase* parent;
		OptionMap* option;
		TFile* file;
		TTree* tree;
		int nEventProc;
		KBuilder* MyBuilder;
		HistoMap MyHistos;
		ErrorMap MyErrorBands;
		string stmp;
		TH1F* htmp;
		TGraphAsymmErrors* etmp;
		bool isBuilt;
		bool doSubtract;
		double intlumi;
		string normType;
};

//---------------------------------------------------------------
//extension of base class for data - has intlumi
class KBaseData : public KBase {
	public:
		//constructors
		KBaseData() : KBase() {}
		KBaseData(string name_, string filename_, OptionMap* option_, double intlumi_=0) : KBase(name_, filename_, option_) { intlumi = intlumi_; }
		//destructor
		virtual ~KBaseData() {}

		//functions for histo creation
		virtual void Build(); //implemented in KBuilder.h to avoid circular dependency
		
		//accessors
		double GetIntLumi() { return intlumi; }
		void SetIntLumi(double L) { intlumi = L; }
		
	protected:
		//member variables
		
};

//--------------------------------------------------------------------------------
//extension of base class for MC - has cross section, norm type, error band calc
class KBaseMC : public KBase {
	public:
		//constructors
		KBaseMC() : KBase() { normType = "MC"; }
		KBaseMC(string name_, string filename_, OptionMap* option_, double xsection_=0) : 
		KBase(name_, filename_, option_), xsection(xsection_), initWeight(false) { normType = "MC"; }
		//destructor
		virtual ~KBaseMC() {}
		
		//functions for histo creation
		virtual void Build(); //implemented in KBuilder.h to avoid circular dependency
		//in case of normalization to yield or other scaling
		virtual void Normalize(double nn, bool toYield=true){
			double simyield = htmp->Integral(0,htmp->GetNbinsX()+1);
			if(toYield) htmp->Scale(nn/simyield);
			else htmp->Scale(nn);
		}
		
		//accessors
		double GetCrossSection() { return xsection; }
		void SetCrossSection(double c) { xsection = c; }
		
	protected:
		//member variables
		double xsection;
		bool initWeight;

};

//-------------------------------------------
//extension of base class for external histos
class KBaseExt : public KBase {
	public:
		//constructors
		KBaseExt() : KBase(), add_ext(false) {}
		KBaseExt(string name_, OptionMap* option_) : KBase(name_,"",option_), add_ext(false) {}
		
		//change histo add mode
		void SetAddExt(bool ae) { add_ext = ae; }
		//histo add so external histos won't get overwritten
		TH1F* AddHisto(string s, TH1F* h){
			//set current name
			stmp = s;
			
			if(!add_ext){ //if the histo being added is not from ext, check to see if it is already added
				TH1F* hist = MyHistos.Get(s);
				if(hist){ //if it is already added, just use it, do not overwrite it
					htmp = hist;
					return hist;
				}
			}
			
			//otherwise, set current histo the usual way
			htmp = (TH1F*)h->Clone();
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
		
			TH1F* h1 = 0;
			TH1F* h2 = 0;
			
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

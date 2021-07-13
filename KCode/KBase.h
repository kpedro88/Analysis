#ifndef KBASE_H
#define KBASE_H

//custom headers
#include "KMap.h"
#include "KFit.h"
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

//contains various linked objects used for sets
class KObject {
	public:
		TH1* htmp = nullptr;
		KHisto* khtmp = nullptr;
		THStack* shtmp = nullptr;
		TGraphAsymmErrors* etmp = nullptr;
		TGraphAsymmErrors* btmp = nullptr;
		vector<double>* efftmp = nullptr;
		vector<KFit*> ftmp;
		vector<TH1*> rtmp;
};
typedef KMap<KObject*> ObjectMap;

//------------------------------------------------------------------------------------------------------------------
//base class: virtual methods and info for bases, other base/set classes inherit from it
class KBase {
	public:
		//constructors
		KBase() :
			name(""), parent(0), localOpt(new OptionMap()), globalOpt(new OptionMap()), 
			file(0), tree(0), nEventHist(0), nEventNegHist(0), MyCutflow(0),
			MyLooper(0), MySelection(0), stmp(""), obj(0), isBuilt(false), MyStyle(0)
		{
			//enable histo errors
			TH1::SetDefaultSumw2(kTRUE);
		}
		KBase(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : 
			name(name_), parent(0), localOpt(localOpt_ ? localOpt_ : new OptionMap()), globalOpt(globalOpt_ ? globalOpt_ : new OptionMap()), 
			file(0), tree(0), nEventHist(0), nEventNegHist(0), MyCutflow(0), 
			MyLooper(0), MySelection(0), stmp(""), obj(0), isBuilt(false), MyStyle(0)
		{
			debugroc = globalOpt->Get("debugroc",false);
			poisson = localOpt->Get("poisson",false);
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
			if(not localOpt->Get("nEventProc",nEventProc) and nEventHist) {
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
			TGraphAsymmErrors* esim = new TGraphAsymmErrors(obj->htmp->GetNbinsX()+2); //under- and overflow
			for(int b = 0; b < esim->GetN(); b++){
				if(obj->htmp->GetBinContent(b)>0){
					esim->SetPoint(b,obj->htmp->GetBinCenter(b),obj->htmp->GetBinContent(b)); //set to y value of sim
					double width = obj->htmp->GetBinWidth(b);
					esim->SetPointEXlow(b, width/2.);
					esim->SetPointEXhigh(b, width/2.);
					esim->SetPointEYlow(b, obj->htmp->GetBinError(b));
					esim->SetPointEYhigh(b, obj->htmp->GetBinError(b));
				}
			}
			esim->SetFillColor(kGray+1);
			//esim->SetFillStyle(3013);
			//esim->SetFillStyle(3001);
			esim->SetFillStyle(3005);

			obj->etmp = esim;
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
		//sets up fits for current histo (should only be called on top-level sets)
		virtual void AddFits(string s, OptionMap* omap, OptionMapMap& fitopts, string fitopt="fits") {
			//histo fit list overrides global fit list
			vector<string> fits; globalOpt->Get(fitopt,fits); omap->Get(fitopt,fits);
			if(fits.empty()) return;
			//cases: 1) no localfits specified, just use fits from histo, 2) empty localfits specified -> don't fit, 3) subset specified, fit just that subset
			//use intersection of histo fits and localfits because localfits may not be strict subset of histo fits
			//different histos can have different fits, each set only provides one list of localfits
			vector<string> localfits;
			if(localOpt->Get(fitopt,localfits) and localfits.empty()) return;
			set<string> localfitset(localfits.begin(),localfits.end());

			if(stmp==s or GetHisto(s)){
				for(const auto& fit: fits){
					if(!localfitset.empty() and localfitset.find(fit)==localfitset.end()) continue;
					auto fopt = fitopts.Get(fit);
					if(!fopt) throw runtime_error("No options provided for fit "+fit+" for histo "+s);
					auto kfit = new KFit(fit+"_"+s+"_"+name,fopt);
					//copy set style, then override w/ fit options
					kfit->SetStyle(*MyStyle);
					//set range to this histo
					kfit->SetRange(obj->htmp->GetXaxis()->GetXmin(),obj->htmp->GetXaxis()->GetXmax());
					obj->ftmp.push_back(kfit);
				}
			}
		}
		//perform any fits to current histo
		virtual void DoFits() {
			for(const auto& fit : obj->ftmp){
				if(obj->htmp->GetDimension()==1){
					if(obj->btmp) fit->DoFit(obj->btmp);
					else fit->DoFit(obj->htmp);
				}
			}
		}
		//gets current histo
		virtual TH1* GetHisto(){ return obj->htmp; }
		//gets current histo name
		virtual string GetHistoName() { return stmp; }
		//gets fits associated w/ current histo
		virtual vector<KFit*> GetFits() { return obj->ftmp; }
		//resets current name and histo
		virtual TH1* GetHisto(string hname) {
			KObject* otmp = MyObjects.Get(hname);
			if(otmp) {
				stmp = hname;
				obj = otmp;
				return obj->htmp;
			}
			else return nullptr; //do not reset if the histo does not exist
		}
		virtual void SaveHisto(string pname, TFile* file){
			file->cd();
			string outname = name;
			localOpt->Get("outname",outname);
			string oname = pname + "_" + outname;
			if(!obj->rtmp.empty()){
				//to save residuals from ratio
				for(auto res : obj->rtmp){
					//name is already assigned as res_[fit]_[hist]_[set]
					res->Write();
				}
			}
			else if(obj->btmp){
				obj->btmp->SetName(oname.c_str());
				obj->btmp->Write();
			}
			else {
				obj->htmp->SetName(oname.c_str());
				obj->htmp->Write();
			}
			if(globalOpt->Get("savefits",false)){
				for(auto fit : obj->ftmp){
					//fit name is already assigned as [fit]_[hist]_[set]
					fit->GetFn()->Write();
					auto g = fit->GetBand();
					if(g) g->Write();
				}
			}
		}
		virtual KHisto* GetKHisto(string hname){
			if(GetHisto(hname)) return obj->khtmp;
			else return nullptr;
		}
		virtual KHisto* GetKHisto(){ return obj->khtmp; }
		//returns efficiency for cut on current histo qty
		//does calculation and stores result if necessary
		virtual vector<double>* GetEff(){
			if(obj->efftmp) return obj->efftmp;
			
			//calculate efficiencies
			//normal: yield(i,nbins)/yield(0,nbins)
			obj->efftmp = new vector<double>(obj->htmp->GetNbinsX()+1);
			double ydenom = obj->htmp->Integral(0,obj->htmp->GetNbinsX()+1);
			if(debugroc) cout << name << " " << stmp << ":";
			for(int b = 0; b < obj->htmp->GetNbinsX()+1; b++){
				obj->efftmp->at(b) = obj->htmp->Integral(b,obj->htmp->GetNbinsX()+1)/ydenom;
				if(debugroc) cout << " " << obj->efftmp->at(b);
			}
			if(debugroc) cout << endl;

			return obj->efftmp;
		}
		virtual map<string,KObject*>& GetTable() { return MyObjects.GetTable(); }
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
			double hint = obj->htmp->IntegralAndError(0,obj->htmp->GetNbinsX()+1,err);
			cout << name << ": " << hint << " +/- " << err << endl;
		}
		virtual double GetYield() { return obj->htmp->Integral(0,obj->htmp->GetNbinsX()+1); }
		TFile* GetFile() { return file; }
		virtual void CloseFile() { if(file) file->Close(); }
		//rebin current histo
		virtual void Rebin(int rebinx, int rebiny=-1){
			if(rebiny<0) rebiny = rebinx;
			if(rebinx==0) rebinx = 1;
			if(rebiny==0) rebiny = 1;
			if(obj->htmp->GetDimension()==2) ((TH2*)obj->htmp)->Rebin2D(rebinx,rebiny);
			else obj->htmp->Rebin(rebinx);
		}
		//divide current histo by bin width, default implementation
		virtual void BinDivide(bool do_x, bool do_y, TH1* htmp=nullptr){
			if(!htmp) htmp = obj->htmp;
			//in TH1 case, GetNbinsY() == 1
			for(int by = 1; by <= htmp->GetNbinsY(); ++by){
				for(int bx = 1; bx <= htmp->GetNbinsX(); bx++){
					//access bin contents/errors w/ global bin number, widths w/ axis-specific bin number
					int b = htmp->GetBin(bx,by);
					if(do_x){
						htmp->SetBinContent(b,htmp->GetBinContent(b)/htmp->GetXaxis()->GetBinWidth(bx));
						htmp->SetBinError(b,htmp->GetBinError(b)/htmp->GetXaxis()->GetBinWidth(bx));
					}
					if(do_y){
						htmp->SetBinContent(b,htmp->GetBinContent(b)/htmp->GetYaxis()->GetBinWidth(b));
						htmp->SetBinError(b,htmp->GetBinError(b)/htmp->GetYaxis()->GetBinWidth(b));
					}
				}
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
		virtual void SetStyle(string styleName="") {
			if(styleName.size()==0) return;
			
			//search the list of default styles
			MyStyle = KStyle::GetWithDefault(styleName,localOpt);
		}
		virtual void SetStyle(const KStyle* orig){
			MyStyle = new KStyle(*orig);
			MyStyle->SetLocalOpt(localOpt);
		}
		virtual KStyle* GetStyle() { return MyStyle; }
		virtual bool IsData() { return localOpt->Get("data",false); }
		virtual bool IsMC() { return !localOpt->Get("data",true); }
		virtual bool IsRatio() { return false; }
		virtual bool IsExt() { return false; }
		virtual void SetBuilt() { isBuilt = true; }
		virtual void SetPoisson(bool p) { poisson = p; }

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
		ObjectMap MyObjects;
		string stmp;
		KObject* obj;
		bool isBuilt;
		bool debugroc;
		bool poisson;
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
			debug = globalOpt->Get("debugext",false);
			bool ext_auto = localOpt->Get("ext_auto",false);
			string hsuff;
			if(localOpt->Get("exthisto_suff",hsuff)) ext_auto = true;
			else hsuff = name;
			string hextra;
			globalOpt->Get("exthisto_extra",hextra);
			localOpt->Get("exthisto_extra",hextra);
			//append extra suffix to default suffix
			if(!hextra.empty()) hsuff += "_"+hextra;
			string dextra;
			globalOpt->Get("exthisto_dir",dextra);
			localOpt->Get("exthisto_dir",dextra);
			TDirectory* dir = NULL;
			if(!dextra.empty()) dir = KGet<TDirectory>(file,dextra);

			//check for k-factor (for non-ext, applied in KMCWeightSelector)
			useKFactor = localOpt->Get("kfactor",kfactor);

			add_ext = true;
			if(ext_auto){
				TKey* key;
				TIter next(dir ? dir->GetListOfKeys() : file->GetListOfKeys());
				int counter = 0;
				while((key = (TKey*)next())) {
					string ntmp = key->GetName();
					//look for names in the format histo_suff(_extra)
					if(ntmp.size() > hsuff.size() and ntmp.compare(ntmp.size()-hsuff.size(),hsuff.size(),hsuff)==0){
						auto extmp = dir ? KGet<TH1,TDirectory>(dir,ntmp) : KGet<TH1>(file,ntmp);
						AddHisto(ntmp.substr(0,ntmp.size()-hsuff.size()-1),extmp);
						++counter;
					}
				}
				if (counter==0) cout << "Warning: ext_auto was specified for base ext " << name << ", but no histograms were found." << endl;
			}
			else {
				//check for specific histos to import
				vector<string> exthisto_in;
				vector<string> exthisto_out;
				globalOpt->Get("exthisto_in",exthisto_in);
				localOpt->Get("exthisto_in",exthisto_in);
				globalOpt->Get("exthisto_out",exthisto_out);
				localOpt->Get("exthisto_out",exthisto_out);
				if(exthisto_in.size() != exthisto_out.size()){
					throw runtime_error("vectors of external histo input and output names have different lengths "+to_string(exthisto_in.size())+", "+to_string(exthisto_out.size())+" (in "+name+")");
				}
				for(unsigned i = 0; i < exthisto_in.size(); i++){
					auto extmp = dir ? KGet<TH1,TDirectory>(dir,exthisto_in[i]) : KGet<TH1>(file,exthisto_in[i]);
					AddHisto(exthisto_out[i],extmp);
				}
			}
			add_ext = false;
		}
		
		//change histo add mode
		void SetAddExt(bool ae) { add_ext = ae; }
		//histo add so external histos won't get overwritten, implemented in KHisto.h
		virtual TH1* AddHisto(string s, TH1* h, OptionMap* omap=NULL);
		
		//external histos do not need to build
		using KBase::Build;
		virtual void Build() {}

		virtual bool IsExt() { return true; }

	private:
		//member variables
		bool add_ext;
		bool debug;
		bool useKFactor;
		double kfactor;
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

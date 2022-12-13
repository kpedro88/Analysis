#ifndef KSET_H
#define KSET_H

//custom headers
#include "THN.h"
#include "KMap.h"
#include "KBase.h"
#include "KHisto.h"
#include "KLegend.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <THStack.h>

//STL headers
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

//------------------------------------------------------------------------------------------
//set class: has children and a parent (KBase or KBase-derived)
//           inherits from KBase to allow for sets of sets
//           default value for color
class KSet : public KBase {
	public:
		//constructor
		KSet() : KBase() { }
		KSet(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KBase(name_, localOpt_, globalOpt_) {
			debug = globalOpt->Get("debugset",false);
		}
		//destructor
		virtual ~KSet() {
			for(auto& child : children){
				delete child;
			}
		}
		
		virtual void AddChild(KBase* ch){
			children.push_back(ch);
			//cout << this->name << " added child " << children[children.size()-1]->GetName() << endl;
			ch->SetParent(this);
			if(poisson) ch->SetPoisson(true);
		}
		vector<KBase*>& GetChildren() { return children; }
		KBase* GetParent() { return parent; }
		void SetParent(KBase* p) { parent = p; }
		//add function - does formatting
		THN* AddHisto(string s, THN* h, OptionMap* omap=NULL){
			//does *not* make KHisto (not needed)
			if(h) KBase::AddHisto(s,h);
			
			//add to children
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->AddHisto(s,h,omap);
			}
			
			//*does* make KHisto (for special histo filling)
			if(!h) KBase::AddHisto(s,h,omap);
			
			//formatting			
			MyStyle->Format(obj->htmp);
			
			return obj->htmp;
		}
		//default build for sets
		using KBase::Build;
		virtual void Build(){
			//first, all children build
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->Build();
			}
			//then loop to add up histos (only resetting current histo for children once)
			if(debug) cout << "Set " << name << ":" << endl;
			for(auto& sit : MyObjects.GetTable()){
				GetHisto(sit.first); //this will propagate to children
				if(obj->khtmp and obj->khtmp->IsSpecial()) continue; //don't hadd special histos

				THN* htmp = nullptr;
				bool all_ext = true;
				if(debug) cout << "Histo " << sit.first << ":" << endl;
				//workaround for THnSparse::Add() bug
				if(children.size()==1){
					auto& child = children[0];
					htmp = child->GetHisto()->Clone();
					if(poisson) htmp->SetBinErrorOption(TH1::kPoisson);
					all_ext = child->IsExt();
					if(debug){
						cout << "child " << name << (child->IsExt() ? " (ext)" : " ") << ":" << endl;
						child->GetHisto()->Print();
					}
					if(child->GetLocalOpt()->Get("subtract",false)) htmp->Scale(-1);
				}
				else {
					for(auto& child: children){ //include option to subtract histos, off by default
						if(htmp==nullptr){
							htmp = child->GetHisto()->Clone();
							htmp->Reset("ICESM");
							if(poisson) htmp->SetBinErrorOption(TH1::kPoisson);
						}
						all_ext &= child->IsExt();
						if(debug){
							cout << "child " << name << (child->IsExt() ? " (ext)" : " ") << ":" << endl;
							child->GetHisto()->Print();
						}
						htmp->Add(child->GetHisto(), child->GetLocalOpt()->Get("subtract",false) ? -1 : 1);				
					}
				}

				//special case to allow external histos to have different binning
				if(all_ext){
					double diffmin = obj->htmp->GetXaxis()->GetXmin() - htmp->GetXaxis()->GetXmin();
					double diffmax = obj->htmp->GetXaxis()->GetXmax() - htmp->GetXaxis()->GetXmax();
					const double eps = 1e-10;
					if(abs(diffmin)<eps and abs(diffmax)<eps){
						//keep titles & style from this set
						htmp->GetXaxis()->SetTitle(obj->htmp->GetXaxis()->GetTitle());
						htmp->GetYaxis()->SetTitle(obj->htmp->GetYaxis()->GetTitle());
						htmp->GetZaxis()->SetTitle(obj->htmp->GetZaxis()->GetTitle());
						obj->htmp = htmp;
						MyStyle->Format(obj->htmp);
					}
					else {
						std::stringstream sdiff;
						sdiff << scientific << setprecision(10) << diffmin << "," << diffmax;
						throw runtime_error("Build error: inconsistent ranges in base histo ("+to_string(obj->htmp->GetXaxis()->GetXmin())+","+to_string(obj->htmp->GetXaxis()->GetXmax())+") and child histo ("+to_string(htmp->GetXaxis()->GetXmin())+","+to_string(htmp->GetXaxis()->GetXmax())+") [diff = "+sdiff.str()+"]");
					}
				}
				else {
					//continue workaround
					if(children.size()==1) obj->htmp = htmp;
					else obj->htmp->Add(htmp);
				}
				if(debug) {
					cout << "Result:" << endl;
					obj->htmp->Print();
				}

				//build error band, disabled by default
				if(localOpt->Get("errband",false)) {
					BuildErrorBand();
					//style
					MyStyle->FormatErr(obj->etmp);
				}
			}
		}
		virtual void MakeCutflows(){
			//all children get their cutflows
			//then add up raw cutflows & nevents from children
			TH1F* cutflowRaw = NULL;
			//todo: allow weighting
			for(unsigned c = 0; c < children.size(); c++){
				if(c==0){
					nEventHist = children[c]->GetNEventHist()->Clone();
					cutflowRaw = (TH1F*)(children[c]->GetCutflow(KCutflowType::CutRaw))->Clone();
				}
				else {
					nEventHist->Add(children[c]->GetNEventHist());
					cutflowRaw->Add(children[c]->GetCutflow(KCutflowType::CutRaw));
				}
			}
			//make derived (abs, rel) cutflows
			MyCutflow = new KCutflow(name,cutflowRaw,nEventHist);
		}
		//resetting current histo propagates to children for consistency
		using KBase::GetHisto;
		virtual THN* GetHisto(string hname){
			KObject* otmp = MyObjects.Get(hname);
			if(otmp and otmp->htmp) {
				stmp = hname;
				obj = otmp;
				for(unsigned c = 0; c < children.size(); c++){
					children[c]->GetHisto(hname); //ignore returned pointer
				}
				return obj->htmp;
			}
			else return nullptr; //do not reset if the histo does not exist
		}
		virtual void CloseFile(){
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->CloseFile();
			}
		}
		//rebin current histo, set implementation
		using KBase::Rebin;
		virtual void Rebin(int rebinx, int rebiny=-1){
			KBase::Rebin(rebinx, rebiny);

			//propagate to children for consistency
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->Rebin(rebinx, rebiny);
			}
		}
		//divide current histo by bin width, set implementation
		virtual void BinDivide(bool do_x, bool do_y, THN* htmp=nullptr){
			KBase::BinDivide(do_x,do_y);
		
			//scale children for consistency
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->BinDivide(do_x,do_y);
			}
		}
		//in case of normalization to yield or other scaling
		virtual void Normalize(double nn, bool toYield=true){
			if(obj->khtmp and obj->khtmp->IsSpecial()) return;
			//first, normalize all children
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->Normalize(nn,toYield);
			}
			//then do current histo
			KBase::Normalize(nn,toYield);
		}
		//adds histo to legend
		void AddToLegend(KLegend* kleg) {
			int panel_tmp = 0;
			localOpt->Get("panel",panel_tmp);
			vector<string> extra_text;
			localOpt->Get("extra_text",extra_text);
			string option = MyStyle->GetLegOpt();
			//only draw horizontal line if horizontal error bar is enabled
			//note: this setting only works in ROOT 5.34.11+
			if(IsData() and option.find("e")!=string::npos && (globalOpt->Get("horizerrbars",false) || obj->htmp->GetXaxis()->IsVariableBinSize())){
				option += "l";
			}
			kleg->AddEntry(obj->htmp ? (TObject*)obj->htmp->TH1() : (TObject*)obj->btmp,GetLegName(),option,panel_tmp,extra_text);
			//check if error band needs to be added
			if(localOpt->Get("errband",false) && localOpt->Get("errbandleg",true) and obj->etmp) {
				//this assumes it has already been created previously... a little unsafe, but a pain in the ass otherwise
				option = MyStyle->GetLegOptErr();
				kleg->AddEntry(obj->etmp,"Uncertainty",option,panel_tmp);
			}
			//check for fits
			for(auto fit : obj->ftmp){
				fit->AddToLegend(kleg,panel_tmp,true);
			}
		}

	protected:
		//member variables
		KBase* parent; //overloaded variable name
		vector<KBase*> children;
		bool debug;
};

//------------------------------
//specialization for data histos
class KSetData: public KSet {
	public:
		//constructor
		KSetData() : KSet() {}
		KSetData(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KSet(name_, localOpt_, globalOpt_) {}
		//destructor
		virtual ~KSetData() {}
		
		//sum up lumi when adding children
		void AddChild(KBase* ch){
			KSet::AddChild(ch);
			double intlumi, intlumi_ch;
			intlumi = intlumi_ch = 0;
			if(ch->GetLocalOpt()->Get("intlumi",intlumi_ch)) {
				localOpt->Get("intlumi",intlumi);
				localOpt->Set("intlumi",intlumi + intlumi_ch);
			}
		}
		//draw function
		void Draw(TPad* pad) {
			pad->cd();
			if(obj->htmp->GetDimension()==1) {
				for(auto fit : obj->ftmp){
					fit->Draw(pad);
				}
				obj->htmp->Draw(MyStyle->GetDrawOpt("same").c_str());
			}
			else if(obj->htmp->GetDimension()==2) obj->htmp->Draw("colz same");
		}
		using KBase::SetStyle;
		virtual void SetStyle(string styleName="") {
			KBase::SetStyle("data");
		}
		//accessors
		virtual bool IsData() { return true; }
		virtual bool IsMC() { return false; }
};
REGISTER_SET(KSetData,hist,data);

//------------------------------
//specialization for MC histos
class KSetMC: public KSet {
	public:
		//constructor
		KSetMC() : KSet() {}
		KSetMC(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KSet(name_, localOpt_, globalOpt_) {}
		//destructor
		virtual ~KSetMC() {}

		//draw function
		void Draw(TPad* pad) {
			pad->cd();
			if(obj->htmp->GetDimension()==1) {
				obj->htmp->Draw(MyStyle->GetDrawOpt("same").c_str());
				//disabled by default
				if(localOpt->Get("errband",false)) obj->etmp->Draw(MyStyle->GetDrawOptErr("same").c_str());
				for(auto fit : obj->ftmp){
					fit->Draw(pad);
				}
			}
			else if(obj->htmp->GetDimension()==2) obj->htmp->Draw("colz same");
		}
		using KBase::SetStyle;
		virtual void SetStyle(string styleName="") {
			//set some defaults first
			if(!localOpt->Has("fillcolor")) localOpt->Set("fillcolor",kWhite);
			KBase::SetStyle("hist");
		}
		//accessors
		virtual bool IsData() { return false; }
		virtual bool IsMC() { return true; }
		
};
REGISTER_SET(KSetMC,hist,mc);

//----------------------------
//specialization for MC stacks
class KSetMCStack : public KSet {
	public:
		//constructor
		KSetMCStack() : KSet() {}
		KSetMCStack(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KSet(name_, localOpt_, globalOpt_) {
			//check option
			savechildren = localOpt->Get("savechildren",true);
		}
		//destructor
		virtual ~KSetMCStack() {}

		virtual void AddChild(KBase* ch){
			KSet::AddChild(ch);
			//set formatting for stack
			Color_t color = kBlack;
			ch->GetLocalOpt()->Get("color",color);
			if(!ch->GetLocalOpt()->Has("fillcolor")) ch->GetLocalOpt()->Set("fillcolor",color);
			if(!ch->GetLocalOpt()->Has("linecolor")) ch->GetLocalOpt()->Set("linecolor",color);
			if(!ch->GetLocalOpt()->Has("markercolor")) ch->GetLocalOpt()->Set("markercolor",color);
			if(!ch->GetLocalOpt()->Has("linewidth")) ch->GetLocalOpt()->Set("linewidth",1);
			if(!ch->GetLocalOpt()->Has("legopt")) ch->GetLocalOpt()->Set<string>("legopt","f");
			//special formatting for stacked signal
			string sigstack = "";
			if(globalOpt->Get("sigstack",sigstack) && ch->GetName()==sigstack){
				if(!ch->GetLocalOpt()->Has("fillcolor")) ch->GetLocalOpt()->Set("fillcolor",kWhite);
				if(!ch->GetLocalOpt()->Has("linecolor")) ch->GetLocalOpt()->Set("linecolor",kBlack);
				if(!ch->GetLocalOpt()->Has("markercolor")) ch->GetLocalOpt()->Set("markercolor",kBlack);
				if(!ch->GetLocalOpt()->Has("linewidth")) ch->GetLocalOpt()->Set("linewidth",3);
				if(!ch->GetLocalOpt()->Has("legopt")) ch->GetLocalOpt()->Set<string>("legopt","l");
			}
		}

		//polymorphic add function for stacks (does formatting)
		virtual THN* AddHisto(string s, THN* h, OptionMap* omap=NULL){
			stmp = s;
			obj = new KObject();
			obj->shtmp = new THStack(stmp.c_str(),stmp.c_str());
			MyObjects.Add(stmp,obj);
			
			//add to children
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->AddHisto(s,h,omap);
				//formatting of children for stack already fixed
			}
			
			return nullptr; //no histo yet, pointless return value
		}
		
		//polymorphic build for stacks
		using KBase::Build;
		virtual void Build(){
			//first, all children build
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->Build();
			}
			
			//then loop to add up histos (only resetting current histo for children once)
			for(auto& sit : MyObjects.GetTable()){
				GetHisto(sit.first); //this will propagate to children
				
				//sort vector of children according to current histo - BEFORE adding to stack
				//unless disabled by user
				if(!globalOpt->Get("nosort",false)) sort(children.begin(),children.end(),KComp());
				
				string sigstack = "";
				bool do_sigstack = globalOpt->Get("sigstack",sigstack);
				int c_sigstack = -1;
				for(unsigned c = 0; c < children.size(); c++){
					if(do_sigstack && children[c]->GetName()==sigstack) c_sigstack = c; //do not add stacked signal yet, just store index
					else obj->shtmp->Add(children[c]->GetHisto()->TH1());
				}
				
				//fill in htmp now that shtmp is built
				obj->htmp = new THN1((TH1*)obj->shtmp->GetStack()->Last());
				//build error band, enabled by default for stack
				if(localOpt->Get("errband",true)) {
					BuildErrorBand();
					//style
					MyStyle->FormatErr(obj->etmp);
				}
				
				//add stacked signal histo after calculating error band
				if(c_sigstack > -1) obj->shtmp->Add(children[c_sigstack]->GetHisto()->TH1());
			}
		}
		//polymorphic GetHisto for stacks
		using KBase::GetHisto;
		virtual THN* GetHisto(string hname) {
			KObject* otmp = MyObjects.Get(hname);
			if(otmp and otmp->shtmp) {
				stmp = hname;
				obj = otmp;
				if(obj->shtmp->GetStack()) obj->htmp = new THN1((TH1*)obj->shtmp->GetStack()->Last());
				else obj->htmp = NULL; //might have a THStack but no GetStack during the histo building process
				for(unsigned c = 0; c < children.size(); c++){
					children[c]->GetHisto(hname); //ignore returned pointer
				}
				return obj->htmp;
			}
			else return nullptr; //do not reset if the histo does not exist
		}
		virtual void SaveHisto(string pname, TFile* file){
			if(savechildren){
				for(unsigned c = 0; c < children.size(); c++){
					children[c]->SaveHisto(pname,file);
				}				
			}
			else KBase::SaveHisto(pname,file);
		}
		//adds child histos to legend
		virtual void AddToLegend(KLegend* kleg) {
			//sort vector of children according to current histo - BEFORE adding to legend
			if(!globalOpt->Get("nosort",false)) sort(children.begin(),children.end(),KComp());	
		
			//add to legend in reverse order so largest is first
			for(int c = children.size()-1; c >= 0; c--){
				children[c]->AddToLegend(kleg);
			}
			//error band enabled by default
			int panel_tmp = 0;
			localOpt->Get("panel",panel_tmp);
			if(localOpt->Get("errband",true)) {
				string option = MyStyle->GetLegOptErr();
				kleg->AddEntry(obj->etmp,"Uncertainty",option,panel_tmp);
				//this assumes it has already been created previously... a little unsafe, but a pain in the ass otherwise
			}
			//check for fits
			for(auto fit : obj->ftmp){
				fit->AddToLegend(kleg,panel_tmp,true);
			}
		}
		//draw function
		virtual void Draw(TPad* pad) {
			pad->cd();
			if(obj->htmp->GetDimension()==1){
				obj->shtmp->Draw(MyStyle->GetDrawOpt("same").c_str());
				
				//error band enabled by default for stack
				if(localOpt->Get("errband",true)) obj->etmp->Draw(MyStyle->GetDrawOptErr("same").c_str());
				
				if(globalOpt->Get("bgline",false) and obj->htmp){
					THN* hoverlay = obj->htmp->Clone();
					MyStyle->Format(hoverlay);
					hoverlay->Draw(MyStyle->GetDrawOpt("same").c_str());
				}

				for(auto fit : obj->ftmp){
					fit->Draw(pad);
				}
			}
			else if(obj->htmp->GetDimension()==2){
				obj->htmp->Draw("colz same");
			}
		}
		//normalize all children histos
		virtual void Normalize(double nn, bool toYield=true){
			double simyield = obj->htmp->Integral(-1,-1); //yield of summed histo
			
			//scale stack histos
			TObjArray* stack_array = obj->shtmp->GetStack();
			for(int s = 0; s < stack_array->GetSize(); s++){
				TH1* hist = (TH1*)stack_array->At(s);
				if(hist){
					if(toYield) hist->Scale(nn/simyield);
					else hist->Scale(nn);
				}
			}
			
			//scale children (directly to overall yield)
			for(unsigned c = 0; c < children.size(); c++){
				if(toYield) children[c]->Normalize(nn/simyield,false);
				else children[c]->Normalize(nn,false);
			}
			
			//rebuild error band (enabled by default)
			if(globalOpt->Get("errband",true)) BuildErrorBand();
		}
		//rebin current histo, stack implementation
		virtual void Rebin(int rebinx, int rebiny=-1){
			//scale stack histos
			TObjArray* stack_array = obj->shtmp->GetStack();
			for(int s = 0; s < stack_array->GetSize(); s++){
				TH1* hist = (TH1*)stack_array->At(s);
				hist->Rebin(rebinx);
			}

			//scale children for consistency
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->Rebin(rebinx,rebiny);
			}

			//rebuild error band (enabled by default)
			if(globalOpt->Get("errband",true)) {
				BuildErrorBand();
				//style
				MyStyle->FormatErr(obj->etmp);
			}
		}
		//divide current histo by bin width, stack implementation
		virtual void BinDivide(bool do_x, bool do_y, THN* htmp=nullptr){
			//scale stack histos
			TObjArray* stack_array = obj->shtmp->GetStack();
			for(int s = 0; s < stack_array->GetSize(); s++){
				TH1* hist = (TH1*)stack_array->At(s);
				BinDivide(do_x,do_y,new THN1(hist));
			}
		
			//scale children for consistency
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->BinDivide(do_x,do_y);
			}
			
			//rebuild error band (enabled by default)
			if(globalOpt->Get("errband",true)) {
				BuildErrorBand();
				//style
				MyStyle->FormatErr(obj->etmp);
			}
		}
		//print yield from children
		virtual void PrintYield() { 
			KBase::PrintYield();
			//reverse order so largest will print first
			for(int c = children.size()-1; c >= 0; c--){
				cout << "    "; //indent children
				children[c]->PrintYield();
			}
		}

		using KBase::SetStyle;
		virtual void SetStyle(string styleName="") {
			KBase::SetStyle("stack");
		}
		//accessors
		virtual bool IsData() { return false; }
		virtual bool IsMC() { return true; }
		
	protected:
		//new stack-based member variables
		bool savechildren;
};
REGISTER_SET(KSetMCStack,stack,mc);

//-------------------------
//specialization for ratios
class KSetRatio: public KSet {
	public:
		//enums for different ratio calculations
		//DataMC = data/MC, TF = data/MC but keep both errors, PctDiff = (data - MC)/MC, Pull = (data - MC)/err, Q1 = S/sqrt(B), Q2=S/sqrt(S+B), Q3 = 2[sqrt(S+B) - sqrt(B)], Q4 = sqrt(2*[(S+B)*log(1+S/B)-S]), Binom = pass/all
		//numer = data, sig; denom = MC, bkg
		enum ratiocalc { DataMC=0, PctDiff=1, Pull=2, Q1=3, Q2=4, Q3=5, Q4=6, Binom=7, TF=8, Res=9, RelRes=10, PullFit=11 };
		//constructor
		KSetRatio() : KSet() {}
		KSetRatio(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KSet(name_, localOpt_, globalOpt_), calc1D(DataMC), noErrBand(false) {
			int nchildren(2); localOpt->Get("nchildren",nchildren);
			children.resize(nchildren);
			globalOpt->Get("ratiocalc",calcName1D);
			SetCalc(calcName1D,&calc1D);
			if(globalOpt->Get("ratiocalc2D",calcName2D)) SetCalc(calcName2D,&calc2D);
			else calc2D = calc1D;
			//auto disable ratio fits on residuals
			if(calc1D==Res or calc1D==RelRes or calc1D==PullFit) localOpt->Set<vector<string>>("ratiofits",{});
		}
		//destructor
		virtual ~KSetRatio() {}
		
		//first child is numerator, second child is denominator
		void AddNumerator(KBase* numer){ children[0] = numer; }
		void AddDenominator(KBase* denom){ children[1] = denom; }
		string GetRatioName2D() {
			if(calc2D==DataMC) return "[" + children[0]->GetLegName() + " / " + children[1]->GetLegName() + "]";
			else return "[" + children[0]->GetLegName() + " - " + children[1]->GetLegName() + "]/#sigma";
		}
		void SetCalc(const string& calcName, ratiocalc* calcVal=nullptr){
			if(!calcVal) calcVal = &calc1D;
			if(calcName=="DataMC") *calcVal = DataMC;
			else if(calcName=="TF") *calcVal = TF;
			else if(calcName=="PctDiff") *calcVal = PctDiff;
			else if(calcName=="Pull") *calcVal = Pull;
			else if(calcName=="Q1") *calcVal = Q1;
			else if(calcName=="Q2") *calcVal = Q2;
			else if(calcName=="Q3") *calcVal = Q3;
			else if(calcName=="Q4") *calcVal = Q4;
			else if(calcName=="Binom") *calcVal = Binom;
			else if(calcName=="Res") *calcVal = Res;
			else if(calcName=="RelRes") *calcVal = RelRes;
			else if(calcName=="PullFit") *calcVal = PullFit;
		}

		//ratios should not be rebinned or normalized
		virtual void Rebin(int rebin){}
		virtual void Normalize(double nn, bool toYield) {}
		//simplified generic build (for using ratio calc in pad1)
		using KBase::Build;
		virtual void Build(){
			int rebin = 0; globalOpt->Get("rebin",rebin);
			//first, all children build
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->Build();
			}
			//then loop over histos (only resetting current histo for children once, rebin if necessary)
			if(debug) cout << "Set " << name << ":" << endl;
			for(auto& sit : MyObjects.GetTable()){
				GetHisto(sit.first); //this will propagate to children
				for(unsigned c = 0; c < children.size(); c++){
					if(rebin) children[c]->Rebin(rebin);
				}
				Build(sit.first,children[0]->GetHisto(),children[1]->GetHisto());
			}
		}
		//ratio class acts a little differently:
		//only builds from the current histo of numer and denom
		//(since some histos might not want ratios, and also has to wait for possible norm to yield)
		void Build(OptionMap* omap, OptionMapMap& fitopts, THN* htemp=NULL){
			Build(children[0]->GetHistoName(),children[0]->GetHisto(),children[1]->GetHisto(),htemp);
			if(omap==NULL) return;
			AddFits(stmp,omap,fitopts,"ratiofits");
			DoFits();
		}
		//in case of standalone use
		void Build(string histoname, THN* h0, THN* h1, THN* htemp=NULL){
			stmp = histoname;
			obj = new KObject();

			THN* hdata = h0;
			THN* hsim = h1;
			THN* hsim0 = hsim->Clone();
			//in case displaying with different number of bins than original histo
			THN* hrat = htemp ? htemp->Clone() : h0->Clone();

			int nbins = hrat->GetNbinsX()+1;
			ratiocalc calc = calc1D;
			//comparing a set to itself: must be residuals from fit
			if(children[0]==children[1]){
				if(calc!=Res and calc!=RelRes and calc!=PullFit) calc = Res;
			}
			//only pull,data/MC supported for 2D
			//todo: add others
			if(hrat->GetDimension()==2) {
				calc = calc2D;
				if(calc!=DataMC and calc!=Pull) calc = Pull;
				nbins = ((TH2F*)hrat)->GetSize();
			}
			//only data/MC supported for TProfile
			//todo: add others
			if(hrat->InheritsFrom(TProfile::Class())) {
				calc = DataMC;
				hrat = new THN1(static_cast<TProfile*>(hrat->TH1())->ProjectionX());
				hdata = new THN1(static_cast<TProfile*>(hdata->TH1())->ProjectionX());
				hsim = new THN1(static_cast<TProfile*>(hsim->TH1())->ProjectionX());
				hsim0 = new THN1(static_cast<TProfile*>(hsim0->TH1())->ProjectionX());
			}
			//only data/MC supported for ratio of ratios
			if(children[0]->IsRatio() and children[1]->IsRatio()) calc = DataMC;

			if(calc==DataMC){ //data/mc
				auto b0 = ((KSetRatio*)children[0])->obj->btmp;
				auto b1 = ((KSetRatio*)children[1])->obj->btmp;
				//special case for ratio of ratios
				if(children[0]->IsRatio() and children[1]->IsRatio() and b0 and b1){
					auto y0 = b0->GetY();
					auto y1 = b1->GetY();
					auto yel0 = b0->GetEYlow();
					auto yel1 = b1->GetEYlow();
					auto yeh0 = b0->GetEYhigh();
					auto yeh1 = b1->GetEYhigh();
					int npts = b0->GetN();
					std::vector<double> yval, yel, yeh;
					yval.reserve(npts); yel.reserve(npts); yeh.reserve(npts);
					for(int b = 0; b < npts; ++b){
						yval.push_back(y0[b]/y1[b]);
						yel.push_back(yval.back()*sqrt(yel0[b]*yel0[b]/(y0[b]*y0[b])+yel1[b]*yel1[b]/(y1[b]*y1[b])));
						yeh.push_back(yval.back()*sqrt(yeh0[b]*yeh0[b]/(y0[b]*y0[b])+yeh1[b]*yeh1[b]/(y1[b]*y1[b])));
					}
					obj->btmp = new TGraphAsymmErrors(npts,b0->GetX(),yval.data(),b0->GetEXlow(),b0->GetEXhigh(),yel.data(),yeh.data());
				}
				else {
					//remove sim bin errors
					for(int b = 0; b < nbins; b++){
						hsim0->SetBinError(b,0);
					}
					hrat->Divide(hdata,hsim0);
	
					if(hrat->GetDimension()==2){
						for(int b = 0; b < nbins; b++){
							if(std::isnan(hrat->GetBinContent(b)) or hrat->GetBinError(b)<=0) hrat->SetBinContent(b,-1000); //hack so empty cells are not painted
						}
					}
				}
			}
			else if(calc==TF){ //data/mc but keep all errors, no band
				hrat->Divide(hdata,hsim0);
				noErrBand = true;
			}
			else if(calc==Binom){ //binomial error case
				obj->btmp = new TGraphAsymmErrors(hdata->TH1(),hsim->TH1());

				//remove x errors
				for(int b = 0; b < obj->btmp->GetN(); b++){
					obj->btmp->SetPointEXlow(b,0);
					obj->btmp->SetPointEXhigh(b,0);
				}
				
				//formatting
				if(MyStyle) MyStyle->Format(obj->btmp);
			}
			else if(calc==Pull){ //(data-mc)/err
				//todo: add option to allow removal of either data or mc errors
				hrat->Add(hdata,hsim,1,-1);
				for(int b = 0; b < nbins; b++){
					if(hrat->GetBinError(b)!=0) hrat->SetBinContent(b,hrat->GetBinContent(b)/hrat->GetBinError(b));
					if(hrat->GetDimension()==2 && hrat->GetBinError(b)<=0) hrat->SetBinContent(b,-1000); //hack so empty cells are not painted
					hrat->SetBinError(b,0);
				}
			}
			else if(calc==PctDiff){ //(data-mc)/mc
				//todo: add option to allow removal of either data or mc errors
				hrat->Add(hdata,hsim,1,-1);
				for(int b = 0; b < nbins; b++){
					//division and propagation of error
					if(hsim->GetBinContent(b)!=0){
						hrat->SetBinContent(b,hrat->GetBinContent(b)/hsim->GetBinContent(b));
						hrat->SetBinError(b,hrat->GetBinContent(b)*sqrt(pow(hdata->GetBinError(b)/hdata->GetBinContent(b),2) + pow(hsim->GetBinError(b)/hsim->GetBinContent(b),2)));
					}
					else {
						hrat->SetBinContent(b,0.);
						hrat->SetBinError(b,0.);
					}
				}
			}
			else if(calc==Q1){ //S/sqrt(B)
				for(int b = 0; b < nbins; b++){
					//sqrt & division
					if(hsim->GetBinContent(b)!=0) hrat->SetBinContent(b,hdata->GetBinContent(b)/sqrt(hsim->GetBinContent(b)));
					else hrat->SetBinContent(b,0.);
					//no errors
					hrat->SetBinError(b,0.);
				}
				noErrBand = true;
			}
			else if(calc==Q2){ //S/sqrt(S+B)
				for(int b = 0; b < nbins; b++){
					//sqrt & division
					if(hrat->GetBinContent(b)+hsim->GetBinContent(b)!=0) hrat->SetBinContent(b,hdata->GetBinContent(b)/sqrt(hrat->GetBinContent(b)+hsim->GetBinContent(b)));
					else hrat->SetBinContent(b,0.);
					//no errors
					hrat->SetBinError(b,0.);
				}
				noErrBand = true;
			}
			else if(calc==Q3){ //2[sqrt(S+B) - sqrt(B)]
				for(int b = 0; b < nbins; b++){
					//sqrt & diff
					hrat->SetBinContent(b,2*(sqrt(hdata->GetBinContent(b)+hsim->GetBinContent(b))-sqrt(hsim->GetBinContent(b))));
					//no errors
					hrat->SetBinError(b,0.);
				}
				noErrBand = true;
			}
			else if(calc==Q4){ //sqrt(2*[(S+B)*log(1+S/B)-S])
				for(int b = 0; b < nbins; b++){
					//undefined if B=0
					if(hsim->GetBinContent(b)==0.) hrat->SetBinContent(b,0.);
					else hrat->SetBinContent(b,sqrt(2*((hdata->GetBinContent(b)+hsim->GetBinContent(b))*log(1+hdata->GetBinContent(b)/hsim->GetBinContent(b))-hdata->GetBinContent(b))));
					//no errors
					hrat->SetBinError(b,0.);
				}
				noErrBand = true;
			}
			else if(calc==Res or calc==RelRes or calc==PullFit){ //data-fit, (data-fit)/fit, (data-fit)/err
				const auto& basefits = children[0]->GetFits();
				for(auto fit : basefits){
					THN* rtmp = hrat->Clone((calcName1D+"_"+fit->GetName()).c_str());
					//subtract function
					if(calc==Res or calc==RelRes){
						rtmp->Add(fit->GetFn(),-1);
						//divide by function if relative
						if(calc==RelRes) rtmp->Divide(fit->GetFn());
					}
					else if(calc==PullFit){ //divide by data error
						for(int b = 0; b < nbins; b++){
							//subtract by integral rather than center of bin
							rtmp->SetBinContent(b, rtmp->GetBinContent(b) - fit->GetFn()->Integral(rtmp->GetXaxis()->GetBinLowEdge(b), rtmp->GetXaxis()->GetBinLowEdge(b+1))/rtmp->GetXaxis()->GetBinWidth(b));
							//choice of up or down err follows:
							//  https://github.com/CMSDIJET/DijetRootTreeAnalyzer/blob/5735a7ff8819093e67ad189589dcb10e08b0520a/python/BinnedFit.py#L116-L209
							//  RooHist::makeResidHist()
							double err_tot_data = rtmp->GetBinContent(b) > 0 ? KMath::PoissonErrorLow(hrat->GetBinContent(b)) : KMath::PoissonErrorUp(hrat->GetBinContent(b));
							rtmp->SetBinContent(b, err_tot_data > 0 ? rtmp->GetBinContent(b)/err_tot_data : 0);
							//technically this should set up and down err separately
							rtmp->SetBinError(b, err_tot_data > 0 ? rtmp->GetBinError(b)/err_tot_data : 0);
						}
					}
					//formatting
					fit->GetStyle()->Format(rtmp);
					//store
					obj->rtmp.push_back(rtmp);
				}
				//signal case (no fit): (s+b)-b = s (data should be denom)
				if(basefits.empty() and calc==PullFit){
					THN* rtmp = hrat->Clone((calcName1D+"_"+children[0]->GetName()).c_str());
					for(int b = 0; b < nbins; b++){
						//s+b > b always, so use up err
						double err_tot_data = KMath::PoissonErrorUp(h1->GetBinContent(b));
						rtmp->SetBinContent(b, err_tot_data > 0 ? rtmp->GetBinContent(b)/err_tot_data : 0);
						rtmp->SetBinError(b, err_tot_data > 0 ? rtmp->GetBinError(b)/err_tot_data : 0);
					}
					children[0]->GetStyle()->Format(rtmp);
					obj->rtmp.push_back(rtmp);
					//different style
					localOpt->Set<string>("drawopt",children[0]->GetStyle()->GetDrawOpt());
					MyStyle->SetLocalOpt(localOpt);
				}
				else if(calc==PullFit){
					//also different style for data case
					localOpt->Set<string>("drawopt","hist");
					MyStyle->SetLocalOpt(localOpt);
				}
				noErrBand = true;
			}
			
			//formatting
			if(MyStyle) MyStyle->Format(hrat);

			obj->htmp = hrat;
			MyObjects.Add(stmp,obj);
			
			//error band enabled by default for 1D histo
			if(globalOpt->Get("errband",true) && obj->htmp->GetDimension()==1) {
				BuildErrorBand(h0,h1);
				//style
				if(MyStyle) MyStyle->FormatErr(obj->etmp);
			}
		}
		//calculate ratio error band from denom
		using KBase::BuildErrorBand;
		TGraphAsymmErrors* BuildErrorBand(THN* hdata, THN* hsim){
			//make sim error band
			TGraphAsymmErrors* erat = new TGraphAsymmErrors(obj->htmp->GetNbinsX()+2); //under- and overflow
			for(int b = 0; b < erat->GetN(); b++){
				if(obj->htmp->GetBinContent(b)>0){
					erat->SetPoint(b,obj->htmp->GetXaxis()->GetBinCenter(b),1); //hypothesis: numer/denom = 1
					double width = obj->htmp->GetXaxis()->GetBinWidth(b);
					erat->SetPointEXlow(b, width/2.);
					erat->SetPointEXhigh(b, width/2.);
					//y widths use error propagation for f = data/mc : sigma_f = sigma_mc*data/mc^2
					//(taking sigma_data = 0, since included in ratio point error bars)
					//currenty only defined for data/mc calc
					if(calc1D==DataMC){
						erat->SetPointEYlow(b, hsim->GetBinError(b)*hdata->GetBinContent(b)/(hsim->GetBinContent(b)*hsim->GetBinContent(b)));
						erat->SetPointEYhigh(b, hsim->GetBinError(b)*hdata->GetBinContent(b)/(hsim->GetBinContent(b)*hsim->GetBinContent(b)));
					}
					else {
						erat->SetPointEYlow(b, 0.);
						erat->SetPointEYhigh(b, 0.);
					}
				}
			}
			
			obj->etmp = erat;
			return erat;
		}

		//draw function
		void Draw(TPad* pad) {
			pad->cd();
			//error band enabled by default
			if(obj->htmp->GetDimension()==1) {
				if(!obj->rtmp.empty()){
					for(auto res : obj->rtmp){
						res->Draw(MyStyle->GetDrawOpt("same").c_str());
					}
				}
				else if(obj->btmp){
					obj->btmp->Draw("PZ same");
				}
				else {
					if(globalOpt->Get("errband",true) and !noErrBand) obj->etmp->Draw(MyStyle->GetDrawOptErr("same").c_str());
					obj->htmp->Draw(MyStyle->GetDrawOpt("same").c_str());
				}
				for(auto fit : obj->ftmp){
					fit->Draw(pad);
				}
			}
			else if(obj->htmp->GetDimension()==2){
				obj->htmp->Draw("colz same");
			}
		}
		using KBase::SetStyle;
		virtual void SetStyle(string styleName="") {
			KBase::SetStyle("data");
		}
		using KSet::AddToLegend;
		void AddToLegendRatio(KLegend* kleg) {
			if(!kleg) return;
			//for legend placement
			if(!obj->rtmp.empty()) {
				for(auto res : obj->rtmp){
					kleg->AddHist(res->TH1F());
				}
			}
			else if(obj->btmp) kleg->AddGraph(obj->btmp);
			else kleg->AddHist(obj->htmp->TH1F());
			//todo: add ratio w/ marker, error band?
			for(auto fit : obj->ftmp){
				//how to assign panel?
				fit->AddToLegend(kleg,0);
			}
		}
		virtual bool IsRatio() { return true; }

	protected:
		//new member variables
		string calcName1D, calcName2D;
		ratiocalc calc1D, calc2D;
		bool noErrBand;
};
REGISTER_SET(KSetRatio,ratio,mc);

#endif

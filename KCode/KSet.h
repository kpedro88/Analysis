#ifndef KSET_H
#define KSET_H

//custom headers
#include "KMap.h"
#include "KBase.h"
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
		KSet(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KBase(name_, localOpt_, globalOpt_) {	}
		//destructor
		virtual ~KSet() {}
		
		virtual void AddChild(KBase* ch){
			children.push_back(ch);
			//cout << this->name << " added child " << children[children.size()-1]->GetName() << endl;
			ch->SetParent(this);
		}
		vector<KBase*>& GetChildren() { return children; }
		KBase* GetParent() { return parent; }
		void SetParent(KBase* p) { parent = p; }
		//default build for sets
		using KBase::Build;
		virtual void Build(){
			//first, all children build
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->Build();
			}
			//then loop to add up histos (only resetting current histo for children once)
			HMit sit;
			for(sit = MyHistos.GetTable().begin(); sit != MyHistos.GetTable().end(); sit++){
				GetHisto(sit->first); //this will propagate to children
				for(unsigned c = 0; c < children.size(); c++){ //include option to subtract histos, off by default
					htmp->Add(children[c]->GetHisto(), children[c]->GetLocalOpt()->Get("subtract",false) ? -1 : 1);				
				}
			}
		}
		//resetting current histo propagates to children for consistency
		using KBase::GetHisto;
		virtual TH1* GetHisto(string hname){
			TH1* hist = MyHistos.Get(hname);
			etmp = MyErrorBands.Get(hname); //it's okay for etmp to be null
			efftmp = MyEffs.Get(hname); //will be calculated later if needed
			if(hist) {
				stmp = hname;
				htmp = hist;
				for(unsigned c = 0; c < children.size(); c++){
					children[c]->GetHisto(hname); //ignore returned pointer
				}
				return htmp;
			}
			else return NULL; //do not reset if the histo does not exist
		}
		virtual void CloseFile(){
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->CloseFile();
			}
		}
		//divide current histo by bin width, set implementation
		virtual void BinDivide(){
			KBase::BinDivide();
		
			//scale children for consistency
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->BinDivide();
			}
		}
		//in case of normalization to yield or other scaling
		virtual void Normalize(double nn, bool toYield=true){
			//first, normalize all children
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->Normalize(nn,toYield);
			}
			//then do current histo
			KBase::Normalize(nn,toYield);
		}
		
	protected:
		//member variables
		KBase* parent; //overloaded variable name
		vector<KBase*> children;
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
		//add function - does formatting
		TH1* AddHisto(string s, TH1* h){
			KBase::AddHisto(s,h);
			
			//add to children
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->AddHisto(s,h);
			}
			
			//formatting
			MyStyle->Format(htmp);
			Color_t color = kBlack;
			localOpt->Get("color",color);
			//formatting
			htmp->SetLineColor(color);
			htmp->SetMarkerColor(color);
			htmp->SetFillColor(0);
			htmp->SetMarkerStyle(20);
			
			return htmp;
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
			if(option.find("e")!=string::npos && (globalOpt->Get("horizerrbars",false) || htmp->GetXaxis()->IsVariableBinSize())){
				option += "l";
			}
			kleg->AddEntry(htmp,name,option,panel_tmp,extra_text);
		}
		//draw function
		void Draw(TPad* pad) {
			pad->cd();
			if(htmp->GetDimension()==1) htmp->Draw(MyStyle->GetDrawOpt("same").c_str());
			else if(htmp->GetDimension()==2) htmp->Draw("colz same");
		}
		virtual void SetStyle(KMap<string>& allStyles, string styleName="") {
			KBase::SetStyle(allStyles,"data");
		}
};

//------------------------------
//specialization for MC histos
class KSetMC: public KSet {
	public:
		//constructor
		KSetMC() : KSet() {}
		KSetMC(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KSet(name_, localOpt_, globalOpt_) {}
		//destructor
		virtual ~KSetMC() {}

		//build for MC sets
		using KBase::Build;
		virtual void Build(){
			//first, all children build
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->Build();
			}
			//then loop to add up histos (only resetting current histo for children once)
			HMit sit;
			for(sit = MyHistos.GetTable().begin(); sit != MyHistos.GetTable().end(); sit++){
				GetHisto(sit->first); //this will propagate to children
				for(unsigned c = 0; c < children.size(); c++){ //include option to subtract histos
					htmp->Add(children[c]->GetHisto(), children[c]->GetLocalOpt()->Get("subtract",false) ? -1 : 1);
				}
			}
			
			//build error band, disabled by default
			if(localOpt->Get("errband",false)) {
				BuildErrorBand();
				//style
				MyStyle->FormatErr(etmp);
			}
		}		
		//add function - does formatting
		TH1* AddHisto(string s, TH1* h){
			KBase::AddHisto(s,h);
			
			//add to children
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->AddHisto(s,h);
			}
			
			//formatting			
			MyStyle->Format(htmp);
			
			return htmp;
		}
		//adds histo to legend
		void AddToLegend(KLegend* kleg) {
			int panel_tmp = 0;
			localOpt->Get("panel",panel_tmp);
			vector<string> extra_text;
			localOpt->Get("extra_text",extra_text);
			string option = MyStyle->GetLegOpt();
			kleg->AddEntry(htmp,name,option,panel_tmp,extra_text);
			
			//check if error band needs to be added
			if(localOpt->Get("errband",false) && localOpt->Get("errbandleg",true)) {
				//this assumes it has already been created previously... a little unsafe, but a pain in the ass otherwise
				option = MyStyle->GetLegOptErr();
				kleg->AddEntry(etmp,"uncertainty",option,panel_tmp);
			}
		}
		//draw function
		void Draw(TPad* pad) {
			pad->cd();
			if(htmp->GetDimension()==1) {
				htmp->Draw(MyStyle->GetDrawOpt("same").c_str());
				//disabled by default
				if(localOpt->Get("errband",false)) etmp->Draw(MyStyle->GetDrawOptErr("same").c_str());
			}
			else if(htmp->GetDimension()==2) htmp->Draw("colz same");
		}
		virtual void SetStyle(KMap<string>& allStyles, string styleName="") {
			//set some defaults first
			if(!localOpt->Has("fillcolor")) localOpt->Set("fillcolor",kWhite);
			KBase::SetStyle(allStyles,"hist");
		}
		
};

//----------------------------
//specialization for MC stacks
class KSetMCStack : public KSet {
	public:
		//constructor
		KSetMCStack() : KSet() {}
		KSetMCStack(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KSet(name_, localOpt_, globalOpt_), shtmp(0) {}
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
		TH1* AddHisto(string s, TH1* h){
			stmp = s;
			shtmp = new THStack(stmp.c_str(),stmp.c_str());
			MyStacks.Add(stmp,shtmp);
			
			//add to children
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->AddHisto(s,h);
				//formatting of children for stack already fixed
			}
			
			return NULL; //no histo yet, pointless return value
		}
		
		//polymorphic build for stacks
		using KBase::Build;
		void Build(){
			//first, all children build
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->Build();
			}
			
			//then loop to add up histos (only resetting current histo for children once)
			SMit sit;
			for(sit = MyStacks.GetTable().begin(); sit != MyStacks.GetTable().end(); sit++){
				GetHisto(sit->first); //this will propagate to children
				
				//sort vector of children according to current histo - BEFORE adding to stack
				//unless disabled by user
				if(!globalOpt->Get("nosort",false)) sort(children.begin(),children.end(),KComp());
				
				string sigstack = "";
				bool do_sigstack = globalOpt->Get("sigstack",sigstack);
				int c_sigstack = -1;
				for(unsigned c = 0; c < children.size(); c++){
					if(do_sigstack && children[c]->GetName()==sigstack) c_sigstack = c; //do not add stacked signal yet, just store index
					else shtmp->Add(children[c]->GetHisto());
				}
				
				//fill in htmp now that shtmp is built
				htmp = (TH1*)shtmp->GetStack()->Last();
				//build error band, enabled by default for stack
				if(localOpt->Get("errband",true)) {
					BuildErrorBand();
					//style
					MyStyle->FormatErr(etmp);
				}
				
				//add stacked signal histo after calculating error band
				if(c_sigstack > -1) shtmp->Add(children[c_sigstack]->GetHisto());				
			}
		}
		//polymorphic GetHisto for stacks
		using KBase::GetHisto;
		TH1* GetHisto(string hname) {
			THStack* stk = MyStacks.Get(hname);
			etmp = MyErrorBands.Get(hname); //it's okay for etmp to be null
			efftmp = MyEffs.Get(hname); //will be calculated later if needed
			if(stk) {
				stmp = hname;
				shtmp = stk;
				if(shtmp->GetStack()) htmp = (TH1*)shtmp->GetStack()->Last();
				else htmp = NULL; //might have a THStack but no GetStack during the histo building process
				for(unsigned c = 0; c < children.size(); c++){
					children[c]->GetHisto(hname); //ignore returned pointer
				}
				return htmp;
			}
			else return NULL; //do not reset if the histo does not exist
		}
		//adds child histos to legend
		void AddToLegend(KLegend* kleg) {
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
				kleg->AddEntry(etmp,"uncertainty",option,panel_tmp);
			}
			//this assumes it has already been created previously... a little unsafe, but a pain in the ass otherwise
		}
		//draw function
		void Draw(TPad* pad) {
			pad->cd();
			if(htmp->GetDimension()==1){
				shtmp->Draw(MyStyle->GetDrawOpt("same").c_str());
				
				//error band enabled by default for stack
				if(localOpt->Get("errband",true)) etmp->Draw(MyStyle->GetDrawOptErr("same").c_str());
				
				if(globalOpt->Get("bgline",false)&&htmp){
					TH1* hoverlay = (TH1*)htmp->Clone();
					MyStyle->Format(hoverlay);
					hoverlay->Draw(MyStyle->GetDrawOpt("same").c_str());
				}
			}
			else if(htmp->GetDimension()==2){
				htmp->Draw("colz same");
			}
		}
		//normalize all children histos
		void Normalize(double nn, bool toYield=true){
			double simyield = htmp->Integral(0,htmp->GetNbinsX()+1); //yield of summed histo
			
			//scale stack histos
			TObjArray* stack_array = shtmp->GetStack();
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
		//divide current histo by bin width, stack implementation
		void BinDivide(){
			//scale stack histos
			TObjArray* stack_array = shtmp->GetStack();
			for(int s = 0; s < stack_array->GetSize(); s++){
				TH1* hist = (TH1*)stack_array->At(s);
				for(int b = 1; b <= htmp->GetNbinsX(); b++){
					hist->SetBinContent(b,hist->GetBinContent(b)/hist->GetBinWidth(b));
					hist->SetBinError(b,hist->GetBinError(b)/hist->GetBinWidth(b));
				}
			}
		
			//scale children for consistency
			for(unsigned c = 0; c < children.size(); c++){
				children[c]->BinDivide();
			}
			
			//rebuild error band (enabled by default)
			if(globalOpt->Get("errband",true)) {
				BuildErrorBand();
				//style
				MyStyle->FormatErr(etmp);
			}
		}
		//print yield from children
		void PrintYield() { 
			KBase::PrintYield();
			//reverse order so largest will print first
			for(int c = children.size()-1; c >= 0; c--){
				cout << "    "; //indent children
				children[c]->PrintYield();
			}
		}
		//check special status for children also
		KBase* CheckSpecial(string special){
			if(name==special) return (KBase*)this;
			for(unsigned c = 0; c < children.size(); c++){
				if(children[c]->GetName()==special) return children[c];
			}
			return NULL;
		}
		virtual void SetStyle(KMap<string>& allStyles, string styleName="") {
			KBase::SetStyle(allStyles,"stack");
		}
		
	protected:
		//new stack-based member variables
		THStack* shtmp;
		StackMap MyStacks;
};

//-------------------------
//specialization for ratios
class KSetRatio: public KSet {
	public:
		//enums for different ratio calculations
		//DataMC = data/MC, PctDiff = (data - MC)/MC, Pull = (data - MC)/err, Q1 = S/sqrt(B), Q2=S/sqrt(S+B), Q3 = 2[sqrt(S+B) - sqrt(B)], Binom = pass/all
		//numer = data, sig; denom = MC, bkg
		enum ratiocalc { DataMC=0, PctDiff=1, Pull=2, Q1=3, Q2=4, Q3=5, Binom=6 };
		//constructor
		KSetRatio() : KSet() {}
		KSetRatio(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KSet(name_, localOpt_, globalOpt_), btmp(0), calc(DataMC) { 
			children.resize(2);
			string calcName = "";
			setCalc = globalOpt->Get("ratiocalc",calcName);
			
			if(calcName=="DataMC") calc = DataMC;
			else if(calcName=="PctDiff") calc = PctDiff;
			else if(calcName=="Pull") calc = Pull;
			else if(calcName=="Q1") calc = Q1;
			else if(calcName=="Q2") calc = Q2;
			else if(calcName=="Q3") calc = Q3;
			else if(calcName=="Binom") calc = Binom;
		}
		//destructor
		virtual ~KSetRatio() {}
		
		//first child is numerator, second child is denominator
		void AddNumerator(KBase* numer){ children[0] = numer; }
		void AddDenominator(KBase* denom){ children[1] = denom; }
		
		//ratio class acts a little differently:
		//only builds from the current histo of numer and denom
		//(since some histos might not want ratios, and also has to wait for possible norm to yield)
		using KBase::Build;
		void Build(TH1* hrat_){
			stmp = children[0]->GetHistoName();
			TH1* hrat = (TH1*)hrat_->Clone();
			TH1* hdata = children[0]->GetHisto();
			TH1* hsim = children[1]->GetHisto();
			TH1* hsim0 = (TH1*)hsim->Clone();
			
			int nbins = hrat->GetNbinsX()+1;
			//only pull supported for 2D
			//todo: add others
			if(hrat->GetDimension()==2) {
				calc = Pull;
				nbins = ((TH2F*)hrat)->GetSize();
			}
			//only data/MC supported for TProfile
			//todo: add others
			if(hrat->InheritsFrom(TProfile::Class())) {
				calc = DataMC;
				hrat = static_cast<TProfile*>(hrat)->ProjectionX();
				hdata = static_cast<TProfile*>(hdata)->ProjectionX();
				hsim = static_cast<TProfile*>(hsim)->ProjectionX();
				hsim0 = static_cast<TProfile*>(hsim0)->ProjectionX();
			}
			
			if(calc==DataMC){ //data/mc
				//remove sim bin errors
				for(int b = 0; b < nbins; b++){
					hsim0->SetBinError(b,0);
				}
				
				hrat->Divide(hdata,hsim0);				
			}
			else if(calc==Binom){ //binomial error case
				btmp = new TGraphAsymmErrors(hdata,hsim);
				MyBinoms.Add(stmp,btmp);

				//remove x errors
				for(int b = 0; b < btmp->GetN(); b++){
					btmp->SetPointEXlow(b,0);
					btmp->SetPointEXhigh(b,0);
				}
				
				//formatting
				MyStyle->Format(btmp);
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
			}
			else if(calc==Q2){ //S/sqrt(S+B)
				for(int b = 0; b < nbins; b++){
					//sqrt & division
					if(hrat->GetBinContent(b)+hsim->GetBinContent(b)!=0) hrat->SetBinContent(b,hdata->GetBinContent(b)/sqrt(hrat->GetBinContent(b)+hsim->GetBinContent(b)));
					else hrat->SetBinContent(b,0.);
					//no errors
					hrat->SetBinError(b,0.);
				}
			}
			else if(calc==Q3){ //2[sqrt(S+B) - sqrt(B)]
				for(int b = 0; b < nbins; b++){
					//sqrt & diff
					hrat->SetBinContent(b,2*(sqrt(hdata->GetBinContent(b)+hsim->GetBinContent(b))-sqrt(hsim->GetBinContent(b))));
					//no errors
					hrat->SetBinError(b,0.);
				}
			}
			
			//formatting
			MyStyle->Format(hrat);
			
			htmp = hrat;
			MyHistos.Add(stmp,htmp);
			
			//error band enabled by default for 1D histo
			if(globalOpt->Get("errband",true) && htmp->GetDimension()==1) {
				BuildErrorBand();
				//style
				MyStyle->FormatErr(etmp);
			}
		}
		//calculate ratio error band from denom
		TGraphAsymmErrors* BuildErrorBand(){
			TH1* hdata = children[0]->GetHisto();
			TH1* hsim = children[1]->GetHisto();
		
			//make sim error band
			TGraphAsymmErrors* erat = new TGraphAsymmErrors(htmp->GetNbinsX()+2); //under- and overflow
			for(int b = 0; b < erat->GetN(); b++){
				if(htmp->GetBinContent(b)>0){
					erat->SetPoint(b,htmp->GetBinCenter(b),1); //hypothesis: numer/denom = 1
					double width = htmp->GetBinWidth(b);
					erat->SetPointEXlow(b, width/2.);
					erat->SetPointEXhigh(b, width/2.);
					//y widths use error propagation for f = data/mc : sigma_f = sigma_mc*data/mc^2
					//(taking sigma_data = 0, since included in ratio point error bars)
					//currenty only defined for data/mc calc
					if(calc==DataMC){
						erat->SetPointEYlow(b, hsim->GetBinError(b)*hdata->GetBinContent(b)/(hsim->GetBinContent(b)*hsim->GetBinContent(b)));
						erat->SetPointEYhigh(b, hsim->GetBinError(b)*hdata->GetBinContent(b)/(hsim->GetBinContent(b)*hsim->GetBinContent(b)));
					}
					else {
						erat->SetPointEYlow(b, 0.);
						erat->SetPointEYhigh(b, 0.);
					}
				}
			}
			
			MyErrorBands.Add(stmp,erat);
			etmp = erat;
			return erat;
		}
		
		//draw function
		void Draw(TPad* pad) {
			pad->cd();
			//error band enabled by default
			if(htmp->GetDimension()==1) {
				if(btmp){
					btmp->Draw("PZ same");
					return;
				}
				
				if(globalOpt->Get("errband",true)) etmp->Draw(MyStyle->GetDrawOptErr("same").c_str());
				htmp->Draw(MyStyle->GetDrawOpt("same").c_str());
			}
			else if(htmp->GetDimension()==2){
				htmp->Draw("colz same");
			}
		}
		virtual void SetStyle(KMap<string>& allStyles, string styleName="") {
			KBase::SetStyle(allStyles,"data");
		}
		
		protected:
		//new member variables for binomial case
		TGraphAsymmErrors* btmp;
		ErrorMap MyBinoms;
		ratiocalc calc;
		bool setCalc;
		
};

//extension of namespace for processing bases/sets
namespace KParser {
	KBase* processBase(string line, OptionMap* globalOpt){
		//tab separated input
		vector<string> fields;
		process(line,'\t',fields);
		
		//check for necessary options
		if(fields.size()<3) {
			cout << "Input error: not enough fields in this line:" << endl << line << endl << "Check that all fields are tab-separated. This input will be ignored." << endl;
			return NULL;
		}
		
		//universal variables
		string type = fields[0];
		string subtype = fields[1];
		string name = fields[2];
		
		OptionMap* omap = new OptionMap(); //for local options
		//process local options before constructing objects
		for(unsigned i = 3; i < fields.size(); i++){
			processOption(fields[i],omap);
		}
		
		//create object
		KBase* tmp = 0;
		if(type=="base"){
			if(subtype=="ext") tmp = new KBaseExt(name,omap,globalOpt);
			else if(subtype=="data") tmp = new KBaseData(name,omap,globalOpt); 
			else if(subtype=="mc") tmp = new KBaseMC(name,omap,globalOpt);
			else if(subtype=="skim") tmp = new KBaseSkim(name,omap,globalOpt);
		}
		else {
			if(type=="hist" && subtype=="data") tmp = new KSetData(name,omap,globalOpt);
			else if(type=="hist" && subtype=="mc") tmp = new KSetMC(name,omap,globalOpt);
			else if(type=="stack" && subtype=="mc") tmp = new KSetMCStack(name,omap,globalOpt);
		}
		
		if(!tmp){
			cout << "Input error: set type \"" << type << " " << subtype << "\" is not recognized. This input will be ignored." << endl;
		}
		
		return tmp;
	}
}

#endif

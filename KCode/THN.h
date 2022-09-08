#ifndef THN_H
#define THN_H

#include <TROOT.h>
#include <TObject.h>
#include <TString.h>
#include <TH1.h>
#include <TH1F.h>
#include <THnSparse.h>
#include <TH2.h>
#include <TProfile.h>
#include <TF1.h>
#include <TFitResultPtr.h>
#include <TDirectory.h>

#include <string>
#include <vector>
#include <exception>
#include <iostream>
#define DEBUG_THN

using namespace std;

class THN {
	public:
		//to get the underlying pointer
		virtual ::TH1* TH1() { return nullptr; }
		virtual const ::TH1* TH1() const { return nullptr; }
		virtual ::TH1F* TH1F() { return nullptr; }
		virtual const ::TH1F* TH1F() const { return nullptr; }
		virtual ::THnSparse* THnSparse() { return nullptr; }
		virtual const ::THnSparse* THnSparse() const { return nullptr; }
		virtual string cl() const { return ""; }

		virtual Bool_t Add (const ::TH1 *h1, const ::TH1 *h2, Double_t c1=1, Double_t c2=1) { unimpl(__PRETTY_FUNCTION__); return false; }
		virtual Bool_t Add (const ::TH1 *h1, Double_t c1=1) { unimpl(__PRETTY_FUNCTION__); return false; }
		virtual Bool_t Add (TF1 *f1, Double_t c1=1, Option_t *option="") { unimpl(__PRETTY_FUNCTION__); return false; }
		virtual Bool_t Add (const THN *h1, const THN *h2, Double_t c1=1, Double_t c2=1) { unimpl(__PRETTY_FUNCTION__); return false; }
		virtual Bool_t Add (const THN *h1, Double_t c1=1) { unimpl(__PRETTY_FUNCTION__); return false; }
		virtual Bool_t Divide (const ::TH1 *h1) { unimpl(__PRETTY_FUNCTION__); return false; }
		virtual Bool_t Divide (const ::TH1 *h1, const ::TH1 *h2, Double_t c1=1, Double_t c2=1, Option_t *option="") { unimpl(__PRETTY_FUNCTION__); return false; }
		virtual Bool_t Divide (TF1 *f1, Double_t c1=1) { unimpl(__PRETTY_FUNCTION__); return false; }
		virtual Bool_t Divide (const THN *h1) { unimpl(__PRETTY_FUNCTION__); return false; }
		virtual Bool_t Divide (const THN *h1, const THN *h2, Double_t c1=1, Double_t c2=1, Option_t *option="") { unimpl(__PRETTY_FUNCTION__); return false; }
		virtual Bool_t Multiply (const ::TH1 *h1) { unimpl(__PRETTY_FUNCTION__); return false; }
		virtual Bool_t Multiply (const ::TH1 *h1, const ::TH1 *h2, Double_t c1=1, Double_t c2=1, Option_t *option="") { unimpl(__PRETTY_FUNCTION__); return false; }
		virtual Bool_t Multiply (TF1 *f1, Double_t c1=1) { unimpl(__PRETTY_FUNCTION__); return false; }
		virtual Bool_t Multiply (const THN *h1) { unimpl(__PRETTY_FUNCTION__); return false; }
		virtual Bool_t Multiply (const THN *h1, const THN *h2, Double_t c1=1, Double_t c2=1, Option_t *option="") { unimpl(__PRETTY_FUNCTION__); return false; }

		//changed return type
		virtual THN * Clone (const char *newname="") { unimpl(__PRETTY_FUNCTION__); return nullptr; }
		virtual void Draw (Option_t *option="") { unimpl(__PRETTY_FUNCTION__); }
		//use THnSparse-style interface, but with STL
		virtual Long64_t Fill(const vector<Double_t>& x, Double_t w=1.) { unimpl(__PRETTY_FUNCTION__); return 0; }
		virtual TFitResultPtr Fit (TF1 *f1, Option_t *option="", Option_t *goption="", Double_t xmin=0, Double_t xmax=0) { unimpl(__PRETTY_FUNCTION__); return nullptr; }
		//use larger input datatype from THnSparse
		virtual Long64_t FindBin (Double_t x, Double_t y=0, Double_t z=0) { unimpl(__PRETTY_FUNCTION__); return 0; }
		virtual Long64_t GetBin (Int_t binx, Int_t biny=0, Int_t binz=0) const { unimpl(__PRETTY_FUNCTION__); return 0; }
		virtual Double_t GetBinContent (Long64_t bin) const { unimpl(__PRETTY_FUNCTION__); return 0.; }
		virtual Double_t GetBinError (Long64_t bin) const { unimpl(__PRETTY_FUNCTION__); return 0.; }
		virtual ::TH1::EBinErrorOpt GetBinErrorOption () const { unimpl(__PRETTY_FUNCTION__); return ::TH1::kNormal; }
		virtual Int_t GetDimension () const { unimpl(__PRETTY_FUNCTION__); return 0; }
		virtual Int_t GetNbinsX () const { unimpl(__PRETTY_FUNCTION__); return 0; }
		virtual Int_t GetNbinsY () const { unimpl(__PRETTY_FUNCTION__); return 0; }
		virtual TAxis * GetAxis (Int_t dim) { unimpl(__PRETTY_FUNCTION__); return nullptr; }
		virtual const TAxis * GetAxis (Int_t dim) const { unimpl(__PRETTY_FUNCTION__); return nullptr; }
		virtual TAxis* GetXaxis() { unimpl(__PRETTY_FUNCTION__); return nullptr; }
		virtual const TAxis* GetXaxis() const { unimpl(__PRETTY_FUNCTION__); return nullptr; }
		virtual TAxis* GetYaxis() { unimpl(__PRETTY_FUNCTION__); return nullptr; }
		virtual const TAxis* GetYaxis() const { unimpl(__PRETTY_FUNCTION__); return nullptr; }
		virtual TAxis* GetZaxis() { unimpl(__PRETTY_FUNCTION__); return nullptr; }
		virtual const TAxis* GetZaxis() const { unimpl(__PRETTY_FUNCTION__); return nullptr; }
		virtual Bool_t InheritsFrom (const char *classname) const { unimpl(__PRETTY_FUNCTION__); return false; }
		virtual Bool_t InheritsFrom (const TClass *cl) const { unimpl(__PRETTY_FUNCTION__); return false; }
		virtual Double_t Integral (Int_t binx1, Int_t binx2, Option_t *option="") const { unimpl(__PRETTY_FUNCTION__); return 0.; }
		virtual Double_t Integral (Option_t *option="") const { unimpl(__PRETTY_FUNCTION__); return 0.; }
		virtual Double_t IntegralAndError (Int_t binx1, Int_t binx2, Double_t &err, Option_t *option="") const { unimpl(__PRETTY_FUNCTION__); return 0.; }
		virtual void Print (Option_t *option="") const { unimpl(__PRETTY_FUNCTION__); }
		//changed return type
		virtual THN * Rebin (Int_t ngroup=2, const char *newname="", const Double_t *xbins=0) { unimpl(__PRETTY_FUNCTION__); return nullptr; }
		virtual void Reset (Option_t *option="") { unimpl(__PRETTY_FUNCTION__); }
		virtual void Scale (Double_t c1=1, Option_t *option="") { unimpl(__PRETTY_FUNCTION__); }
		virtual void SetAxisRange (Double_t xmin, Double_t xmax, Option_t *axis="X") { unimpl(__PRETTY_FUNCTION__); }
		virtual void SetBinContent (Long64_t bin, Double_t content) { unimpl(__PRETTY_FUNCTION__); }
		virtual void SetBinError (Long64_t bin, Double_t error) { unimpl(__PRETTY_FUNCTION__); }
		virtual void SetBinErrorOption (::TH1::EBinErrorOpt type) { unimpl(__PRETTY_FUNCTION__); }
		virtual void SetDirectory (TDirectory *dir) { unimpl(__PRETTY_FUNCTION__); }
		virtual void SetName (const char *name) { unimpl(__PRETTY_FUNCTION__); }
		virtual void Sumw2 (Bool_t flag=kTRUE) { unimpl(__PRETTY_FUNCTION__); }
		virtual Int_t Write (const char *name=0, Int_t option=0, Int_t bufsize=0) { unimpl(__PRETTY_FUNCTION__); return 0; }
		virtual Int_t Write (const char *name=0, Int_t option=0, Int_t bufsize=0) const { unimpl(__PRETTY_FUNCTION__); return 0; }

		//style accessors
		virtual void SetFillColor (Color_t fcolor) { unimpl(__PRETTY_FUNCTION__); }
		virtual void SetFillStyle (Style_t fstyle) { unimpl(__PRETTY_FUNCTION__); }
		virtual void SetLineColor (Color_t lcolor) { unimpl(__PRETTY_FUNCTION__); }
		virtual void SetLineStyle (Style_t lstyle) { unimpl(__PRETTY_FUNCTION__); }
		virtual void SetLineWidth (Width_t lwidth) { unimpl(__PRETTY_FUNCTION__); }
		virtual void SetMarkerColor (Color_t mcolor=1) { unimpl(__PRETTY_FUNCTION__); }
		virtual void SetMarkerSize (Size_t msize=1) { unimpl(__PRETTY_FUNCTION__); }
		virtual void SetMarkerStyle (Style_t mstyle=1) { unimpl(__PRETTY_FUNCTION__); }

	protected:
		void unimpl(const string& fn) const { throw runtime_error(fn+" not implemented for "+cl()); }
};

//only complete specializations for certain classes implemented (below)
template <class T>
class THNT : public THN {};

template <>
class THNT<::TH1> : public THN {
	public:
		THNT<::TH1>(::TH1* h_) : h(h_), isTH2(h->InheritsFrom(TH2::Class())), isTProfile(h->InheritsFrom(TProfile::Class())) { }
		::TH1* TH1() override { return h; }
		const ::TH1* TH1() const override { return h; }
		::TH1F* TH1F() override { return (::TH1F*)h; }
		const ::TH1F* TH1F() const override { return (::TH1F*)h; }
		string cl() const override { return "TH1"; }

		Bool_t Add (const ::TH1 *h1, const ::TH1 *h2, Double_t c1=1, Double_t c2=1) override { return h->Add(h1,h2,c1,c2); }
		Bool_t Add (const ::TH1 *h1, Double_t c1=1) override { return h->Add(h1,c1); }
		Bool_t Add (TF1 *h1, Double_t c1=1, Option_t *option="") override { return h->Add(h1,c1,option); }
		Bool_t Add (const THN *h1, const THN *h2, Double_t c1=1, Double_t c2=1) override {
			if(!h1->TH1() or !h2->TH1()) return false;
			return h->Add(h1->TH1(),h2->TH1(),c1,c2);
		}
		Bool_t Add (const THN *h1, Double_t c1=1) override {
			if(!h1->TH1()) return false;
			return h->Add(h1->TH1(),c1);
		}
		Bool_t Divide (const ::TH1 *h1) override { return h->Divide(h1); }
		Bool_t Divide (const ::TH1 *h1, const ::TH1 *h2, Double_t c1=1, Double_t c2=1, Option_t *option="") override { return h->Divide(h1,h2,c1,c2,option); }
		Bool_t Divide (TF1 *f1, Double_t c1=1) override { return h->Divide(f1,c1); }
		Bool_t Divide (const THN *h1) override {
			if(!h1->TH1()) return false;
			return h->Divide(h1->TH1());
		}
		Bool_t Divide (const THN *h1, const THN *h2, Double_t c1=1, Double_t c2=1, Option_t *option="") override {
			if(!h1->TH1() or !h2->TH1()) return false;
			return h->Divide(h1->TH1(),h2->TH1(),c1,c2,option);
		}
		Bool_t Multiply (const ::TH1 *h1) override { return h->Multiply(h1); }
		Bool_t Multiply (const ::TH1 *h1, const ::TH1 *h2, Double_t c1=1, Double_t c2=1, Option_t *option="") override { return h->Multiply(h1,h2,c1,c2,option); }
		Bool_t Multiply (TF1 *f1, Double_t c1=1) override { return h->Multiply(f1,c1); }
		Bool_t Multiply (const THN *h1) override {
			if(!h1->TH1()) return false;
			return h->Multiply(h1->TH1());
		}
		Bool_t Multiply (const THN *h1, const THN *h2, Double_t c1=1, Double_t c2=1, Option_t *option="") override {
			if(!h1->TH1() or !h2->TH1()) return false;
			return h->Multiply(h1->TH1(),h2->TH1(),c1,c2,option);
		}

		THN * Clone (const char *newname="") override { return new THNT<::TH1>((::TH1*)h->Clone(newname)); }
		void Draw (Option_t *option="") override { h->Draw(option); }
		Long64_t Fill(const vector<Double_t>& x, Double_t w=1.) override {
			if(isTProfile) return static_cast<TProfile*>(h)->Fill(x[0], x[1], w);
			else if(isTH2) return static_cast<TH2*>(h)->Fill(x[0], x[1], w);
			else return h->Fill(x[0], w);
		}
		TFitResultPtr Fit (TF1 *f1, Option_t *option="", Option_t *goption="", Double_t xmin=0, Double_t xmax=0) override { return h->Fit(f1,option,goption,xmin,xmax); }
		Long64_t FindBin (Double_t x, Double_t y=0, Double_t z=0) override { return (Long64_t)h->FindBin(x,y,z); }
		Long64_t GetBin (Int_t binx, Int_t biny=0, Int_t binz=0) const override { return (Long64_t)h->GetBin(binx,biny,binz); }
		Double_t GetBinContent (Long64_t bin) const override { return h->GetBinContent((Int_t)bin); }
		Double_t GetBinError (Long64_t bin) const override { return h->GetBinError((Int_t)bin); }
		::TH1::EBinErrorOpt GetBinErrorOption () const override { return h->GetBinErrorOption(); }
		Int_t GetDimension () const override { return h->GetDimension(); }
		Int_t GetNbinsX () const override { return h->GetNbinsX(); }
		Int_t GetNbinsY () const override { return h->GetNbinsY(); }
		TAxis * GetAxis (Int_t dim) override {
			if(dim==0) return GetXaxis();
			else if(dim==1) return GetYaxis();
			else if(dim==2) return GetZaxis();
			else { unimpl(string(__PRETTY_FUNCTION__)+"w/ arg "+to_string(dim)); return nullptr; }
		}
		const TAxis * GetAxis (Int_t dim) const override {
			if(dim==0) return GetXaxis();
			else if(dim==1) return GetYaxis();
			else if(dim==2) return GetZaxis();
			else { unimpl(string(__PRETTY_FUNCTION__)+"w/ arg "+to_string(dim)); return nullptr; }
		}
		TAxis* GetXaxis() override { return h->GetXaxis(); }
		const TAxis* GetXaxis() const override { return h->GetXaxis(); }
		TAxis* GetYaxis() override { return h->GetYaxis(); }
		const TAxis* GetYaxis() const override { return h->GetYaxis(); }
		TAxis* GetZaxis() override { return h->GetZaxis(); }
		const TAxis* GetZaxis() const override { return h->GetZaxis(); }
		Bool_t InheritsFrom (const char *classname) const override { return h->InheritsFrom(classname); }
		Bool_t InheritsFrom (const TClass *cl) const override { return h->InheritsFrom(cl); }
		Double_t Integral (Int_t binx1, Int_t binx2, Option_t *option="") const override { return h->Integral(binx1,binx2,option); }
		Double_t Integral (Option_t *option="") const override { return h->Integral(option); }
		Double_t IntegralAndError (Int_t binx1, Int_t binx2, Double_t &err, Option_t *option="") const override { return h->IntegralAndError(binx1,binx2,err,option); }
		void Print (Option_t *option="") const override { h->Print(option); }
		THN * Rebin (Int_t ngroup=2, const char *newname="", const Double_t *xbins=0) override {
			::TH1* hnew = h->Rebin(ngroup,newname,xbins);
			if(!newname){ h = hnew; return this; }
			else { return new THNT<::TH1>(hnew); }
		}
		void Reset (Option_t *option="") override { h->Reset(option); }
		void Scale (Double_t c1=1, Option_t *option="") override { h->Scale(c1,option); }
		void SetAxisRange (Double_t xmin, Double_t xmax, Option_t *axis="X") override { h->SetAxisRange(xmin,xmax,axis); }
		void SetBinContent (Long64_t bin, Double_t content) override { h->SetBinContent((Int_t)bin,content); }
		void SetBinError (Long64_t bin, Double_t error) override { h->SetBinError((Int_t)bin,error); }
		void SetBinErrorOption (::TH1::EBinErrorOpt type) override { h->SetBinErrorOption(type); }
		void SetDirectory (TDirectory *dir) override { h->SetDirectory(dir); }
		void SetName (const char *name) override { h->SetName(name); }
		void Sumw2 (Bool_t flag=kTRUE) override { h->Sumw2(flag); }
		Int_t Write (const char *name=0, Int_t option=0, Int_t bufsize=0) override { return h->Write(name,option,bufsize); }
		Int_t Write (const char *name=0, Int_t option=0, Int_t bufsize=0) const override { return h->Write(name,option,bufsize); }

		//style accessors
		void SetFillColor (Color_t fcolor) override { h->SetFillColor(fcolor); }
		void SetFillStyle (Style_t fstyle) override { h->SetFillStyle(fstyle); }
		void SetLineColor (Color_t lcolor) override { h->SetLineColor(lcolor); }
		void SetLineStyle (Style_t lstyle) override { h->SetLineStyle(lstyle); }
		void SetLineWidth (Width_t lwidth) override { h->SetLineWidth(lwidth); }
		void SetMarkerColor (Color_t mcolor=1) override { h->SetMarkerColor(mcolor); }
		void SetMarkerSize (Size_t msize=1) override { h->SetMarkerSize(msize); }
		void SetMarkerStyle (Style_t mstyle=1) override { h->SetMarkerStyle(mstyle); }

	protected:
		::TH1* h;
		bool isTH2, isTProfile;
};
using THN1 = THNT<::TH1>;

template <>
class THNT<::THnSparse> : public THN {
	public:
		THNT<::THnSparse>(::THnSparse* h_) : h(h_) { debug(__PRETTY_FUNCTION__); }
		::THnSparse* THnSparse() override { return h; }
		const ::THnSparse* THnSparse() const override { return h; }
		string cl() const override { return "THnSparse"; }
		void debug(const string& fn) const {
#ifdef DEBUG_THN
			cout << "THnSparse: called " << fn << endl; h->Print("asm"); cout << h->GetWeightSum() << endl;
#endif
		}

		Bool_t Add (const ::TH1 *h1, Double_t c1=1) override { debug(__PRETTY_FUNCTION__); h->Add(h1,c1); return true; }
		Bool_t Add (const THN *h1, Double_t c1=1) override { debug(__PRETTY_FUNCTION__);
			if(h1->THnSparse()) { h->Add(h1->THnSparse(),c1); return true; }
			else { h->Add(h1->TH1(),c1); return true; }
		}
		Bool_t Divide (const THN *h1) override { debug(__PRETTY_FUNCTION__);
			if(!h1->THnSparse()) return false;
			h->Divide(h1->THnSparse());
			return true;
		}
		Bool_t Divide (const THN *h1, const THN *h2, Double_t c1=1, Double_t c2=1, Option_t *option="") override { debug(__PRETTY_FUNCTION__);
			if(!h1->THnSparse() or !h2->THnSparse()) return false;
			h->Divide(h1->THnSparse(),h2->THnSparse(),c1,c2,option);
			return true;
		}
		Bool_t Multiply (TF1 *f1, Double_t c1=1) override { debug(__PRETTY_FUNCTION__); h->Multiply(f1,c1); return true; }
		Bool_t Multiply (const THN *h1) override { debug(__PRETTY_FUNCTION__);
			if(!h1->THnSparse()) return false;
			h->Multiply(h1->THnSparse());
			return true;
		}

		THN * Clone (const char *newname="") override { debug(__PRETTY_FUNCTION__); return new THNT<::THnSparse>((::THnSparse*)h->Clone(newname)); }
		void Draw (Option_t *option="") override { debug(__PRETTY_FUNCTION__); h->Draw(option); }
		Long64_t Fill(const vector<Double_t>& x, Double_t w=1.) override { debug(__PRETTY_FUNCTION__); return h->Fill(x.data(),w); }
		TFitResultPtr Fit (TF1 *f1, Option_t *option="", Option_t *goption="", Double_t xmin=0, Double_t xmax=0) override { debug(__PRETTY_FUNCTION__);
			if(xmin!=0 or xmax!=0) { unimpl(__PRETTY_FUNCTION__); return nullptr; }
			return h->Fit(f1,option,goption);
		}
		Long64_t FindBin (Double_t x, Double_t y=0, Double_t z=0) override { debug(__PRETTY_FUNCTION__);
			vector<int> values(GetDimension(),0);
			if(values.size()>0) values[0] = x;
			if(values.size()>1) values[1] = y;
			if(values.size()>2) values[2] = z;
			return h->GetBin(values.data(),false);
		}
		Long64_t GetBin (Int_t binx, Int_t biny=0, Int_t binz=0) const override { debug(__PRETTY_FUNCTION__);
			vector<int> coords(GetDimension(),0);
			if(coords.size()>0) coords[0] = binx;
			if(coords.size()>1) coords[1] = biny;
			if(coords.size()>2) coords[2] = binz;
			return h->GetBin(coords.data(),false);
		}
		Double_t GetBinContent (Long64_t bin) const override { debug(__PRETTY_FUNCTION__); return h->GetBinContent(bin); }
		Double_t GetBinError (Long64_t bin) const override { debug(__PRETTY_FUNCTION__); return h->GetBinError(bin); }
		::TH1::EBinErrorOpt GetBinErrorOption () const override { debug(__PRETTY_FUNCTION__); return ::TH1::kNormal; }
		Int_t GetDimension () const override { debug(__PRETTY_FUNCTION__); return h->GetNdimensions(); }
		Int_t GetNbinsX () const override { debug(__PRETTY_FUNCTION__); return GetXaxis()->GetNbins(); }
		Int_t GetNbinsY () const override { debug(__PRETTY_FUNCTION__); return GetYaxis()->GetNbins(); }
		TAxis * GetAxis (Int_t dim) override { debug(__PRETTY_FUNCTION__); return h->GetAxis(dim); }
		const TAxis * GetAxis (Int_t dim) const override { debug(__PRETTY_FUNCTION__); return h->GetAxis(dim); }
		TAxis* GetXaxis() override { debug(__PRETTY_FUNCTION__); return h->GetAxis(0); }
		const TAxis* GetXaxis() const override { debug(__PRETTY_FUNCTION__); return h->GetAxis(0); }
		TAxis* GetYaxis() override { debug(__PRETTY_FUNCTION__); return h->GetAxis(1); }
		const TAxis* GetYaxis() const override { debug(__PRETTY_FUNCTION__); return h->GetAxis(1); }
		TAxis* GetZaxis() override { debug(__PRETTY_FUNCTION__); return h->GetAxis(2); }
		const TAxis* GetZaxis() const override { debug(__PRETTY_FUNCTION__); return h->GetAxis(2); }
		Bool_t InheritsFrom (const char *classname) const override { debug(__PRETTY_FUNCTION__); return h->InheritsFrom(classname); }
		Bool_t InheritsFrom (const TClass *cl) const override { debug(__PRETTY_FUNCTION__); return h->InheritsFrom(cl); }
		Double_t Integral (Int_t binx1, Int_t binx2, Option_t *option="") const override { debug(__PRETTY_FUNCTION__);
			TString opt = option;
			opt.ToLower();
			if(binx1==-1 and binx2==-1 and !opt.Contains("width")) return h->GetWeightSum();
			else { unimpl(__PRETTY_FUNCTION__); return 0.; }
		}
		Double_t Integral (Option_t *option="") const override { debug(__PRETTY_FUNCTION__);
			TString opt = option;
			opt.ToLower();
			if(!opt.Contains("width")) return h->ComputeIntegral();
			else { unimpl(__PRETTY_FUNCTION__); return 0.; }
		}
		//todo: get a sensible value for err
		Double_t IntegralAndError (Int_t binx1, Int_t binx2, Double_t &err, Option_t *option="") const override { debug(__PRETTY_FUNCTION__); err = 0.; return Integral(binx1,binx2,option); }
		void Print (Option_t *option="") const override { debug(__PRETTY_FUNCTION__); h->Print(option); }
		//the behavior for xbins!=0 is not the same as TH1
		THN * Rebin (Int_t ngroup=2, const char *newname="", const Double_t *xbins=0) override { debug(__PRETTY_FUNCTION__);
			::THnSparse* hnew = nullptr;
			if(xbins) { vector<int> group; group.insert(group.end(), xbins, xbins + h->GetNbins()); hnew = h->Rebin(group.data()); }
			else { hnew = h->Rebin(ngroup); }

			if(!newname){ h = hnew; return this; }
			else { return new THNT<::THnSparse>(hnew); }
		}
		void Reset (Option_t *option="") override { debug(__PRETTY_FUNCTION__); h->Reset(option); }
		void Scale (Double_t c1=1, Option_t *option="") override { debug(__PRETTY_FUNCTION__);
			TString opt = option;
			opt.ToLower();
			if(!opt.Contains("width")) h->Scale(c1);
			else { unimpl(__PRETTY_FUNCTION__); }
		}
		void SetAxisRange (Double_t xmin, Double_t xmax, Option_t *axis="X") override { debug(__PRETTY_FUNCTION__);
			//based on TH1::AxisChoice() and TH1::SetAxisRange()
			char achoice = toupper(axis[0]);
			TAxis* taxis = nullptr;
			if (achoice == 'X') taxis = GetXaxis();
			if (achoice == 'Y') taxis = GetYaxis();
			if (achoice == 'Z') taxis = GetZaxis();
			if(!taxis) return;
			Int_t bin1 = taxis->FindFixBin(xmin);
			Int_t bin2 = taxis->FindFixBin(xmax);
			taxis->SetRange(bin1, bin2);
		}
		void SetBinContent (Long64_t bin, Double_t content) override { debug(__PRETTY_FUNCTION__); h->SetBinContent(bin,content); }
		void SetBinError (Long64_t bin, Double_t error) override { debug(__PRETTY_FUNCTION__); h->SetBinError(bin,error); }
		void SetBinErrorOption (::TH1::EBinErrorOpt type) override { debug(__PRETTY_FUNCTION__); } //does nothing
		void SetDirectory (TDirectory *dir) override { debug(__PRETTY_FUNCTION__); }
		void SetName (const char *name) override { debug(__PRETTY_FUNCTION__); h->SetName(name); }
		void Sumw2 (Bool_t flag=kTRUE) override { debug(__PRETTY_FUNCTION__); h->Sumw2(); } //false does nothing
		Int_t Write (const char *name=0, Int_t option=0, Int_t bufsize=0) override { debug(__PRETTY_FUNCTION__); return h->Write(name,option,bufsize); }
		Int_t Write (const char *name=0, Int_t option=0, Int_t bufsize=0) const override { debug(__PRETTY_FUNCTION__); return h->Write(name,option,bufsize); }

		//style accessors do nothing for THn
		void SetFillColor (Color_t fcolor) override { debug(__PRETTY_FUNCTION__); }
		void SetFillStyle (Style_t fstyle) override { debug(__PRETTY_FUNCTION__); }
		void SetLineColor (Color_t lcolor) override { debug(__PRETTY_FUNCTION__); }
		void SetLineStyle (Style_t lstyle) override { debug(__PRETTY_FUNCTION__); }
		void SetLineWidth (Width_t lwidth) override { debug(__PRETTY_FUNCTION__); }
		void SetMarkerColor (Color_t mcolor=1) override { debug(__PRETTY_FUNCTION__); }
		void SetMarkerSize (Size_t msize=1) override { debug(__PRETTY_FUNCTION__); }
		void SetMarkerStyle (Style_t mstyle=1) override { debug(__PRETTY_FUNCTION__); }

	protected:
		::THnSparse* h;
};
using THNn = THNT<::THnSparse>;

#endif

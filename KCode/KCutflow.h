#ifndef KCUTFLOW_H
#define KCUTFLOW_H

//custom headers
#include "KMath.h"
#include "KMap.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>

//STL headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdlib>
#include <exception>

using namespace std;

class KCutflow {
	public:
		//enum
		enum CutflowType { CutRaw=0, CutAbs=1, CutRel=2 };
	
		//constructors
		KCutflow(string name_, string fname_) : name(name_), h_raw(NULL), h_abs(NULL), h_rel(NULL) {
			TFile* file = KOpen(fname_);
			GetFromFile(file);
		}
		KCutflow(string name_, TFile* file) : name(name_), h_raw(NULL), h_abs(NULL), h_rel(NULL) {
			GetFromFile(file);
		}
		//get nevent info from histo
		KCutflow(string name_, TH1F* h_tmp, TH1F* h_ntmp) : name(name_), h_raw(h_tmp), nentries(h_ntmp->GetBinContent(1)), nentriesE(h_ntmp->GetBinError(1)), h_abs(NULL), h_rel(NULL) { 
			//initialize derived histos
			CalcEfficiency();
		}
		//get nevent info directly
		KCutflow(string name_, TH1F* h_tmp, long long nentries_, double nentriesE_=0) : name(name_), h_raw(h_tmp), nentries(nentries_), nentriesE(nentriesE_), h_abs(NULL), h_rel(NULL) { 
			//initialize derived histos
			CalcEfficiency();
		}
		//constructor helper
		void GetFromFile(TFile* file){
			if(!file) throw runtime_error("null file pointer!");
			
			h_raw = KGet<TH1F>(file,"cutflow");
			
			TH1F* h_nevent = KGet<TH1F>(file,"nEventProc");

			nentries = h_nevent->GetBinContent(1);
			nentriesE = h_nevent->GetBinError(1);
			
			//initialize derived histos
			CalcEfficiency();			
		}
		//destructor
		virtual ~KCutflow() { }
		
		//accessors
		//todo: expand for object sync selectors
		
		//make abs and rel efficiency histograms
		void CalcEfficiency(){
			if(!h_raw) return;

			//initialize histos
			vector<string> names = {"cutflow","cutflowAbs","cutflowRel"};
			if(name.size()>0){
				for(auto& name_ : names) name_ += "_"+name;
			}
			h_raw->SetName((names[0]).c_str());
			title = h_raw->GetTitle();
			h_raw->SetTitle("");
			h_abs = new TH1F((names[1]).c_str(),"",h_raw->GetNbinsX(),0,h_raw->GetNbinsX());
			h_rel = new TH1F((names[2]).c_str(),"",h_raw->GetNbinsX(),0,h_raw->GetNbinsX());
			
			//fill histos
			for(int c = 1; c <= h_raw->GetNbinsX(); c++){
				long long counter = (long long)h_raw->GetBinContent(c);
				long long prev_counter = c==1 ? nentries : (long long)h_raw->GetBinContent(c-1);
				
				h_abs->GetXaxis()->SetBinLabel(c,h_raw->GetXaxis()->GetBinLabel(c));
				h_abs->SetBinContent(c,((double)counter/(double)nentries)*100);
				h_abs->SetBinError(c,KMath::EffError(counter,nentries)*100);
				
				h_rel->GetXaxis()->SetBinLabel(c,h_raw->GetXaxis()->GetBinLabel(c));
				h_rel->SetBinContent(c,((double)counter/(double)prev_counter)*100);
				h_rel->SetBinError(c,KMath::EffError(counter,prev_counter)*100);
			}
			h_abs->GetXaxis()->SetNoAlphanumeric();
			h_rel->GetXaxis()->SetNoAlphanumeric();
		}
		//print efficiencies
		void PrintEfficiency(bool printerrors=false){
			if(!h_raw) return;
			if(!h_abs || !h_rel) GetEfficiency();

			//loop to get width of selector name
			widths = vector<unsigned>(6,0);
			unsigned width1s = 10;
			for(int c = 1; c <= h_raw->GetNbinsX(); ++c){
				string sname = h_raw->GetXaxis()->GetBinLabel(c);
				if(sname.size()>width1s) width1s = sname.size();
			}
			
			//setup widths
			widths[0] = width1s;
			if(printerrors) {
				widths[4] = max(log10(nentries)+1,log10(sqrt(nentries))+1+3); //extra width for num and err, based on # digits
				int numcolwidth1 = widths[4]*2 + 5; //x + 5 + x (num +/- err)
				widths[1] = numcolwidth1;
				widths[5] = 6; //extra width for eff and err (assumes yieldprecision = 2)
				int numcolwidth2 = widths[5]*2 + 5; //6 + 5 + 6 (eff +/- err)
				widths[2] = widths[3] = numcolwidth2;
			}
			else {
				int numcolwidth = 13;
				widths[1] = widths[2] = widths[3] = numcolwidth;
			}
			
			//print preamble
			cout << string(widths[0]+widths[1]+widths[2]+widths[3]+2*(4-1),'-') << endl;
			cout << "Selection: " << title << endl;
			cout << left << setw(widths[0]) << "Selector" << "  " << right << setw(widths[1]) << "Raw # Events" << "  " << right << setw(widths[2]) << "Abs. Eff. (%)" << "  " << right << setw(widths[3]) << "Rel. Eff. (%)" << endl;
			cout << left << setw(widths[0]) << "NEventProc";
			if(printerrors) cout << "  " << right << setw(widths[4]) << nentries << " +/- " << right << setw(widths[4]) << nentriesE << endl;
			else cout << "  " << right << setw(widths[1]) << nentries << endl;
			
			//print selectors
			for(int c = 1; c <= h_raw->GetNbinsX(); ++c){
				cout << left << setw(widths[0]) << h_raw->GetXaxis()->GetBinLabel(c);
				long long raw = (long long)h_raw->GetBinContent(c); double rawE = h_raw->GetBinError(c);
				double abs = h_abs->GetBinContent(c); double absE = h_abs->GetBinError(c);
				double rel = h_rel->GetBinContent(c); double relE = h_rel->GetBinError(c);				
				if(printerrors){
					cout << "  " << right << setw(widths[4]) << raw << " +/- " << right << setw(widths[4]) << rawE;
					cout << "  " << right << setw(widths[5]) << abs << " +/- " << right << setw(widths[5]) << absE;
					//rel. eff. = abs. eff. for first selector
					cout << "  " << right << setw(widths[5]) << rel << " +/- " << right << setw(widths[5]) << relE;
				}
				else {
					cout << "  " << right << setw(widths[1]) << raw;
					cout << "  " << right << setw(widths[2]) << abs;
					//rel. eff. = abs. eff. for first selector
					cout << "  " << right << setw(widths[3]) << rel;
				}
				cout << endl;
			}
		}
		//get an efficiency histogram
		TH1F* GetEfficiency(CutflowType ct=CutRaw, bool cutflownorm=false){
			if(ct==CutRaw) return h_raw;
			else if(ct==CutAbs) {
				if(cutflownorm) h_abs->Scale(100./h_abs->GetBinContent(1));
				return h_abs;
			}
			else if(ct==CutRel) {
				if(cutflownorm) h_rel->Scale(100./h_rel->GetBinContent(1));
				return h_rel;
			}
			else return NULL;
		}
	
	private:
		//members
		string name, title;
		TH1F *h_raw, *h_abs, *h_rel;
		long long nentries;
		double nentriesE;
		vector<unsigned> widths;
};

#endif

#ifndef KCUTFLOW_H
#define KCUTFLOW_H

//custom headers
#include "KMath.h"

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

using namespace std;

class KCutflow {
	public:
		//constructors
		KCutflow(string fname_) : h_raw(NULL), h_abs(NULL), h_rel(NULL) {
			TFile* file = TFile::Open(fname_.c_str());
			if(!file) {
				cout << "Input error: could not open file " << fname_ << endl;
			}
			h_raw = (TH1F*)file->Get("cutflow");
			if(!h_raw){
				cout << "Input error: could not find cutflow histogram in file " << fname_ << endl;
			}
			
			//initialize derived histos
			GetEfficiency();
		}
		KCutflow(TH1F* h_tmp) : h_raw(h_tmp), h_abs(NULL), h_rel(NULL) { 
			//initialize derived histos
			GetEfficiency();
		}
		//destructor
		virtual ~KCutflow() { }
		
		//accessors
		//todo: expand for object sync selectors
		
		//make abs and rel efficiency histograms
		void GetEfficiency(){
			if(!h_raw) return;
			
			//get info from histo
			int nentries = (int)h_raw->GetBinContent(1);
			
			//initialize histos
			h_abs = new TH1F("cutflowAbs","",h_raw->GetNbinsX()-1,0,h_raw->GetNbinsX()-1);
			h_rel = new TH1F("cutflowRel","",h_raw->GetNbinsX()-1,0,h_raw->GetNbinsX()-1);
			
			//fill histos
			for(unsigned c = 2; c <= h_raw->GetNbinsX(); c++){
				int counter = (int)h_raw->GetBinContent(c);
				int prev_counter = (int)h_raw->GetBinContent(c-1);
				
				h_abs->GetXaxis()->SetBinLabel(c-1,h_raw->GetXaxis()->GetBinLabel(c));
				h_abs->SetBinContent(c-1,((double)counter/(double)nentries)*100);
				h_abs->SetBinError(c-1,KMath::EffError(counter,nentries)*100);
				
				h_rel->GetXaxis()->SetBinLabel(c-1,h_raw->GetXaxis()->GetBinLabel(c));
				h_rel->SetBinContent(c-1,((double)counter/(double)prev_counter)*100);
				h_rel->SetBinError(c-1,KMath::EffError(counter,prev_counter)*100);
			}
		}
		//print efficiencies
		void PrintEfficiency(bool printerrors=false){
			if(!h_raw) return;
			if(!h_abs || !h_rel) GetEfficiency();
			
			//get info from histo
			int nentries = (int)h_raw->GetBinContent(1);
			double nentriesE = (int)h_raw->GetBinError(1);
			
			//loop to get width of selector name
			widths = vector<unsigned>(6,0);
			unsigned width1s = 10;
			for(unsigned c = 1; c <= h_raw->GetNbinsX(); ++c){
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
			cout << "Selection: " << h_raw->GetTitle() << endl;
			cout << left << setw(widths[0]) << "Selector" << "  " << right << setw(widths[1]) << "Raw # Events" << "  " << right << setw(widths[2]) << "Abs. Eff. (%)" << "  " << right << setw(widths[3]) << "Rel. Eff. (%)" << endl;
			cout << left << setw(widths[0]) << "NEventProc";
			if(printerrors) cout << "  " << right << setw(widths[4]) << nentries << " +/- " << right << setw(widths[4]) << nentriesE << endl;
			else cout << "  " << right << setw(widths[1]) << nentries << endl;
			
			//print selectors
			for(unsigned c = 2; c <= h_raw->GetNbinsX(); ++c){
				cout << left << setw(widths[0]) << h_raw->GetXaxis()->GetBinLabel(c);
				int raw = (int)h_raw->GetBinContent(c); double rawE = h_raw->GetBinError(c);
				double abs = h_abs->GetBinContent(c-1); double absE = h_abs->GetBinError(c-1);
				double rel = h_rel->GetBinContent(c-1); double relE = h_rel->GetBinError(c-1);				
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
	
	private:
		//members
		TH1F *h_raw, *h_abs, *h_rel;
		vector<unsigned> widths;
};

#endif
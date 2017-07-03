#ifndef KBUILDERSELECTORS_H
#define KBUILDERSELECTORS_H

//custom headers
#include "KSelection.h"
#include "KCommonSelectors.h"
#include "KMath.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>

//STL headers
#include <string>
#include <vector>
#include <cstdlib>
#include <map>
#include <set>

using namespace std;

//base class for Selectors is in KSelection.h

//--------------------------------------------------------------------------
//does cross section normalization, data/MC corrections, uncertainties, etc.
class KMCWeightSelector : public KSelector {
	public:
		//constructor
		KMCWeightSelector() : KSelector() {}
		KMCWeightSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { 
		}
		virtual void CheckBase(){
			//standard weight options
			normtype = ""; base->GetLocalOpt()->Get("normtype",normtype); GetNormTypeEnum();
			unweighted = base->GetLocalOpt()->Get("unweighted",false);
			nEventProc = 0; got_nEventProc = base->GetLocalOpt()->Get("nEventProc",nEventProc);
			xsection = 0; got_xsection = base->GetLocalOpt()->Get("xsection",xsection);
			norm = 0; got_luminorm = base->GetGlobalOpt()->Get("luminorm",norm);
		}
		virtual void CheckBranches(){
		}
		//enum for normtypes
		enum normtypes { NoNT=0 };
		//convert normtype from string to enum for quicker compares
		void GetNormTypeEnum(){
			NTenum = NoNT;
		}
		double GetWeight(){
			double w = 1.;
			if(unweighted) return w;
			
			//now do scaling: norm*xsection/nevents
			if(got_nEventProc && nEventProc>0 && got_xsection){
				w *= xsection/nEventProc;
				//account for negative weight events
//				if(looper->Weight<0) w *= -1;
			}
			
			//use lumi norm (default)
			if(got_luminorm) w *= norm;
			
			return w;
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			bool goodEvent = true;
			
			//check normalization type here
		
			return goodEvent;
		}
		
		//member variables
		bool unweighted, got_nEventProc, got_xsection, got_luminorm;
		string normtype;
		normtypes NTenum;
		int nEventProc;
		double xsection, norm;
};
REGISTER_SELECTOR(MCWeight);

//---------------------------------------------------------------
//little class to store value & weight pairs for filling histos
class KValue {
	public:
		//constructor
		KValue() : values(0), weights(0) {}
		//accessors
		void Fill(double v, double w=1){
			values.push_back(v);
			weights.push_back(w);
		}
		double & GetValue(int iv) { return values[iv]; }
		double & GetWeight(int iw) { return weights[iw]; }
		int GetSize() { return values.size(); }
		
	protected:
		//member variables
		vector<double> values;
		vector<double> weights;
	
};

//----------------------------------------------------
//final selector to fill histograms
//(accounts for dependence on other selectors)
class KHistoSelector : public KSelector {
	public:
		//constructor
		KHistoSelector() : KSelector() { }
		KHistoSelector(string name_, OptionMap* localOpt_) : 
			KSelector(name_,localOpt_), initialized(false), MCWeight(NULL) 
		{ 
			canfail = false;
		}
		
		virtual void CheckDeps(){
			//set dependencies here
			MCWeight = sel->Get<KMCWeightSelector*>("MCWeight");
		}
		virtual void CheckBranches(){
		}
		virtual void CheckBase(){
			//do not use MCWeight with data
			if(base->IsData()) MCWeight = NULL;
			//but require it for MC
			else if(base->IsMC() && !MCWeight) depfailed = true;
			
			if(depfailed) canfail = true;
		}
		void Initialize(){
			if(initialized) return;
			//initial loop to get histo variables
			int table_size = base->GetTable().size();
			vars.clear(); vars.reserve(table_size);
			htmp.clear(); htmp.reserve(table_size);
			for(auto& sit : base->GetTable()){
				//get histo name
				string stmp = sit.first;
				htmp.push_back(sit.second);
				//split up histo variable names
				vector<string> vars_tmp;
				KParser::process(stmp,'_',vars_tmp);
				vars.push_back(vars_tmp);
			}
			initialized = true;
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(!initialized) Initialize();
			if(depfailed) return false;
			
			double w = 1.0;
			if(MCWeight) w = MCWeight->GetWeight();
			
			for(unsigned h = 0; h < htmp.size(); h++){
				unsigned vsize = vars[h].size();
				vector<KValue> values(vsize);				
			
				for(unsigned i = 0; i < vsize; i++){
					string vname = vars[h][i];
					//list of cases for histo calculation and filling
//					if(vname=="njets"){//jet multiplicity
//						values[i].Fill(looper->NJets,w);
//					}
//					else { //if it's a histogram with no known variable or calculation, do nothing
//					}
				}
				
				//now fill the histogram
				if(vsize==1){
					for(int ix = 0; ix < values[0].GetSize(); ix++){
						htmp[h]->Fill(values[0].GetValue(ix), values[0].GetWeight(ix));
					}
				}
				else if(vsize==2){
					//need to cast in order to use Fill(x,y,w)
					//these three cases allow for various x vs. y comparisons: same # entries per event, or 1 vs. N per event
					if(values[0].GetSize()==values[1].GetSize()) {
						for(int i = 0; i < values[0].GetSize(); i++){
							if(htmp[h]->GetDimension()==1)
								static_cast<TProfile*>(htmp[h])->Fill(values[0].GetValue(i), values[1].GetValue(i), values[0].GetWeight(i)); //pick the x weight by default
							else if(htmp[h]->GetDimension()==2)
								static_cast<TH2*>(htmp[h])->Fill(values[0].GetValue(i), values[1].GetValue(i), values[0].GetWeight(i)); //pick the x weight by default
						}
					}
					else if(values[0].GetSize()==1){
						for(int iy = 0; iy < values[1].GetSize(); iy++){
							if(htmp[h]->GetDimension()==1)
								static_cast<TProfile*>(htmp[h])->Fill(values[0].GetValue(0), values[1].GetValue(iy), values[1].GetWeight(iy));
							else if(htmp[h]->GetDimension()==2)
								static_cast<TH2*>(htmp[h])->Fill(values[0].GetValue(0), values[1].GetValue(iy), values[1].GetWeight(iy));
						}
					}
					else if(values[1].GetSize()==1){
						for(int ix = 0; ix < values[0].GetSize(); ix++){
							if(htmp[h]->GetDimension()==1)
								static_cast<TProfile*>(htmp[h])->Fill(values[0].GetValue(ix), values[1].GetValue(0), values[0].GetWeight(ix));
							else if(htmp[h]->GetDimension()==2)
								static_cast<TH2*>(htmp[h])->Fill(values[0].GetValue(ix), values[1].GetValue(0), values[0].GetWeight(ix));
						}
					}
				}
				else { //no support for other # of vars
				}
			}
			
			return true;
		}
		virtual void Finalize(TFile* file){
			if(sel->GetGlobalOpt()->Get("plotoverflow",false)){
				for(unsigned h = 0; h < htmp.size(); h++){
					if(vars[h].size()==2) continue; //not implemented for 2D histos or profiles yet
					
					//temporary histo to calculate error correctly when adding overflow bin to last bin
					TH1* otmp = (TH1*)htmp[h]->Clone();
					otmp->Reset("ICEM");
					int ovbin = htmp[h]->GetNbinsX()+1;
					double err = 0.;
					otmp->SetBinContent(ovbin-1,htmp[h]->IntegralAndError(ovbin,ovbin,err));
					otmp->SetBinError(ovbin-1,err);
					
					//add overflow bin to last bin
					htmp[h]->Add(otmp);
					
					//remove overflow bin from htmp[h] (for consistent integral/yield)
					htmp[h]->SetBinContent(ovbin,0);
					htmp[h]->SetBinError(ovbin,0);
					
					delete otmp;
				}
			}
		}
		
		//member variables
		bool initialized;
		vector<vector<string> > vars;
		vector<TH1*> htmp;
		KMCWeightSelector* MCWeight;
};
REGISTER_SELECTOR(Histo);

#endif

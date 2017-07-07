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
		//for mass comparisons
		enum masstypes { NoMass=0, MJJ=1, MT=2, Mmc=3, MAOS=4 };
		//constructor
		KHistoSelector() : KSelector() { }
		KHistoSelector(string name_, OptionMap* localOpt_) : 
			KSelector(name_,localOpt_), initialized(false), MCWeight(NULL), mtype(NoMass)
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
			string mname;
			base->GetLocalOpt()->Get("masstype",mname);
			if(mname=="MJJ") mtype = MJJ;
			else if(mname=="MT") mtype = MT;
			else if(mname=="Mmc") mtype = Mmc;
			else if(mname=="MAOS") mtype = MAOS;
			//else cout << "Input error: unknown mass type: " << mname << endl;

			//do not use MCWeight with data
			if(base->IsData()) MCWeight = NULL;
			//but require it for MC
			else if(base->IsMC() && !MCWeight) depfailed = true;
			
			if(depfailed){
				canfail = true;
				cout << "Input error: missing MCWeight selector for MC set: " << base->GetName() << endl;
			}
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
		bool IsPerJet(const string& vname, KValue& value, double w){
			bool leadjet = false, subleadjet = false;
			string vname2;
			
			if(vname.compare(0,7,"leadjet")==0) {
				leadjet = true;
				vname2 = vname.substr(7,string::npos);
			}
			else if(vname.compare(0,10,"subleadjet")==0){
				subleadjet = true;
				vname2 = vname.substr(10,string::npos);
			}
			else if(vname.compare(0,7,"bothjet")==0) {
				leadjet = true;
				subleadjet = true;
				vname2 = vname.substr(7,string::npos);
			}

			if(leadjet) FillPerJet(vname2,value,w,0);
			if(subleadjet) FillPerJet(vname2,value,w,1);
			
			return (leadjet or subleadjet);
		}
		void FillPerJet(const string& vname, KValue& value, double w, unsigned index){
			if(looper->GenJetsAK8->size()>index){
				if(vname=="pt") value.Fill(looper->GenJetsAK8->at(index).Pt(),w);
				else if(vname=="eta") value.Fill(looper->GenJetsAK8->at(index).Eta(),w);
				else if(vname=="axisminor") value.Fill(looper->GenJetsAK8_AxisMinor->at(index),w);
				else if(vname=="axismajor") value.Fill(looper->GenJetsAK8_AxisMajor->at(index),w);
				else if(vname=="axisaverage") value.Fill(looper->GenJetsAK8_AxisAverage->at(index),w);
				else if(vname=="girth") value.Fill(looper->GenJetsAK8_MomentGirth->at(index),w);
				else if(vname=="mhalf") value.Fill(looper->GenJetsAK8_MomentHalf->at(index),w);
				else if(vname=="mult") value.Fill(looper->GenJetsAK8_Multiplicity->at(index),w);
				else if(vname=="overflow") value.Fill(looper->GenJetsAK8_Overflow->at(index),w);
				else if(vname=="ptD") value.Fill(looper->GenJetsAK8_PtD->at(index),w);
				//derived substructure variables
				else if(vname=="tau21"){
					if(looper->GenJetsAK8_Tau1->at(index)>0) value.Fill(looper->GenJetsAK8_Tau2->at(index)/looper->GenJetsAK8_Tau1->at(index),w);
				}
				else if(vname=="tau32"){
					if(looper->GenJetsAK8_Tau2->at(index)>0) value.Fill(looper->GenJetsAK8_Tau3->at(index)/looper->GenJetsAK8_Tau2->at(index),w);
				}
				else if(vname=="ecfC2"){
					if(looper->GenJetsAK8_ECF2->at(index)>0) value.Fill(looper->GenJetsAK8_ECF3->at(index)/pow(looper->GenJetsAK8_ECF2->at(index),2),w);
				}
				else if(vname=="ecfD2"){
					if(looper->GenJetsAK8_ECF2->at(index)>0) value.Fill(looper->GenJetsAK8_ECF3->at(index)/pow(looper->GenJetsAK8_ECF2->at(index),3),w);
				}
				else { //do nothing
				}
			}
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
					if(vname=="mass"){//mass comparison
						if(mtype==MJJ) values[i].Fill(looper->MJJ,w);
						else if(mtype==MT) values[i].Fill(looper->MT,w);
						else if(mtype==Mmc) values[i].Fill(looper->Mmc,w);
						else if(mtype==MAOS) values[i].Fill(looper->MAOS,w);
					}
					else if(vname=="MT"){//transverse mass
						values[i].Fill(looper->MT,w);
					}
					else if(vname=="MJJ"){//dijet mass
						values[i].Fill(looper->MJJ,w);
					}
					else if(vname=="Mmc"){//dijet+truth mass
						values[i].Fill(looper->Mmc,w);
					}
					else if(vname=="njet"){//# of jets
						values[i].Fill(looper->GenJetsAK8->size(),w);
					}
					else if(IsPerJet(vname,values[i],w)){
						//per-jet histos (leading, subleading, or both)
						//nothing to do - histo is filled as a side effect
					}
					else if(vname=="deltaeta"){//deta(j1,j2)
						if(looper->GenJetsAK8->size()>1) values[i].Fill(abs(looper->GenJetsAK8->at(0).Eta()-looper->GenJetsAK8->at(1).Eta()),w);
					}
					else if(vname=="deltaphi1"){//dphi(j1,MET)
						values[i].Fill(looper->DeltaPhi1,w);
					}
					else if(vname=="deltaphi2"){//dphi(j2,MET)
						values[i].Fill(looper->DeltaPhi2,w);
					}
					else if(vname=="deltaphimin"){//min dphi(j1/2,MET)
						values[i].Fill(looper->DeltaPhiMin,w);
					}
					else if(vname=="met"){//missing ET
						values[i].Fill(looper->MET,w);
					}
					else if(vname=="metMTratio"){//MET/MT
						values[i].Fill(looper->MET/looper->MT,w);
					}
					else { //if it's a histogram with no known variable or calculation, do nothing
					}
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
		masstypes mtype;
};
REGISTER_SELECTOR(Histo);

#endif

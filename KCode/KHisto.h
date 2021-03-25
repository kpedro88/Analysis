#ifndef KHISTO_H
#define KHISTO_H

//custom headers
#include "KMap.h"
#include "KChecker.h"
#include "KFactory.h"
#include "KParser.h"
#include "KBase.h"

//ROOT headers
#include <TH1.h>
#include <TH2.h>
#include <TH1F.h>
#include <TProfile.h>

//STL headers
#include <vector>
#include <string>
#include <array>
#include <tuple>

using namespace std;

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

//forward declarations
class KMCWeightSelector;
class KSVJFilterSelector;
class KLepFracFilterSelector;
class KJetPtFilterSelector;
class KHisto;

//---------------------------------------------------------------
//base class for fillers - virtual methods, etc.
class KFiller : public KChecker {
	public:
		//constructors
		KFiller() : KChecker(), khtmp(0) {}
		KFiller(string name_, OptionMap* localOpt_, KHisto* khtmp_) : KChecker(name_, localOpt_), khtmp(khtmp_) {}
		
		//destructor
		virtual ~KFiller() {}
	
		//accessors
		virtual void Fill(KValue& value, double weight) {}
		
	protected:
		//member variables
		KHisto* khtmp;
};
typedef KFactory<KFiller,string,OptionMap*,KHisto*> KFillerFactory;
#define REGISTER_FILLER(a) REGISTER_MACRO2(KFillerFactory,KFiller_##a,a)

//---------------------------------------------------------------
//extension of KFiller for per-jet histos (jet index, flattening)
//todo: allow different jet collections (not just AK8)?
class KJetFiller : public KFiller {
	public:
		//constructors
		KJetFiller() : KFiller() {}
		KJetFiller(string name_, OptionMap* localOpt_, KHisto* khtmp_, vector<unsigned> indices_) : KFiller(name_, localOpt_, khtmp_), indices(indices_) {}
		
		//destructor
		virtual ~KJetFiller() {}
	
		//accessors
		virtual void Fill(KValue& value, double weight){
			double w = weight;
			
			//loop over specified jets
			for(auto index : indices){
				double w = weight;
				w *= GetWeight(index);
				FillPerJet(value,w,index);
			}
		}
		virtual void FillPerJet(KValue& value, double weight, unsigned index) {}
		//defined below KHisto def (uses class method)
		virtual double GetWeight(unsigned index);
		
	protected:
		//member variables
		vector<unsigned> indices;	
};
typedef KFactory<KFiller,string,OptionMap*,KHisto*,vector<unsigned>> KJetFillerFactory;
#define REGISTER_JETFILLER(a) REGISTER_MACRO2(KJetFillerFactory,KJetFiller_##a,a)

//---------------------------------------------------------------
//class to store properties of histograms and associated fillers
class KHisto : public KChecker {
	public:
		//constructors
		KHisto() : KChecker(), isSpecial(false), MCWeight(0) {}
		KHisto(string name_, OptionMap* localOpt_, TH1* htmp_, KBase* base_) : KChecker(name_, localOpt_), htmp(htmp_), isSpecial(false), MCWeight(0) {
			//initialization - checkers
			SetSelection(base_->GetSelection());
			SetBase(base_);

			if(base_->IsExt()) return;

			vector<string> vars;
			if(!localOpt->Get("vars",vars)){
				//split up histo variables from name (if not otherwise specified)
				KParser::process(name,'_',vars);
			}

			if(htmp){
				//make fillers
				vector<unsigned> indices;
				string vname2;
				for(const auto& var: vars){
					if(IsPerJet(var,vname2,indices)) fillers.push_back(KJetFillerFactory::GetFactory().construct(vname2,vname2,localOpt,this,indices));
					else fillers.push_back(KFillerFactory::GetFactory().construct(var,var,localOpt,this));
				}
				
				//finish initializing fillers
				CheckFillers();
			}
			//if htmp is null, assume it is a special histo
			else {
				htmp = GetSpecial();
				isSpecial = true;
			}
		}
		
		//destructor
		virtual ~KHisto() {}
		
		//accessors
		virtual TH1* GetHisto() { return htmp; }
		virtual void Fill(){
			if(fillers.empty()) return; //nothing to do
			
			//weights
			double w = GetWeight();
			
			vector<KValue> values(fillers.size());
			for(unsigned i = 0; i < fillers.size(); ++i){
				fillers[i]->Fill(values[i],w);
			}
			
			//now fill the histogram
			if(fillers.size()==1){
				for(int ix = 0; ix < values[0].GetSize(); ix++){
					htmp->Fill(values[0].GetValue(ix), values[0].GetWeight(ix));
				}
			}
			else if(fillers.size()==2){
				//need to cast in order to use Fill(x,y,w)
				//these three cases allow for various x vs. y comparisons: same # entries per event, or 1 vs. N per event
				if(values[0].GetSize()==values[1].GetSize()) {
					for(int i = 0; i < values[0].GetSize(); i++){
						if(htmp->GetDimension()==1)
							static_cast<TProfile*>(htmp)->Fill(values[0].GetValue(i), values[1].GetValue(i), values[0].GetWeight(i)); //pick the x weight by default
						else if(htmp->GetDimension()==2)
							static_cast<TH2*>(htmp)->Fill(values[0].GetValue(i), values[1].GetValue(i), values[0].GetWeight(i)); //pick the x weight by default
					}
				}
				else if(values[0].GetSize()==1){
					for(int iy = 0; iy < values[1].GetSize(); iy++){
						if(htmp->GetDimension()==1)
							static_cast<TProfile*>(htmp)->Fill(values[0].GetValue(0), values[1].GetValue(iy), values[1].GetWeight(iy));
						else if(htmp->GetDimension()==2)
							static_cast<TH2*>(htmp)->Fill(values[0].GetValue(0), values[1].GetValue(iy), values[1].GetWeight(iy));
					}
				}
				else if(values[1].GetSize()==1){
					for(int ix = 0; ix < values[0].GetSize(); ix++){
						if(htmp->GetDimension()==1)
							static_cast<TProfile*>(htmp)->Fill(values[0].GetValue(ix), values[1].GetValue(0), values[0].GetWeight(ix));
						else if(htmp->GetDimension()==2)
							static_cast<TH2*>(htmp)->Fill(values[0].GetValue(ix), values[1].GetValue(0), values[0].GetWeight(ix));
					}
				}
			}
			else { //no support for other # of vars
			}
		}
		//defined in K(Builder/Skimmer)Selectors.h to avoid circular dependency
		virtual double GetWeight();
		virtual double GetWeightPerJet(unsigned index);
		virtual void Finalize(TH1* haxis=NULL){
			//propagate SetAxisRange to external histos
			//based on upper edge of first and last bins in axis histo (subtract 1 because upper edge is always assigned to next bin)
			if(haxis) {
				auto xaxis = haxis->GetXaxis();
				auto xtmp = htmp->GetXaxis();
				htmp->SetAxisRange(
					xtmp->GetBinCenter(xtmp->FindBin(xaxis->GetBinUpEdge(xaxis->GetFirst()))-1),
					xtmp->GetBinCenter(xtmp->FindBin(xaxis->GetBinUpEdge(xaxis->GetLast()))-1)
				);
			}

			bool overflow = base->GetGlobalOpt()->Get("plotoverflow",false);
			if(overflow){
				if(fillers.size()==2) return; //not implemented for 2D histos or profiles yet

				//handle change in displayed x-axis
				bool overflowall = base->GetGlobalOpt()->Get("plotoverflowall",overflow);
				//temporary histo to calculate error correctly when adding overflow bin to last bin
				TH1* otmp = (TH1*)htmp->Clone();
				otmp->Reset("ICEM");
				int ovbin = htmp->GetNbinsX()+1;
				int ovbin0 = overflowall ? htmp->GetXaxis()->GetLast()+1 : ovbin;
				double err = 0.;
				otmp->SetBinContent(ovbin0-1,htmp->IntegralAndError(ovbin0,ovbin,err));
				otmp->SetBinError(ovbin0-1,err);
				
				//add overflow bin to last bin
				htmp->Add(otmp);
				
				//remove overflow bin(s) from htmp (for consistent integral/yield)
				for(int b = ovbin0; b <= ovbin; ++b){
					htmp->SetBinContent(b,0);
					htmp->SetBinError(b,0);
				}
				
				delete otmp;
			}
		}
		
		//common checkers
		virtual void CheckDeps(){
			//in case of special histo, sel might not be defined
			if(!sel) return;
			MCWeight = sel->Get<KMCWeightSelector*>("MCWeight");
			SVJFilter = sel->Get<KSVJFilterSelector*>("SVJFilter");
			LepFracFilter = sel->Get<KLepFracFilterSelector*>("LepFracFilter");
			JetPtFilter = sel->Get<KJetPtFilterSelector*>("JetPtFilter");
		}
		virtual void CheckBase(){
			//do not use MCWeight with data
			if(base->IsData()) MCWeight = NULL;
		}
		//checkers for fillers
		virtual void CheckFillers() {
			for(auto filler : fillers){
				filler->SetSelection(sel);
				filler->SetBase(base);
			}
		}
		virtual void CheckBranches() {
			for(auto filler : fillers){
				filler->CheckBranches();
			}
		}
		
		//helpers
		bool IsPerJet(const string& vname, string& vname2, vector<unsigned>& indices){
			vector<tuple<string,vector<unsigned>>> jetcases{
				make_tuple("leadjet",vector<unsigned>{0}),
				make_tuple("subleadjet",vector<unsigned>{1}),
				make_tuple("bothjet",vector<unsigned>{0,1}),
				make_tuple("thirdjet",vector<unsigned>{2}),
				make_tuple("fourthjet",vector<unsigned>{3})
			};
			bool foundcase = false;
			for(const auto& jetcase: jetcases){
				const auto& jetstr = get<0>(jetcase);
				if(vname.compare(0,jetstr.size(),jetstr)==0){
					indices = get<1>(jetcase);
					vname2 = vname.substr(jetstr.size(),string::npos);
					foundcase = true;
					break;
				}
			}
			if(!foundcase){
				indices.clear();
				vname2 = "";
			}
			return (!indices.empty());
		}
		
		//deal with special histos
		static bool IsSpecial(const string& s){
			static const array<string,3> specials{"cutflowRaw","cutflowAbs","cutflowRel"};
			for(const auto& special: specials){
				if(s==special) return true;
			}
			return false;
		}
		bool IsSpecial() { return isSpecial; }
		TH1* GetSpecial() {
			if(name=="cutflowRaw") return base->GetCutflow(KCutflow::CutRaw);
			else if(name=="cutflowAbs") return base->GetCutflow(KCutflow::CutAbs);
			else if(name=="cutflowRel") return base->GetCutflow(KCutflow::CutRel);
			else return NULL;
		}

		//member variables needed by fillers
		KMCWeightSelector* MCWeight;
		KSVJFilterSelector* SVJFilter;
		KLepFracFilterSelector* LepFracFilter;
		KJetPtFilterSelector* JetPtFilter;
		
	protected:
		//member variables
		TH1* htmp;
		bool isSpecial;
		vector<KFiller*> fillers;
};

double KJetFiller::GetWeight(unsigned index){
	return khtmp->GetWeightPerJet(index);
}

//avoid circular dependency
//add a blank histo for future building
TH1* KBase::AddHisto(string s, TH1* h, OptionMap* omap){
	//avoid re-adding
	if(!GetHisto(s)){
		stmp = s;
		obj = new KObject();
		if(h){
			obj->htmp = (TH1*)h->Clone();
			obj->htmp->Sumw2();					
		}
		//KHisto will generate special histo automatically (if h==NULL)
		//but don't make KHisto if no omap provided
		if(omap) obj->khtmp = new KHisto(s,omap,obj->htmp,this);
		if(!obj->htmp and obj->khtmp) obj->htmp = obj->khtmp->GetHisto();
		if(poisson) obj->htmp->SetBinErrorOption(TH1::kPoisson);
		MyObjects.Add(stmp,obj);
	}
	return obj->htmp;
}
//in case of normalization to yield or other scaling
void KBase::Normalize(double nn, bool toYield){
	if(obj->khtmp and obj->khtmp->IsSpecial()) return;
	if(obj->htmp->InheritsFrom(TProfile::Class())) return;
	double simyield = obj->htmp->GetDimension()==2 ? ((TH2*)obj->htmp)->Integral(0,obj->htmp->GetNbinsX()+1,0,obj->htmp->GetNbinsY()+1) : obj->htmp->Integral(0,obj->htmp->GetNbinsX()+1);
	if(toYield) obj->htmp->Scale(nn/simyield);
	else obj->htmp->Scale(nn);
}
//histo add so external histos won't get overwritten
TH1* KBaseExt::AddHisto(string s, TH1* h, OptionMap* omap){
	//set current name
	stmp = s;
	
	if(!add_ext){ //if the histo being added is not from ext, check to see if it is already added
		KObject* otmp = MyObjects.Get(s);
		if(otmp and otmp->htmp){ //if it is already added, just use it, do not overwrite it
			//ext has no build step, so just finalize now
			obj = otmp;
			obj->khtmp->Finalize(h);
			return obj->htmp;
		}
	}
	
	//otherwise, set current histo the usual way
	obj = new KObject();
	obj->htmp = (TH1*)h->Clone();
	if(poisson) obj->htmp->SetBinErrorOption(TH1::kPoisson);
	if (useKFactor) obj->htmp->Scale(kfactor);
	obj->khtmp = new KHisto(s,omap,obj->htmp,this);
	MyObjects.Add(stmp,obj);
	if(debug){
		cout << "base ext " << name << ": added histo " << s << endl;
		obj->htmp->Print();
	}
	return obj->htmp;
}
#endif

#ifndef LEPTONCORRECTOR_H
#define LEPTONCORRECTOR_H

#include "Helper.h"

#include "TFile.h"
#include "TH2F.h"
#include "TAxis.h"
#include "TLorentzVector.h"

#include <string>
#include <cmath>
#include <array>
#include <vector>
#include <exception>

using namespace std;

class LeptonCorrector {
	public:
		//types
		enum class LCtype { id = 0, iso = 1, trk = 2 };
		enum class LCaxes { pt = 0, eta = 1, abseta = 2 };
		//helper class
		class AxisQty {
			public:
				//constructor
				AxisQty() : axis(NULL), atype(LCaxes::pt) {}
				AxisQty(TAxis* axis_, LCaxes atype_) : axis(axis_), atype(atype_) { }
				//accessor
				int GetBin(const TLorentzVector& vec) const {
					double qty = 0;
					if(atype==LCaxes::pt) qty = vec.Pt();
					else if(atype==LCaxes::eta) qty = vec.Eta();
					else if(atype==LCaxes::abseta) qty = abs(vec.Eta());
					
					qty = min(max(qty,axis->GetXmin()+0.01),axis->GetXmax()-0.01);
					return axis->FindBin(qty);
				}
				//members
				TAxis* axis;
				LCaxes atype;
		};
		//constructor
		LeptonCorrector() : hist(NULL), minpt(-1), maxpt(-1) {}
		LeptonCorrector(LCtype ltype_, string fname_, string hname_, array<LCaxes,2> axes_, int unc_, double syst_=0., double minpt_=-1, double maxpt_=-1)
			: hist(NULL), ltype(ltype_), unc(unc_), syst2(syst_*syst_), minpt(minpt_), maxpt(maxpt_)
		{
			TFile* file = helper::Open(fname_);
			hist = helper::Get<TH2F>(file,hname_);
			hist->SetDirectory(0);
			file->Close();
			
			for(unsigned a = 0; a < axes_.size(); ++a){
				TAxis* axtmp;
				if(a==0) axtmp = hist->GetXaxis();
				else if (a==1) axtmp = hist->GetYaxis();				
				axes[a] = AxisQty(axtmp,axes_[a]);
			}
		}
		
		//accessor
		double GetSF(const TLorentzVector& vec) const {
			//check bounds
			if(minpt > 0 and vec.Pt()<minpt) return 1.;
			if(maxpt > 0 and vec.Pt()>=maxpt) return 1.;
			
			array<int,2> bins;
			for(unsigned a = 0; a < axes.size(); ++a){
				bins[a] = axes[a].GetBin(vec);
			}
			
			double sf = hist->GetBinContent(bins[0],bins[1]);
			double sfunc = 0.;
			if(unc!=0){
				sfunc = hist->GetBinError(bins[0],bins[1]);
				if(syst2>0) sfunc = sqrt(sfunc*sfunc+syst2);
				if(unc<0) sfunc *= -1;
			}
			return sf + sfunc;
		}
		
		//helper
		static vector<TLorentzVector> MatchGenRec(const vector<TLorentzVector>& gen, double gen_pt_min, double gen_eta_max, const vector<TLorentzVector>& rec, const vector<vector<bool>>& rec_masks, double delta_pt_max, double delta_r_max){
			vector<TLorentzVector> gen_acc;
			for(const auto& vgen : gen){
				if(vgen.Pt() > gen_pt_min and abs(vgen.Eta()) < gen_eta_max) gen_acc.push_back(vgen);
			}
			if(rec.empty()) return gen_acc;
			
			vector<TLorentzVector> matched;
			for(unsigned r = 0; r < rec.size(); ++r){
				bool rec_acc = true;
				for(const auto& mask : rec_masks){
					rec_acc &= mask[r];
				}
				if(!rec_acc) continue;
				const auto& vrec = rec[r];
				for(const auto& vgen: gen_acc){
					if(abs(vgen.Pt()-vrec.Pt())/vgen.Pt() < delta_pt_max and vgen.DeltaR(vrec) < delta_r_max) matched.push_back(vgen);
				}
			}
			
			return matched;
		}
		
		//members
		TH2* hist;
		LCtype ltype;
		array<AxisQty,2> axes;
		int unc;
		double syst2;
		double minpt, maxpt;
};

#endif

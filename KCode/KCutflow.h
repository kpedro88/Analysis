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
#include <set>

using namespace std;

//forward declaration
class KCutflow;

//enum
enum class KCutflowType { CutRaw=0, CutAbs=1, CutRel=2 };

//keeps track of raw values and parent (direct antecedent), base (earliest antecedent)
class KCutflowItem {
	public:
		//constructor
		KCutflowItem(string name_, double raw_, double rawE_, KCutflow* map_, bool weighted=false, string baseName_="", string parentName_="");
		//dummy constructor for finding in set
		KCutflowItem(string name_) : name(name_) {}

		//accessors
		const string& GetName() const { return name; }
		pair<double,double> GetVal(KCutflowType ct) const {
			return
				ct==KCutflowType::CutRaw ? make_pair((double)raw,rawE) : (
				ct==KCutflowType::CutAbs ? make_pair(abs,absE) : (
				ct==KCutflowType::CutRel ? make_pair(rel,relE) :
				make_pair(0.,0.) ));
		}
		unsigned long long GetRaw() const { return raw; }

		//for sorting
		bool operator<(const KCutflowItem& rhs) const { return name < rhs.name; }

	protected:
		//members
		string name, parentName, baseName;
		unsigned long long raw;
		double rawD;
		double rawE, abs, absE, rel, relE;
		const KCutflowItem* parent = nullptr;
		const KCutflowItem* base = nullptr;
};

//ostensibly, to test if a histogram is weighted: h->GetSumw2()->GetSum()!=h->GetSumOfWeights()
//however, cutflow histograms created in KSelection have bin errors set to Poisson manually, so this fails
//therefore, just specify whether to treat histograms as weighted when creating cutflow object
class KCutflow {
	public:
		//constructors
		KCutflow(string name_, string fname_, string suff_="", bool weighted_=false) : KCutflow(name_, KOpen(fname_), suff_, weighted_) {}
		KCutflow(string name_, TFile* file, string suff_="", bool weighted_=false) : KCutflow(name_, KGet<TH1F>(file,"cutflow"+(suff_.empty() ? "" : "_"+suff_)), KGet<TH1F>(file,nEventProc()+(suff_.empty() ? "" : "_"+suff_)), weighted_) {}
		//get nevent info from histo
		KCutflow(string name_, TH1F* h_tmp, TH1F* h_ntmp, bool weighted_=false) : KCutflow(name_, h_tmp, h_ntmp->GetBinContent(1), h_ntmp->GetBinError(1), weighted_) {}
		//get nevent info directly
		KCutflow(string name_, TH1F* h_tmp, unsigned long long nentries_=0, double nentriesE_=0, bool weighted_=false) : name(name_), title(h_tmp->GetTitle()), weighted(weighted_) {
			string baseName = "";
			if(nentries_>0) {
				baseName = nEventProc();
				AddItem(baseName,nentries_,nentriesE_);
			}
			for(int c = 1; c <= h_tmp->GetNbinsX(); c++){
				string parentName(c==1 ? baseName : h_tmp->GetXaxis()->GetBinLabel(c-1));
				AddItem(h_tmp->GetXaxis()->GetBinLabel(c), h_tmp->GetBinContent(c), h_tmp->GetBinError(c), baseName, parentName);
			}
		}
		//destructor
		virtual ~KCutflow() { }
		
		//accessors
		//todo: expand for object sync selectors
		
		//print efficiencies
		void PrintEfficiency(bool printerrors=false, int minprec=0){
			if(itemList.empty()) return;

			//loop to get width of selector name
			vector<unsigned> widths(6,0);
			unsigned width1s = 10;
			for(auto ptr : itemList){
				width1s = max(width1s, (unsigned)ptr->GetName().size());
			}
			
			//setup widths
			widths[0] = width1s;
			if(printerrors) {
				widths[4] = max(log10(itemList[0]->GetVal(KCutflowType::CutRaw).first)+1,log10(itemList[0]->GetVal(KCutflowType::CutRaw).second)+1+3); //extra width for num and err, based on # digits
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

			//print selectors
			unsigned prcsn = cout.precision();
			for(auto ptr : itemList){
				const auto& item(*ptr);
				cout << left << setw(widths[0]) << item.GetName();
				auto raw = item.GetRaw();
				auto rawE = item.GetVal(KCutflowType::CutRaw).second;
				auto abs = item.GetVal(KCutflowType::CutAbs);
				auto rel = item.GetVal(KCutflowType::CutRel);

				//handle minimum precision case
				//todo: apply to errors, make sure widths are sufficient
				const auto& s_raw = PrintVal(raw, prcsn, 0); //do not use minprec for integer types (fixed format won't append .0)
				const auto& s_abs = PrintVal(abs.first, prcsn, minprec);
				const auto& s_rel = PrintVal(rel.first, prcsn, minprec);

				if(printerrors){
					cout << "  " << right << setw(widths[4]) << s_raw << " +/- " << right << setw(widths[4]) << rawE;
					if(item.GetName()!=nEventProc()) {
						cout << "  " << right << setw(widths[5]) << s_abs << " +/- " << right << setw(widths[5]) << abs.second;
						//rel. eff. = abs. eff. for first selector
						cout << "  " << right << setw(widths[5]) << s_rel << " +/- " << right << setw(widths[5]) << rel.second;
					}
				}
				else {
					cout << "  " << right << setw(widths[1]) << s_raw;
					if(item.GetName()!=nEventProc()) {
						cout << "  " << right << setw(widths[2]) << s_abs;
						//rel. eff. = abs. eff. for first selector
						cout << "  " << right << setw(widths[3]) << s_rel;
					}
				}
				cout << endl;
			}
		}
		//get an efficiency histogram
		TH1F* GetEfficiency(KCutflowType ct=KCutflowType::CutRaw, bool cutflownorm=false){
			if(ct==KCutflowType::CutRaw) cutflownorm = false;
			string name(
				ct==KCutflowType::CutRaw ? "cutflow" :
				ct==KCutflowType::CutAbs ? "cutflowAbs" :
				ct==KCutflowType::CutRel ? "cutflowRel" :
				""
			);
			if(name.empty()) return nullptr;
			TH1F* h_out = new TH1F(name.c_str(),"",itemList.size(),0,itemList.size());
			int c = 1;
			for(auto ptr : itemList){
				const auto& item(*ptr);
				h_out->GetXaxis()->SetBinLabel(c,item.GetName().c_str());
				h_out->SetBinContent(c,item.GetVal(ct).first);
				h_out->SetBinError(c,item.GetVal(ct).second);
			}
			h_out->GetXaxis()->SetNoAlphanumeric();
			if(cutflownorm) h_out->Scale(100./h_out->GetBinContent(1));
			return h_out;
		}

		//add more items
		void AddItem(string name_, double raw_, double rawE_, string baseName_="", string parentName_="") {
			auto iter_succ = itemMap.emplace(name_,raw_,rawE_,this,weighted,baseName_,parentName_);
			if(!iter_succ.second) throw std::runtime_error("Duplicate item in cutflow map: "+name_);
			itemList.push_back(&(*iter_succ.first));
		}

		//more accessors
		const vector<const KCutflowItem*>& GetList() const { return itemList; }
		const KCutflowItem* GetItem(const string& name) const {
			auto iter = itemMap.find(name);
			if(iter!=itemMap.end()) return &(*iter);
			else {
				throw runtime_error("Could not find item: "+name);
				return nullptr;
			}
		}

		//helper
		static bool IsWeighted(TH1* h) { return h->GetSumw2()->GetSum()!=h->GetSumOfWeights(); }
		//easier than static member
		static const string& nEventProc() {
			static string s("nEventProc");
			return s;
		}
		template <typename T>
		string PrintVal(T val, unsigned prcsn, int minprec=0){
			stringstream ss_val;
			unsigned new_prcsn = prcsn;
			bool has_enough = false;
			while(!has_enough){
				ss_val.str("");
				ss_val << fixed << setprecision(new_prcsn);
				ss_val << val;
				if(minprec==0 or val==(T)0) break; //no need for anything further in these cases
				auto tmp = ss_val.str();
				size_t leading_chars = 0;
				// remove 0.000 from 0.000x (stop at first non-zero char)
				for(unsigned i = 0; i < tmp.size(); ++i){
					if(tmp[i]=='0' or tmp[i]=='.') ++leading_chars;
					else break;
				}
				tmp = tmp.substr(leading_chars,string::npos);
				has_enough = tmp.size()>=minprec;
				if(!has_enough) ++new_prcsn;
			}
			return ss_val.str();
		}

	protected:
		//members
		string name, title;
		bool weighted;
		//sorted set (for search) and vector of pointers (for ordering)
		set<KCutflowItem> itemMap;
		vector<const KCutflowItem*> itemList;
};

KCutflowItem::KCutflowItem(string name_, double raw_, double rawE_, KCutflow* map, bool weighted, string baseName_, string parentName_) :
	name(name_), parentName(parentName_), baseName(baseName_), raw(raw_), rawD(raw_), rawE(rawE_) {
	if((!parentName.empty() or !baseName.empty()) and !map) {
		throw runtime_error("No item map provided");
	}
	if(!parentName.empty()){
		parent = map->GetItem(parentName);
	}
	if(!baseName.empty()){
		base = map->GetItem(baseName);
	}

	//do calculations using doubles (more accurate for weighted samples)
	double nentries = base ? base->rawD : 1;
	double prev = parent ? parent->rawD : 1;
	double nentriesE = base ? base->rawE : 1;
	double prevE = parent ? parent->rawE : 1;
	abs = (rawD/nentries)*100;
	absE = weighted ? KMath::EffErrorWeighted(rawD,rawE,nentries,nentriesE)*100 : KMath::EffError(raw,nentries)*100;
	rel = (rawD/prev)*100;
	relE = weighted ? KMath::EffErrorWeighted(rawD,rawE,prev,prevE)*100 : KMath::EffError(raw,prev)*100;
}

#endif

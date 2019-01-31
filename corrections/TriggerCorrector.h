#ifndef TRIGGERCORRECTOR_H
#define TRIGGERCORRECTOR_H

#include <TFile.h>
#include <TEfficiency.h>

#include <string>
#include <vector>
#include <iostream>
#include <cmath>

class TriggerCorrector {
	public:
		//constructor
		TriggerCorrector() : eff_2016(NULL), eff_2017(NULL), eff_2018(NULL), debug(false) {}
		//destructor
		virtual ~TriggerCorrector() {}

		//setup
		void SetEffs(std::string fname, std::vector<std::string> effnames){
			TFile* file = TFile::Open(fname.c_str());
			eff_2016 = (TEfficiency*)file->Get(effnames[0].c_str());
			eff_2016->SetDirectory(0);
			eff_2017 = (TEfficiency*)file->Get(effnames[1].c_str());
			eff_2017->SetDirectory(0);
			eff_2018 = (TEfficiency*)file->Get(effnames[2].c_str());
			eff_2018->SetDirectory(0);
			file->Close();
		}

		//function
		double GetCorrection(int year, double var, int unc=0){
			auto eff = year==2016 ? eff_2016 : year==2017 ? eff_2017 : year==2018? eff_2018 : NULL;
			int bin = eff->FindFixBin(var);
			//underflow/overflow protection
			bin = std::min(std::max(1,bin),eff->GetTotalHistogram()->GetNbinsX());
			double val = eff->GetEfficiency(bin);
			if(unc>0) val += eff->GetEfficiencyErrorUp(bin);
			else if(unc<0) val -= eff->GetEfficiencyErrorLow(bin);
			if(debug) std::cout << "corr = " << val << " (var = " << var << ", bin = " << bin << ", unc = " << unc << ")" << std::endl;
			return val;
		}

		//members
		TEfficiency *eff_2016, *eff_2017, *eff_2018;
		bool debug;
};

#endif

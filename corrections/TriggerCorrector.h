#ifndef TRIGGERCORRECTOR_H
#define TRIGGERCORRECTOR_H

#include "Helper.h"

#include <TFile.h>
#include <TEfficiency.h>

#include <string>
#include <vector>
#include <iostream>
#include <cmath>

class TriggerCorrector {
	public:
		//constructor
		TriggerCorrector() : eff(NULL), debug(false) {}
		//destructor
		virtual ~TriggerCorrector() {}

		//setup
		void SetEff(std::string fname, std::string effname){
			TFile* file = helper::Open(fname);
			eff = helper::Get<TEfficiency>(file,effname);
			eff->SetDirectory(0);
			file->Close();
		}

		//function
		double GetCorrection(double var, int unc=0){
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
		TEfficiency *eff;
		bool debug;
};

#endif

#ifndef TRIGGERFUNCCORRECTOR_H
#define TRIGGERFUNCCORRECTOR_H

#include "TFile.h"
#include "TF1.h"
#include "TFitResult.h"
#include "Math/WrappedMultiTF1.h"

#include <string>
#include <vector>
#include <iostream>
#include <cmath>

class KFitResult : public TFitResult {
	public:
		using TFitResult::TFitResult;
		void ResetModelFunction(TF1* func){
			this->SetModelFunction(std::shared_ptr<IModelFunction>(dynamic_cast<IModelFunction*>(ROOT::Math::WrappedMultiTF1(*func).Clone())));
		}
};

class TriggerFuncCorrector {
	public:
		//constructor
		TriggerFuncCorrector() : func(NULL), err(NULL), debug(false) {}
		//destructor
		virtual ~TriggerFuncCorrector() {}

		//setup
		void SetFunc(std::string fname, std::string funcname, std::string errname){
			TFile* file = TFile::Open(fname.c_str());
			func = (TF1*)file->Get(funcname.c_str());
			err = (KFitResult*)file->Get(errname.c_str());
			//restore base function to compute error (discarded by serialization)
			err->ResetModelFunction(func);
//			file->Close();
		}

		//function
		double GetCorrection(double var, int unc=0){
			//bounds
			if(var > func->GetXmax()) var = func->GetXmax();
			if(var < func->GetXmin()) var = func->GetXmin();

			double val = func->Eval(var);
			double valerr = 0.0;

			if(unc!=0){
				double x[1] = {var};
				double xe[1] = {0.0};
				//propagate fit parameter errors to value
				err->GetConfidenceIntervals(1,1,1,x,xe,0.683,false);
				valerr = xe[0];
				if(unc>0) val += valerr;
				else if(unc<0) val -= valerr;
			}

			if(debug) std::cout << "corr = " << val << " (var = " << var << ", unc = " << unc << ", err = " << valerr << ")" << std::endl;
			return val;
		}

		//members
		TF1* func;
		KFitResult* err;
		bool debug;
};

#endif

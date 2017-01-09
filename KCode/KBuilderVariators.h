#ifndef KBUILDERVARIATORS_H
#define KBUILDERVARIATORS_H

//custom headers
#include "KVariation.h"
#include "KBuilder.h"

//ROOT headers
#include <TLorentzVector.h>

//STL headers
#include <string>
#include <vector>

using namespace std;

//base class for variators is in KVariation.h

namespace KParser {
	KVariator* processVariator(KNamed* tmp){
		KVariator* vtmp = 0;
		string vname = tmp->fields[0];
		//OptionMap* omap = tmp->second; //unused right now
		
		//check for all known variators
		//skip unknown variators

		if(!vtmp) cout << "Input error: unknown variator " << vname << ". This variator will be skipped." << endl;

		return vtmp;
	}
}

#endif

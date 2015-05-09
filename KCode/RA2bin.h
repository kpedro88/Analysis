#ifndef RA2BIN_H
#define RA2BIN_H

//STL headers
#include <vector>
#include <map>
#include <iostream>

using namespace std;

//---------------------------------------------------------------
//class to build RA2 bin IDs
class RA2binID {
	public:
		//constructors
		RA2binID() : id(0) {}
		RA2binID(unsigned id_) : id(id_) {}
		RA2binID(unsigned NJetBin, unsigned NBJetBin, unsigned MHTBin, unsigned HTBin) : 
			id(NJetBin + 100*NBJetBin + 10000*MHTBin + 1000000*HTBin) {}
		//destructor
		~RA2binID() {}
		
		//accessors
		unsigned raw() { return id; }
		unsigned NJetBin() { return id%100; }
		unsigned NBJetBin() { return (id/100)%100; }
		unsigned MHTBin() { return (id/10000)%100; }
		unsigned HTBin() { return (id/1000000)%100; }
	
	private:
		//member variables
		unsigned id;
};

//---------------------------------------------------------------
//class to store and apply RA2 binning
class RA2binner {
	public:
		typedef map<unsigned, unsigned>::iterator BNit;
	
		//constructors
		RA2binner() {}
		RA2binner(map<unsigned, unsigned> IDtoBinNumber_, vector<int> NJetMin_, vector<int> NJetMax_, vector<int> NBJetMin_, vector<int> NBJetMax_, vector<float> MHTMin_, vector<float> MHTMax_, vector<float> HTMin_, vector<float> HTMax_) :
			IDtoBinNumber(IDtoBinNumber_), NJetMin(NJetMin_), NJetMax(NJetMax_), NBJetMin(NBJetMin_), NBJetMax(NBJetMax_), MHTMin(MHTMin_), MHTMax(MHTMax_), HTMin(HTMin_), HTMax(HTMax_) {}
		//destructor
		~RA2binner() {}
		
		//functions
		unsigned GetBinNumber(int& NJetVal, int& NBJetVal, float& MHTVal, float& HTVal) {
			vector<unsigned> NJetBins = GetNJetBins(NJetVal);
			vector<unsigned> NBJetBins = GetNBJetBins(NBJetVal);
			vector<unsigned> MHTBins = GetMHTBins(MHTVal);
			vector<unsigned> HTBins = GetHTBins(HTVal);
			
			//skip loop if no bin was found for a value
			if(NJetBins.size()==0 || NBJetBins.size()==0 || MHTBins.size()==0 || HTBins.size()==0) return 0;
			
			//find the correct combination of bin numbers that exists in the map
			for(unsigned nj = 0; nj < NJetBins.size(); nj++){
				for(unsigned nb = 0; nb < NBJetBins.size(); nb++){
					for(unsigned nm = 0; nm < MHTBins.size(); nm++){
						for(unsigned nh = 0; nh < HTBins.size(); nh++){
							unsigned raw = RA2binID(NJetBins[nm],NBJetBins[nb],MHTBins[nm],HTBins[nh]).raw();
							BNit it = IDtoBinNumber.find(raw);
							//exit loop as soon as an existing bin is found
							if(it != IDtoBinNumber.end()){
								return it->second;
							}
						}
					}
				}
			}
			
			//if no bins are were found, return default
			return 0;
		}
		vector<unsigned> GetNJetBins(int& NJetVal){
			vector<unsigned> bins;
			for(unsigned n = 0; n < NJetMin.size(); ++n){
				if(NJetVal >= NJetMin[n] && NJetVal <= NJetMax[n]) bins.push_back(n);
			}
			return bins;
		}
		vector<unsigned> GetNBJetBins(int& NBJetVal){
			vector<unsigned> bins;
			for(unsigned n = 0; n < NBJetMin.size(); ++n){
				if(NBJetVal >= NBJetMin[n] && NBJetVal <= NBJetMax[n]) bins.push_back(n);
			}
			return bins;
		}
		vector<unsigned> GetMHTBins(float& MHTVal){
			vector<unsigned> bins;
			for(unsigned n = 0; n < MHTMin.size(); ++n){
				if(MHTVal > MHTMin[n] && MHTVal <= MHTMax[n]) bins.push_back(n);
			}
			return bins;
		}
		vector<unsigned> GetHTBins(float& HTVal){
			vector<unsigned> bins;
			for(unsigned n = 0; n < HTMin.size(); ++n){
				if(HTVal > HTMin[n] && HTVal <= HTMax[n]) bins.push_back(n);
			}
			return bins;
		}
		
	private:
		//member variables
		map<unsigned, unsigned> IDtoBinNumber;
		vector<int> NJetMin, NJetMax, NBJetMin, NBJetMax;
		vector<float> MHTMin, MHTMax, HTMin, HTMax;
};


#endif
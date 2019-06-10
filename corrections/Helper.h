#ifndef HELPER_H
#define HELPER_H

//ROOT headers
#include <TFile.h>

//STL headers
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <exception>

//general helpers
namespace helper {
	//generalization for processing a line
	void process(std::string line, char delim, std::vector<std::string>& fields){
		std::stringstream ss(line);
		std::string field;
		while(std::getline(ss,field,delim)){
			fields.push_back(field);
		}
	}

	TFile* Open(std::string fname, string mode="READ", bool dothrow=true){
		TFile* file = TFile::Open(fname.c_str(), mode.c_str());
		if(!file and dothrow){
			throw std::runtime_error("Could not open file "+fname);
		}
		return file;
	}

	template <class T>
	T* Get(TFile* file, std::string getname, bool dothrow=true){
		T* obj = static_cast<T*>(file->Get(getname.c_str()));
		if(!obj and dothrow){
			throw std::runtime_error("Could not get "+getname+" from file "+file->GetName());
		}
		return obj;
	}
}

#endif

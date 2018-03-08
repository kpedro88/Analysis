#ifndef KVARIATION_H
#define KVARIATION_H

//custom headers
#include "KMap.h"
#include "KChecker.h"
#include "KLooper.h"
#include "KBase.h"

//ROOT headers

//STL headers
#include <string>
#include <vector>

using namespace std;

//----------------------------------------------------------------
//helper classes to keep track of linked variables
template <class T>
class KBranchT {
	public:
		KBranchT(T* value_, string name_, bool status_=true) :
			value(value_), name(name_), status(status_)
		{}
		virtual ~KBranchT() {}
		
		void Check(TTree* fChain) {
			if(!fChain->GetBranchStatus(name.c_str()) || !fChain->GetBranch(name.c_str())) status = false;
		}
		void Enable(TTree* fChain) {
			fChain->SetBranchStatus(name.c_str(),1);
		}
		
		T* value;
		string name;
		bool status;
};
typedef KBranchT<bool> KBranchB;
typedef KBranchT<int> KBranchI;
typedef KBranchT<double> KBranchD;
typedef KBranchT<TLorentzVector> KBranchL;
typedef KBranchT<vector<bool>> KBranchVB;
typedef KBranchT<vector<int>> KBranchVI;
typedef KBranchT<vector<double>> KBranchVD;
typedef KBranchT<vector<TLorentzVector>> KBranchVL;
typedef KBranchT<vector<vector<TLorentzVector>>> KBranchVVL;

class KLinkedBranchBase {
	public:
		KLinkedBranchBase() {}
		virtual ~KLinkedBranchBase() {}
		
		//virtual accessors
		virtual void Enable(TTree* fChain, int which=-1) {}
		virtual void Check(TTree* fChain, int which=-1) {}
		virtual void Store() {}
		virtual void Vary() {}
		virtual void Restore() {}
};

template <class T>
class KLinkedBranchT : public KLinkedBranchBase {
	public:
		KLinkedBranchT(KBranchT<T> branch0_, KBranchT<T> branch1_) : branch0(branch0_), branch1(branch1_), good0(true), good1(true) {
		}
		~KLinkedBranchT() {}

		void Enable(TTree* fChain, int which=-1){
			if(fChain){
				if(which==-1 or which==0) branch0.Enable(fChain);
				if(which==-1 or which==1) branch1.Enable(fChain);
			}
		}
		void Check(TTree* fChain, int which=-1){
			if(fChain){
				if(which==-1 or which==0) branch0.Check(fChain);
				if(which==-1 or which==1) branch1.Check(fChain);
			}
			good0 = (branch0.value and branch0.status);
			good1 = (branch1.value and branch1.status);
		}
		void Store() {
			if(!good0) return;
			original = *(branch0.value);
		}
		void Vary() {
			if(!good0 or !good1) return;
			*(branch0.value) = *(branch1.value);
		}
		void Restore() {
			if(!good0) return;
			*(branch0.value) = original;
		}
		
	protected:
		KBranchT<T> branch0, branch1;
		T original;
		bool good0, good1;
};
typedef KLinkedBranchT<bool> KLinkedBranchB;
typedef KLinkedBranchT<int> KLinkedBranchI;
typedef KLinkedBranchT<double> KLinkedBranchD;
typedef KLinkedBranchT<TLorentzVector> KLinkedBranchL;
typedef KLinkedBranchT<vector<bool>> KLinkedBranchVB;
typedef KLinkedBranchT<vector<int>> KLinkedBranchVI;
typedef KLinkedBranchT<vector<double>> KLinkedBranchVD;
typedef KLinkedBranchT<vector<TLorentzVector>> KLinkedBranchVL;
typedef KLinkedBranchT<vector<vector<TLorentzVector>>> KLinkedBranchVVL;

//----------------------------------------------------------------
//base class for Variators, has standard functions defined
class KVariator : public KChecker {
	public:
		//constructor
		KVariator() : KChecker() {}
		KVariator(string name_, OptionMap* localOpt_) : KChecker(name_, localOpt_) {}
		//destructor
		virtual ~KVariator(){
			for(auto& branch : branches){
				delete branch;
			}
		}
		//functions
		virtual void DoVariation() {}
		virtual void UndoVariation() {}
		
	protected:
		//member variables
		vector<KLinkedBranchBase*> branches; //in case used
};
typedef KFactory<KVariator,string,OptionMap*> KVariatorFactory;
#define REGISTER_VARIATOR(a) REGISTER_MACRO2(KVariatorFactory,K##a##Variator,a)
#define REGISTER_VARIATOR2(a,b) REGISTER_MACRO2(KVariatorFactory,K##a##Variator,b)

//----------------------------------------------------------------
//class to keep track of a list of Variators
class KVariation {
	public:
		//constructor
		KVariation() : name(""), looper(0) {}
		KVariation(string name_) : name(name_), looper(0) {}
		//destructor
		virtual ~KVariation() {}
		//accessors
		string GetName() { return name; }
		void AddVariator(KVariator* var_){
			variatorList.push_back(var_);
		}
		void SetBase(KBase* base_){
			base = base_;
			looper = base->GetLooper();
			for(unsigned v = 0; v < variatorList.size(); v++){
				variatorList[v]->SetBase(base_);
			}
		}
		void CheckBranches() { 
			for(unsigned v = 0; v < variatorList.size(); v++){
				variatorList[v]->CheckBranches();
			}
		}
		void DoVariation() {
			for(unsigned v = 0; v < variatorList.size(); v++){
				variatorList[v]->DoVariation();
			}
		}
		void UndoVariation() {
			for(unsigned v = 0; v < variatorList.size(); v++){
				variatorList[v]->UndoVariation();
			}
		}
		
	protected:
		//member variables
		string name;
		KLooper* looper;
		vector<KVariator*> variatorList;
		KBase* base;
};

//-------------------------------------------------------------
//addition to KParser to create variators
namespace KParser {
	KVariator* processVariator(KNamed* tmp){
		string vname = tmp->fields[0];
		OptionMap* omap = tmp->localOpt();
		
		KVariator* vtmp = KVariatorFactory::GetFactory().construct(vname,vname,omap);
		
		if(!vtmp) cout << "Input error: unknown variator " << vname << ". This variator will be skipped." << endl;

		return vtmp;
	}
}

#endif

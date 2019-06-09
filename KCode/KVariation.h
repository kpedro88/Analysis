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
		KBranchT(T* value_, string name_="", bool status_=true) : name(name_), status(status_), value(value_) {}
		virtual ~KBranchT() {}
		
		void Check(TTree* fChain) {
			if(!name.empty() and (!fChain->GetBranchStatus(name.c_str()) || !fChain->GetBranch(name.c_str()))) status = false;
		}
		void Enable(TTree* fChain) {
			if(!name.empty()) fChain->SetBranchStatus(name.c_str(),1);
		}
		virtual void Copy(const KBranchT<T>& other){
			*(value) = *(other.value);
		}
		
		T* value;
		string name;
		bool status;
};
typedef KBranchT<bool> KBranchB;
typedef KBranchT<int> KBranchI;
typedef KBranchT<double> KBranchD;
typedef KBranchT<TLorentzVector> KBranchL;

//derived version for pointers
//(for a pointer branch, ROOT changes the value of the pointer)
template <class T>
class KBranchPtrT : public KBranchT<T> {
	public:
		using KBranchT<T>::KBranchT;
		void Copy(const KBranchPtrT<T>& other){
			*(*(this->value)) = *(*(other.value));
		}
};
typedef KBranchPtrT<vector<bool>*> KBranchVB;
typedef KBranchPtrT<vector<int>*> KBranchVI;
typedef KBranchPtrT<vector<double>*> KBranchVD;
typedef KBranchPtrT<vector<TLorentzVector>*> KBranchVL;
typedef KBranchPtrT<vector<vector<bool>>*> KBranchVVB;
typedef KBranchPtrT<vector<vector<int>>*> KBranchVVI;
typedef KBranchPtrT<vector<vector<double>>*> KBranchVVD;
typedef KBranchPtrT<vector<vector<TLorentzVector>>*> KBranchVVL;

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

template <class T, class B = KBranchT<T>>
class KLinkedBranchT : public KLinkedBranchBase {
	public:
		KLinkedBranchT(B branch0_, B branch1_) : KLinkedBranchBase(), branch0(branch0_), branch1(branch1_), branchOrig(&original), good0(true), good1(true) {
		}
		virtual ~KLinkedBranchT() {}

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
			branchOrig.Copy(branch0);
		}
		void Vary() {
			if(!good0 or !good1) return;
			branch0.Copy(branch1);
		}
		void Restore() {
			if(!good0) return;
			branch0.Copy(branchOrig);
		}
		
	protected:
		B branch0, branch1, branchOrig;
		T original;
		bool good0, good1;
};
typedef KLinkedBranchT<bool> KLinkedBranchB;
typedef KLinkedBranchT<int> KLinkedBranchI;
typedef KLinkedBranchT<double> KLinkedBranchD;
typedef KLinkedBranchT<TLorentzVector> KLinkedBranchL;

//derived version for pointers
template <class T>
class KLinkedBranchPtrT : public KLinkedBranchT<T*,KBranchPtrT<T*>> {
	public:
		KLinkedBranchPtrT(KBranchPtrT<T*> branch0_, KBranchPtrT<T*> branch1_) : KLinkedBranchT<T*,KBranchPtrT<T*>>(branch0_,branch1_) {
			this->original = new T();
		}
		virtual ~KLinkedBranchPtrT() {
			delete this->original;
		}
};
typedef KLinkedBranchPtrT<vector<bool>> KLinkedBranchVB;
typedef KLinkedBranchPtrT<vector<int>> KLinkedBranchVI;
typedef KLinkedBranchPtrT<vector<double>> KLinkedBranchVD;
typedef KLinkedBranchPtrT<vector<TLorentzVector>> KLinkedBranchVL;
typedef KLinkedBranchPtrT<vector<vector<bool>>> KLinkedBranchVVB;
typedef KLinkedBranchPtrT<vector<vector<int>>> KLinkedBranchVVI;
typedef KLinkedBranchPtrT<vector<vector<double>>> KLinkedBranchVVD;
typedef KLinkedBranchPtrT<vector<vector<TLorentzVector>>> KLinkedBranchVVL;

//only works with pointers to vectors
template <class T>
class KReorderedBranchT : public KLinkedBranchBase {
	public:
		KReorderedBranchT(KBranchPtrT<T*> branch_, const vector<unsigned>& order_) : KLinkedBranchBase(), original(new T()), varied(new T()), branch(branch_), branchOrig(&original), branchVaried(&varied), order(order_), good(true) {
		}
		~KReorderedBranchT() {
			delete original;
			delete varied;			
		}

		void Enable(TTree* fChain, int which=-1){
			if(fChain){
				if(which==-1 or which==0) branch.Enable(fChain);
			}
		}
		void Check(TTree* fChain, int which=-1){
			if(fChain){
				if(which==-1 or which==0) branch.Check(fChain);
			}
			good = (branch.value and branch.status);
		}
		void Store() {
			if(!good) return;
			branchOrig.Copy(branch);
		}
		void Vary() {
			if(!good) return;
			varied->clear();
			varied->reserve(std::min(original->size(),order.size()));
			for(unsigned io = 0; io < order.size(); ++io){
				varied->push_back((*original)[order[io]]);
			}
			branch.Copy(branchVaried);
		}
		void Restore() {
			if(!good) return;
			branch.Copy(branchOrig);
		}
		
	protected:
		T *original, *varied;
		KBranchPtrT<T*> branch, branchOrig, branchVaried;
		const vector<unsigned>& order;
		bool good;
};
//pointless for scalar types
typedef KReorderedBranchT<vector<bool>> KReorderedBranchVB;
typedef KReorderedBranchT<vector<int>> KReorderedBranchVI;
typedef KReorderedBranchT<vector<double>> KReorderedBranchVD;
typedef KReorderedBranchT<vector<TLorentzVector>> KReorderedBranchVL;
typedef KReorderedBranchT<vector<vector<bool>>> KReorderedBranchVVB;
typedef KReorderedBranchT<vector<vector<int>>> KReorderedBranchVVI;
typedef KReorderedBranchT<vector<vector<double>>> KReorderedBranchVVD;
typedef KReorderedBranchT<vector<vector<TLorentzVector>>> KReorderedBranchVVL;

//----------------------------------------------------------------
//base class for Variators, has standard functions defined
class KVariator : public KChecker {
	public:
		//constructor
		KVariator() : KChecker() {}
		KVariator(string name_, OptionMap* localOpt_) : KChecker(name_, localOpt_) {}
		//destructor
		virtual ~KVariator(){
			for(auto& branch : linkbranches){
				delete branch;
			}
		}
		//functions
		virtual void DoVariation() {}
		virtual void UndoVariation() {}
		//different behavior from usual checker
		virtual void CheckBranches() {
			ListBranches();
			for(auto& branch: linkbranches){
				branch->Enable(looper->fChain,1);
				branch->Check(looper->fChain);
			}
			looper->EnableBranches(branches);
		}
		
	protected:
		//member variables
		vector<KLinkedBranchBase*> linkbranches; //in case used
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

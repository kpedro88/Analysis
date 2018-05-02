#ifndef KFACTORY_H
#define KFACTORY_H

#include <string>
#include <map>
#include <utility>

using namespace std;

//singleton factory pattern
//largely based on https://github.com/cms-sw/cmssw/blob/CMSSW_9_0_X/FWCore/PluginManager/interface/PluginFactory.h
//T is base class, U is derived class, Args used for derived class constructors
template <class T, typename... Args>
class KFactory {
	public:
		//helper classes
		struct KConstructorBase {
			virtual T* construct(Args... args) const = 0;
			virtual ~KConstructorBase() {}
		};
		template <class U>
		struct KConstructor : public KConstructorBase {
			KConstructor(string name){
				KFactory<T,Args...>::GetFactory().define(name,this);
			}
			T* construct(Args... args) const {
				return new U(args...);
			}
		};
	
		typedef map<string,KConstructorBase*> ConstructorMap;
		//singleton
		static KFactory<T,Args...>& GetFactory() {
			static KFactory<T,Args...> factory;
			return factory;
		}
		//register derived types
		void define(string name, KConstructorBase* ctor){
			auto cit = MyConstructors.find(name);
			//no redefinitions allowed
			if(cit==MyConstructors.end()){
				MyConstructors.emplace(name,ctor);
			}
		}
		//construct derived types
		T* construct(string name, Args... args){
			auto cit = MyConstructors.find(name);
			if(cit==MyConstructors.end()) return NULL;
			else return cit->second->construct(std::forward<Args>(args)...);
		}
		
	private:
		//members
		ConstructorMap MyConstructors;
};

//preprocessor needs two layers of indirection to evaluate all variables while concatenating
#define MAKECTOR_(a,b) a##ctor##b
#define MAKECTOR(a,b) MAKECTOR_(a,b)

#define REGISTER_MACRO(x,y) static const x::KConstructor<y> MAKECTOR(x,y) (#y)
//allow different names for same class
#define REGISTER_MACRO2(x,y,z) static const x::KConstructor<y> MAKECTOR(x,z) (#z)

#endif

#ifndef EVENTLISTFILTER_H
#define EVENTLISTFILTER_H

#include <unordered_set>
#include <utility>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <exception>

using namespace std;

//internal representation of an event
//from: http://stackoverflow.com/questions/20834838/using-tuple-in-unordered-map
typedef std::tuple<unsigned, unsigned, unsigned long long> Triple;
struct triple_hash : public std::unary_function<Triple, std::size_t>
{
	std::size_t operator()(const Triple& k) const
	{
		return std::get<0>(k) ^ std::get<1>(k) ^ std::get<2>(k);
	}
};
typedef std::unordered_set<Triple,triple_hash> TripleSet;
struct TripleTraits {
	typedef Triple Tuple;
	typedef TripleSet Set;
};

typedef std::tuple<unsigned, unsigned, unsigned long long, unsigned> Quadruple;
struct quadruple_hash : public std::unary_function<Quadruple, std::size_t>
{
	std::size_t operator()(const Quadruple& k) const
	{
		return std::get<0>(k) ^ std::get<1>(k) ^ std::get<2>(k) ^ std::get<3>(k);
	}
};
typedef std::unordered_set<Quadruple,quadruple_hash> QuadrupleSet;
struct QuadrupleTraits {
	typedef Quadruple Tuple;
	typedef QuadrupleSet Set;
};

template <class Traits>
class EventListFilterT {
	public:
		//constructor
		EventListFilterT() : initialized(false) {}
		EventListFilterT(const vector<string>& inputFileList){
			//initialize the set of events
			if(!inputFileList.empty()){
				for(const auto& inputFile : inputFileList){
					ifstream infile(inputFile.c_str());
					if(infile.is_open()){
						string line;
						while(getline(infile,line)){
							vector<string> items;
							process(line,':',items);
							FillList(items);
						}
					}
					else {
						throw std::runtime_error("EventListFilter: could not open file: "+inputFile);
					}
				}
				initialized = true;
			}
		}
		
		//filter
		template<class... Args>
		bool CheckEvent(Args... args) const {
			if(!initialized) return true;
			auto itr = eventList.find(std::make_tuple(args...));
			return (itr==eventList.end());
		}
		
		//accessors
		bool Initialized() const { return initialized; }
		
	private:
		//helper, to be specialized
		void FillList(const vector<string>& items);

		//generalization for processing a line
		void process(string line, char delim, vector<string>& fields){
			stringstream ss(line);
			string field;
			while(getline(ss,field,delim)){
				fields.push_back(field);
			}
		}
	
		//member variables
		bool initialized;
		typename Traits::Set eventList;
};

template<>
void EventListFilterT<TripleTraits>::FillList(const vector<string>& items){
	//convert input to proper types
	if(items.size()==3){
		unsigned run_tmp;
		stringstream s0(items[0]);
		s0 >> run_tmp;
		
		unsigned ls_tmp;
		stringstream s1(items[1]);
		s1 >> ls_tmp;
		
		unsigned long long evt_tmp;
		stringstream s2(items[2]);
		s2 >> evt_tmp;
		
		//insert into set
		eventList.emplace(run_tmp,ls_tmp,evt_tmp);
	}
}

template<>
void EventListFilterT<QuadrupleTraits>::FillList(const vector<string>& items){
	//convert input to proper types
	if(items.size()==4){
		unsigned run_tmp;
		stringstream s0(items[0]);
		s0 >> run_tmp;
		
		unsigned ls_tmp;
		stringstream s1(items[1]);
		s1 >> ls_tmp;
		
		unsigned long long evt_tmp;
		stringstream s2(items[2]);
		s2 >> evt_tmp;
		
		unsigned pt_tmp;
		stringstream s3(items[3]);
		s3 >> pt_tmp;

		//insert into set
		eventList.emplace(run_tmp,ls_tmp,evt_tmp,pt_tmp);
	}
}

//specialization for MC to deal with rounding
template <> template<class... Args>
bool EventListFilterT<QuadrupleTraits>::CheckEvent(Args... args) const {
	if(!initialized) return true;
	auto tup = std::make_tuple(args...);
	auto tupm = std::make_tuple(std::get<0>(tup),std::get<1>(tup),std::get<2>(tup),std::get<3>(tup)-1);
	auto tupp = std::make_tuple(std::get<0>(tup),std::get<1>(tup),std::get<2>(tup),std::get<3>(tup)+1);
	return !(eventList.find(tup)!=eventList.end() or eventList.find(tupm)!=eventList.end() or eventList.find(tupp)!=eventList.end());
}

typedef EventListFilterT<TripleTraits> EventListFilter;
typedef EventListFilterT<QuadrupleTraits> EventListFilterMC;

/*USAGE:
//get event list from TreeMaker repo
git clone git@github.com:TreeMaker/TreeMaker.git -b Run2
cd TreeMaker/Production/test/data
tar -xvf HTMHT_Nov14.tar.gz
//initialize filter
EventListFilter filter("TreeMaker/Production/test/data/HTMHT_csc2015.txt");
//in event loop
bool CSCTightHaloFilterUpdate = filter.CheckEvent(RunNum,LumiBlockNum,EvtNum);
*/

#endif

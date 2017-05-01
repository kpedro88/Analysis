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

using namespace std;

class EventListFilter {
	public:
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
		
		//constructor
		EventListFilter() : initialized(false) {}
		EventListFilter(string inputFileList){
			//initialize the set of events
			if(inputFileList.size()>0){
				ifstream infile(inputFileList.c_str());
				if(infile.is_open()){
					string line;
					while(getline(infile,line)){
						vector<string> items;
						process(line,':',items);
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
					initialized = true;
				}
				else {
					cout << "EventListFilter: could not open file: " << inputFileList << endl;
					initialized = false;
				}
			}
		}
		
		//filter
		bool CheckEvent(unsigned run, unsigned ls, unsigned long long evt){
			if(!initialized) return true;
			auto itr = eventList.find(std::make_tuple(run,ls,evt));
			return (itr==eventList.end());
		}
		
		//accessors
		bool Initialized() const { return initialized; }
		
	private:
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
		TripleSet eventList;
};

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

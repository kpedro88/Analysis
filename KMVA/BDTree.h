#ifndef BDTREE_H
#define BDTREE_H

#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <exception>

#define PUGIXML_HEADER_ONLY
#include "pugixml.hpp"

//now based on https://github.com/cms-sw/cmssw/blob/master/CondFormats/EgammaObjects/interface/GBRTree.h
class DTree {
	public:
		DTree() {}
		virtual ~DTree() {}
		double decision(const vector<float*>& features) const {
			int index = 0;
			do {
				index = *(features[vindex_[index]]) <= vcut_[index] ? vleft_[index] : vright_[index];
			} while (index>0);
			return vres_[-index];
		}
		std::vector<unsigned> vindex_;
		std::vector<double> vcut_;
		std::vector<int> vleft_;
		std::vector<int> vright_;
		std::vector<double> vres_;
};

class BDTree {
	public:
		//constructor (xml parsing)
		BDTree(string xname){
			//parse xml
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(xname.c_str());

			//set up variables
			const auto& method = doc.child("MethodSetup");
			const auto& variables = method.child("Variables");
			for(const auto& v : variables.children("Variable")){
				feature_indices_[v.attribute("Expression").as_string()] = v.attribute("VarIndex").as_int();
			}
			features_.resize(feature_indices_.size(),nullptr);

			//set up trees
			const auto& weights = method.child("Weights");
			trees_.resize(weights.attribute("NTrees").as_int());
			unsigned nt = 0;
			for(const auto& btree : weights.children("BinaryTree")){
				size_t nc = nChildren(btree,"Node");
				if(nc!=1) throw std::runtime_error("BinaryTree should have 1 root node, but has "+std::to_string(nc));

				//start parsing from the root
				parseTree(btree.child("Node"),trees_[nt]);
				++nt;
			}
		}
		
		//accessors
		void AddVariable(std::string vname, float* var){
			if(feature_indices_.find(vname)==feature_indices_.end()) throw std::runtime_error("Unknown variable: "+vname);
			features_[feature_indices_[vname]] = var;
		}
		double evaluate(){
			double sum = 0.;
			for(auto tree : trees_){
				sum += tree.decision(features_);
			}
			//use dumb convention from TMVA::MethodBDT::GetGradBoostMVA() for consistency
			return 2.0/(1.0+exp(-2.0*sum))-1;
		}

	private:
		//helpers
		//extend pugixml to get number of children
		size_t nChildren(const pugi::xml_node& node, std::string cname) const {
			return std::distance(node.children(cname.c_str()).begin(),node.children(cname.c_str()).end());
		}
		bool isLeaf(const pugi::xml_node& node) const {
			size_t nc = nChildren(node,"Node");
			return nc==0;
		}
		void parseTree(const pugi::xml_node& node, DTree& tree) const {
			//leaf case
			if(isLeaf(node)){
				tree.vres_.push_back(node.attribute("res").as_double());
			}
			//tree case
			else {
				int thisidx = tree.vcut_.size();
				tree.vindex_.push_back(node.attribute("IVar").as_int());
				tree.vcut_.push_back(node.attribute("Cut").as_double());
				tree.vleft_.push_back(0);
				tree.vright_.push_back(0);

				//get children
				const pugi::xml_node* left;
				const pugi::xml_node* right;
				for(const auto& cnode : node.children("Node")){
					std::string pos = cnode.attribute("pos").as_string();
					if(pos.compare("l")==0) left = &cnode;
					else if(pos.compare("r")==0) right = &cnode;
				}

				//now parse children recursively
				tree.vleft_[thisidx] = isLeaf(*left) ? -tree.vres_.size() : tree.vcut_.size();
				parseTree(*left,tree);
				tree.vright_[thisidx] = isLeaf(*right) ? -tree.vres_.size() : tree.vcut_.size();
				parseTree(*right,tree);
			}
		}
	
		std::vector<DTree> trees_;
		std::unordered_map<std::string,unsigned> feature_indices_;
		std::vector<float*> features_;
};

#endif

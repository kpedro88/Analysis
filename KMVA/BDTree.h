#ifndef BDTREE_H
#define BDTREE_H

#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <cmath>
#include <exception>

#define PUGIXML_HEADER_ONLY
#include "pugixml.hpp"

struct DNode {
	DNode(unsigned i, double c, int l=0, int r=0) : index_(i), cut_(c), left_(l), right_(r) {}
	unsigned index_;
	double cut_;
	int left_;
	int right_;
};

//now based on https://github.com/cms-sw/cmssw/blob/master/CondFormats/EgammaObjects/interface/GBRTree.h
class DTree {
	public:
		DTree() {}
		virtual ~DTree() {}
		inline double decision(const vector<float>& features) const {
			int index = 0;
			do {
				auto l = nodes_[index].left_;
				auto r = nodes_[index].right_;
				index = features[nodes_[index].index_] <= nodes_[index].cut_ ? l : r;
			} while (index>0);
			return vres_[-index];
		}
		std::vector<DNode> nodes_;
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
			features_.resize(feature_indices_.size(),0.);

			//set up trees
			const auto& weights = method.child("Weights");
			trees_.resize(weights.attribute("NTrees").as_int());
			unsigned nt = 0;
			for(const auto& btree : weights.children("BinaryTree")){
				size_t nc = nChildren(btree,"Node");
				if(nc!=1) throw std::runtime_error("BinaryTree should have 1 root node, but has "+std::to_string(nc));

				//start parsing from the root
				parseTree(btree.child("Node"),trees_[nt]);
/*
				std::cout << "vindex_ = "; std::copy(trees_[nt].nodes_.index_.begin(),trees_[nt].nodes_.index_.end(),std::ostream_iterator<unsigned>(std::cout,",")); std::cout << std::endl;
				std::cout << "vcut_ = "; std::copy(trees_[nt].nodes_.cut_.begin(),trees_[nt].nodes_.cut_.end(),std::ostream_iterator<double>(std::cout,",")); std::cout << std::endl;
				std::cout << "vleft_ = "; std::copy(trees_[nt].nodes_.left_.begin(),trees_[nt].nodes_.left_.end(),std::ostream_iterator<int>(std::cout,",")); std::cout << std::endl;
				std::cout << "vright_ = "; std::copy(trees_[nt].nodes_.right_.begin(),trees_[nt].nodes_.right_.end(),std::ostream_iterator<int>(std::cout,",")); std::cout << std::endl;
				std::cout << "vres_ = "; std::copy(trees_[nt].vres_.begin(),trees_[nt].vres_.end(),std::ostream_iterator<double>(std::cout,",")); std::cout << std::endl;
				std::cout << std::endl;
*/
				++nt;
			}
		}
		
		//accessors
		float* SetVariable(std::string vname){
			if(feature_indices_.find(vname)==feature_indices_.end()) throw std::runtime_error("Unknown variable: "+vname);
			return &features_[feature_indices_[vname]];
		}
		double evaluate(){
			double sum = 0.;
			for(const auto& tree : trees_){
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
				int thisidx = tree.nodes_.size();
				tree.nodes_.emplace_back(node.attribute("IVar").as_int(),node.attribute("Cut").as_double());

				//get children
				pugi::xml_node left;
				pugi::xml_node right;
				for(const auto& cnode : node.children("Node")){
					std::string pos = cnode.attribute("pos").as_string();
					if(pos.compare("l")==0) left = cnode;
					else if(pos.compare("r")==0) right = cnode;
				}

				//now parse children recursively
				tree.nodes_[thisidx].left_ = isLeaf(left) ? -tree.vres_.size() : tree.nodes_.size();
				parseTree(left,tree);
				tree.nodes_[thisidx].right_ = isLeaf(right) ? -tree.vres_.size() : tree.nodes_.size();
				parseTree(right,tree);
			}
		}
	
		std::vector<DTree> trees_;
		std::unordered_map<std::string,unsigned> feature_indices_;
		std::vector<float> features_;
};

#endif

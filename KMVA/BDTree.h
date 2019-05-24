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

class DTree {
	public:
		DTree(double d, unsigned i=0) : left_(nullptr), right_(nullptr), index_(i), cut_(d), isLeaf_(true) {}
		virtual ~DTree() {}
		double decision(const vector<float>& features) const {
			return isLeaf_ ? cut_ :
				features[index_] <= cut_ ? left_->decision(features) : right_->decision(features);
		}
		void setChildren(DTree* left, DTree* right) {
			left_ = left;
			right_ = right;
			if(left_ and right_) isLeaf_ = false;
		}
	protected:
		DTree* left_;
		DTree* right_;
		unsigned index_;
		double cut_;
		bool isLeaf_;
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
			trees_.reserve(weights.attribute("NTrees").as_int());
			for(const auto& btree : weights.children("BinaryTree")){
				size_t nc = nChildren(btree,"Node");
				if(nc!=1) throw std::runtime_error("BinaryTree should have 1 root node, but has "+std::to_string(nc));

				//start parsing from the root
				trees_.push_back(parseTree(btree.child("Node")));
			}
		}
		
		//accessors
		float* SetVariable(std::string vname){
			if(feature_indices_.find(vname)==feature_indices_.end()) throw std::runtime_error("Unknown variable: "+vname);
			return &features_[feature_indices_[vname]];
		}
		double evaluate(){
			double sum = 0.;
			for(auto tree : trees_){
				sum += tree->decision(features_);
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
		DTree* parseTree(const pugi::xml_node& node) const {
			DTree* result = nullptr;
			size_t nc = nChildren(node,"Node");
			//leaf case
			if(nc==0){
				result = new DTree(node.attribute("res").as_double());
			}
			//tree case
			else {
				result = new DTree(node.attribute("Cut").as_double(),node.attribute("IVar").as_int());
				std::array<DTree*,2> children{nullptr,nullptr};
				//now parse children recursively
				unsigned cindex = 0;
				for(const auto& cnode : node.children("Node")){
					children[cindex] = parseTree(cnode);
					++cindex;
				}
				result->setChildren(children[0],children[1]);
			}
			return result;
		}
	
		std::vector<DTree*> trees_;
		std::unordered_map<std::string,unsigned> feature_indices_;
		std::vector<float> features_;
};

#endif

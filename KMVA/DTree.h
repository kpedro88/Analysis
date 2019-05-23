#ifndef DTREE_H
#define DTREE_H

#include <vector>

class DTree {
	public:
		DTree(double d, unsigned i=0) : left_(nullptr), right_(nullptr), index_(i), cut_(d), isLeaf_(true) {}
		virtual ~DTree() {}
		double decision(const vector<float*>& features) const {
			return isLeaf_ ? cut_ :
				*(features[index_]) <= cut_ ? left_->decision(features) : right_->decision(features);
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

#endif

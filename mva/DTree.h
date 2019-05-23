#ifndef DTREE_H
#define DTREE_H

#include <vector>

class DTreeBase {
	public:
		DTreeBase() {}
		virtual ~DTreeBase() {}
		virtual void initialize(double d, unsigned i=0) {}
		virtual double decision(const vector<float*>& features) const { return 0.; }
		virtual DTreeBase* getLeft() { return nullptr; }
		virtual DTreeBase* getRight() { return nullptr; }
};

template <class L, class R>
class DTree : public DTreeBase {
	public:
		using DTreeBase::DTreeBase;
		void initialize(double d, unsigned i) override { cut_ = d; index_ = i; }
		DTreeBase* getLeft() override { return &left; }
		DTreeBase* getRight() override { return &right; }
		double decision(const vector<float*>& features) const override {
			return *(features[index_]) <= cut_ ? left.decision(features) : right.decision(features);
		}
	private:
		L left;
		R right;
		unsigned index_ = 0;
		double cut_ = 0.;
};

class NullChild {

};

template <>
class DTree<NullChild,NullChild> : public DTreeBase {
	public:
		using DTreeBase::DTreeBase;
		void initialize(double d, unsigned) override { value_ = d; }
		double decision(const vector<float*>& features) const override {
			return value_;
		}
	private:
		double value_ = 0.;
};

typedef DTree<NullChild,NullChild> DLeaf;

#endif

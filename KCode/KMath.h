#ifndef KMATH_H
#define KMATH_H

//ROOT headers
#include <TROOT.h>
#include <TMath.h>

//STL headers
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cmath>

using namespace std;

namespace KMath {
	//helper functions
	double phi(double x, double y) {
		double phi_ = atan2(y, x);
		return (phi_>=0) ?  phi_ : phi_ + 2*TMath::Pi();
	}
	double DeltaPhi(double phi1, double phi2) {
		double phi1_= phi( cos(phi1), sin(phi1) );
		double phi2_= phi( cos(phi2), sin(phi2) );
		double dphi_= phi1_-phi2_;
		if( dphi_> TMath::Pi() ) dphi_-=2*TMath::Pi();
		if( dphi_<-TMath::Pi() ) dphi_+=2*TMath::Pi();

		return dphi_;
	}
	double DeltaR(double phi1, double eta1, double phi2, double eta2){
		double dphi = DeltaPhi(phi1,phi2);
		double deta = eta2 - eta1;
		double dR2 = dphi*dphi + deta*deta;
		return sqrt(dR2);
	}
	double TransverseMass(double pt1, double phi1, double pt2, double phi2){
		return sqrt(2*pt1*pt2*(1-cos(DeltaPhi(phi1,phi2))));
	}
}

#endif
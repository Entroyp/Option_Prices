#ifndef DEVIATES_H
#define DEVIATES_H
#include "ran.h"
#include "cmath"

struct Normaldev_BM : Ranq1 {
	double mu,sig;
	double storedval;
	Normaldev_BM( double mmu, double ssig, Ullong i)
	: Ranq1(i), mu(mmu), sig(ssig), storedval(0.) {}
	double dev() {
		double v1,v2,rsq,fac;
		if (storedval == 0.) {
			do {
				v1=2.0*doub()-1.0;
				v2=2.0*doub()-1.0;
				rsq=v1*v1+v2*v2;
			} while (rsq >= 1.0 || rsq == 0.0);
			fac=sqrt(-2.0*log(rsq)/rsq);
			storedval = v1*fac;
			return mu + sig*v2*fac;
		} else {
			fac = storedval;
			storedval = 0.;
			return mu + sig*fac;
		}
	}
};


struct Normaldev : Ranq1 {
	double mu,sig;

	Normaldev(){}
	Normaldev( double mmu, double ssig, Ullong i)
	: Ranq1(i), mu(mmu), sig(ssig){};
	double dev() {
		double u,v,x,y,q;
		do {
			u = doub();
			v = 1.7156*(doub()-0.5);
			x = u - 0.449871;
			y = std::abs(v) + 0.386595;
			q = sqrt(x) + y*(0.19600*y-0.25472*x);
		} while (q > 0.27597
			&& (q > 0.27846 || sqrt(v) > -4.*log(u)*sqrt(u)));
		return mu + sig*v/u;
	}
};

#endif
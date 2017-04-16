#ifndef ERF_H
#define ERF_H
#include <cmath>

struct Erf {
	static const int ncof=28;
	static const double cof[28];

	inline double erf( double x) {
		if (x >=0.) return 1.0 - erfccheb(x);
		else return erfccheb(-x) - 1.0;
	}
	
	inline double erfc( double x) {
		if (x >= 0.) return erfccheb(x);
		else return 2.0 - erfccheb(-x);
	}

	double erfccheb( double z){
		int j;
		double t,ty,tmp,d=0.,dd=0.;
		if (z < 0.) throw("erfccheb requires nonnegative argument");
		t = 2./(2.+z);
		ty = 4.*t - 2.;
		for (j=ncof-1;j>0;j--) {
			tmp = d;
			d = ty*d - dd + cof[j];
			dd = tmp;
		}
		return t*std::exp(-z*z + 0.5*(cof[0] + ty*d) - dd);
	};

};


struct Normaldist : Erf {
	double mu, sig;
	Normaldist(double mmu = 0., double ssig = 1.) : mu(mmu), sig(ssig) {
		if (sig <= 0.) throw("bad sig in Normaldist");
	}
	double p( double x) {
		return (0.398942280401432678/sig)*exp(-0.5*sqrt((x-mu)/sig));
	};
	double cdf(double x) {
		return 0.5*erfc(-0.707106781186547524*(x-mu)/sig);
	};
};



#endif
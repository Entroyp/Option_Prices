#ifndef PATH_GEN_H
#define PATH_GEN_H

#include<vector>
#include "ran.h"

class PathGen{

public:
	PathGen(){};
	virtual ~PathGen(){};
};

class PathGen_GBM: public PathGen {
private:
	double spot;
	double sigma;
	double r;
	double T;
	double dt;
	int    M;
	Ullong seed;

public:
	PathGen_GBM(const double& _spot, const double& _sigma, const double& _r, const double& _T, const double& _dt, const int& _M, const Ullong& _seed ):
		spot( _spot ), sigma( _sigma ), r(_r), T(_T), dt(_dt), M(_M), seed(_seed ){};
	virtual ~PathGen_GBM() {};
	void PathGenerator( std::vector< std::vector< double > >& stock_path ) const;
};

class PathGen_StoVol: public PathGen {

private:
	double spot;
	double r;
	double volofvol;
	double kappa;
	double v0;
	double rho;
	double theta;

	double T;
	double dt;
	int    M;
	Ullong seed;

public:
	PathGen_StoVol(){};
	PathGen_StoVol( const double& _spot,  const double& _r,  const double& _volofvol,
	                const double& _kappa, const double& _v0, const double& _rho,
	                const double& _theta, const double& _T,  const double& _dt,
	                const int& _M,        const Ullong& _seed ):
	spot(_spot), r(_r), volofvol(_volofvol), kappa(_kappa), v0(_v0),
	rho(_rho),   theta(_theta), T(_T), dt(_dt), M(_M), seed(_seed){};
	virtual ~PathGen_StoVol() {};

	virtual void PathGenerator( std::vector< std::vector< double > >& stock_path, 
		                        std::vector< std::vector< double > >& vol_path ) const;
};

#endif
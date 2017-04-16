#ifndef LS_REGRESSION_H
#define LS_REGRESSION_H

#include<vector>
#include "GeneralInfo.h"
#include "payoff.h"
#include "BasisFuncs.h"

#include "ludcmp.h"

class LSRegression{

public:
	LSRegression();
	virtual ~LSRegression(){};
	virtual MC_Price Regression() const = 0;
    
	std::vector< double > OLS( const std::vector< std::vector<double> >& X, 
		                       const std::vector< double >& y ) const;
};

class LSRegression_GBM: public LSRegression {

private:
	double spot;
	double r;
	double dt;
	PayOff* pay_off;
	BASIS* basis;

	std::vector< std::vector<double>> path;

public:
	LSRegression_GBM( const double& _spot, const double& _r, const double& _dt, 
		              PayOff* payoff, BASIS* basis, 
					  const std::vector< std::vector<double>>& _path ):
		spot( _spot ), r(_r), dt(_dt), pay_off( payoff ), basis( basis ), path( _path ){};
	virtual ~LSRegression_GBM() {};
	virtual MC_Price Regression() const;
};

class LSRegression_StoVol: public LSRegression {
private:
	double K; 

public:
	LSRegression_StoVol( const double& k );
	virtual ~LSRegression_StoVol() {};
	virtual MC_Price Regression() const;
};

#endif
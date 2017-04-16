#include "stdafx.h"
#include "BasisFuncs.h"

#include<string>
#include<vector>
#include<cmath>

std::vector<double> BASIS_Laguerre::operator()(const double& x ) const
{
	std::vector< double > res;

	res.push_back( exp( x*( -0.5 ) ) );
	res.push_back( exp( x*( -0.5 ) )*(1.-x) );
	res.push_back( exp( x*( -0.5 ) )*(1.-2.*x+x*x*0.5 ) );

	return( res );
};


std::vector<double> BASIS_Polynomial::operator()(const double& x ) const
{
	std::vector< double > res;

	res.push_back( 1. );
	res.push_back( x );
	res.push_back( x*x );

	return( res );
};


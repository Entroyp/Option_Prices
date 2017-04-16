
#include "stdafx.h"
#include "EuropeanOption.h"
#include "erf.h"
#include <algorithm>

EuropeanOption::EuropeanOption( PayOff* PayOff_ ) : pay_off( PayOff_) {}

double EuropeanOption::price(const double& spot, const double& strike, const double& sigma, const double& r, const double& T ) const {
 
	double d1 = ( log( spot/strike ) + (r + 0.5*sigma*sigma )*T)*(1./(sigma*sqrt(T)));
	double d2 = d1 - sigma*sqrt( T );

	Normaldist normal( 0.0, 1. );
	if( (*pay_off).gettype() == PAYOFF_CALL )
		return( spot*normal.cdf( d1 )-strike*exp(-r*T) *normal.cdf( d2 ) );
	else if( (*pay_off).gettype() == PAYOFF_PUT )
		return( strike*exp(-r*T)*normal.cdf( -d2 )- spot*normal.cdf( -d1 )  );
	else 
		throw( "Unsupported payoff type." );
};

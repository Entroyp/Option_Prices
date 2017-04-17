
#include "stdafx.h"
#include "EuropeanOption.h"
#include "erf.h"
#include <algorithm>
#include <complex>
#include "interp_1d.h"
#include "quadrature.h"
#include "romberg.h"
#include "GeneralInfo.h"


double BlackScholes::price( PayOff* pay_off ) const 
{
 
	double d1 = ( log( spot/strike ) + ( r + 0.5*sigma*sigma )*T )*(1./(sigma*sqrt(T)));
	double d2 = d1 - sigma*sqrt( T );

	Normaldist normal( 0.0, 1. );
	if( (*pay_off).gettype() == PAYOFF_CALL )
		return( spot*normal.cdf( d1 )-strike*exp(-r*T) *normal.cdf( d2 ) );
	else if( (*pay_off).gettype() == PAYOFF_PUT )
		return( strike*exp(-r*T)*normal.cdf( -d2 )- spot*normal.cdf( -d1 )  );
	else 
		throw( "Unsupported payoff type." );
};


 StoVol::StoVol( 
          const double& spot_,  const double& r_,  const double& volofvol_,
          const double& kappa_, const double& v0_, const double& rho_, 
          const double& theta_, const double& T_,  const double& strike_  )
 {
	spot      = spot_;
	r         = r_;
	volofvol  = volofvol_;
	kappa     = kappa_;
	v0        = v0_;
	rho       = rho_;
	theta     = theta_;
	T         = T_;
	strike    = strike_;
	
	double b1 = kappa - rho*volofvol;
	double u1 = 0.5;

	integrand fctor1( rho, volofvol , b1, u1, T, kappa, theta, v0, spot, r, strike );
	Midinf< integrand > p1_( fctor1, 1e-5, 1.e5 );
	
	p1_atom   = 0.5 + qromo( p1_ )*( 1./ PI );
	double b2 = kappa;
	double u2 = -0.5;

	integrand fctor2( rho, volofvol , b2, u2, T, kappa, theta, v0, spot, r, strike );
	Midinf< integrand > p2_( fctor2, 1e-5, 1.e5 );
    p2_atom   =  0.5 + qromo( p2_ )*( 1./ PI );

 };

double StoVol::price( PayOff* pay_off ) const
{

    if( (*pay_off).gettype() == PAYOFF_CALL )
		return( spot*p1_atom -strike*exp(-r*T) *p2_atom );
	else if( (*pay_off).gettype() == PAYOFF_PUT )
		return( strike*exp(-r*T)*( 1.- p2_atom )- spot*( 1.-p1_atom )  );
	else 
		throw( "Unsupported payoff type." );
};!
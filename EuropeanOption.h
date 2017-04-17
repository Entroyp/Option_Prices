#ifndef EUROPEAN_OPTION_H
#define EUROPEAN_OPTION_H

#include "GeneralInfo.h"
#include "payoff.h"
#include <complex>
#include <iomanip>
#include <cmath>
#include <iostream>


class EuropeanOption {
 protected:
  PayOff* pay_off; 

 public:
  EuropeanOption(){};
  virtual ~EuropeanOption() {};
  virtual double price( PayOff* pay_off ) const = 0;
};


class BlackScholes:public EuropeanOption {

 protected:
  double spot;
  double sigma;
  double r;
  double T;
  double strike;

 public:
  BlackScholes( ){};
  BlackScholes( const double& spot_, 
                const double& sigma_, 
                const double& r_, 
                const double& T_ ,
				const double& strike_ ):
                spot( spot_ ), 
                sigma( sigma_ ), r( r_ ), T( T_ ), strike( strike_ ) {};

  virtual ~BlackScholes() {};
  virtual double price( PayOff* pay_off ) const;
};

class StoVol: public EuropeanOption {

 private:
  double p1_atom;
  double p2_atom;

  double spot;
  double r;
  double volofvol;
  double kappa;
  double v0;
  double rho;
  double theta;
  double T;
  double strike;

 public:
  StoVol(){};
  StoVol( 
  const double& spot_,  const double& r_,  const double& volofvol_,
  const double& kappa_, const double& v0_, const double& rho_, 
  const double& theta_, const double& T_,  const double& strike_ );

  virtual ~StoVol() {};
   
  virtual double price( PayOff* PayOff ) const ;
};


struct integrand{

    double rho;
    double volofvol;
    double b;
    double u;
    double tau;
    double kappa;
    double theta;
    double v0;
    double spot;
    double r;
	double strike;

    integrand( const double& rho_,   const double& volofvol_,
               const double& b_,     const double& u_, 
               const double& tau_,   const double& kappa_, 
               const double& theta_, const double& v0_,
               const double& spot_ , const double& r_ ,
			   const double& strike_): 
               rho( rho_ ), volofvol( volofvol_ ),
               b(b_), u(u_), tau(tau_), kappa(kappa_), theta( theta_ ), v0( v0_ ), spot( spot_ ), 
               r(r_), strike( strike_ )
               {};

  double operator()( const double& x )
	{
	   std::complex<double>  i( 0, 1 );
       std::complex<double>  tmp   = rho*volofvol*x*i;
       std::complex<double>  d     = sqrt( (tmp-b)*(tmp-b) - volofvol*volofvol*(2.*u*x*i - x*x ) );
       std::complex<double>  g     = ( b - tmp + d )*(1./( b - tmp - d ) );
       
       std::complex<double>  tmp1  = (b-rho*volofvol*x*i+d)*tau - 2.*log( (1.-g*exp(d*tau))*(1./(1.-g)));
       std::complex<double>  C     = r*x*i*tau + (kappa*theta)*(1./volofvol/volofvol)*tmp1;
       
       std::complex<double>  tmp2  = ( b - rho*volofvol*x*i + d )*(1./volofvol/volofvol);
       std::complex<double>  D     = tmp2*( 1.-exp(d*tau))*(1./(1.-g*exp(d*tau)));
       
       std::complex<double>  phi   = exp( C + D*v0 + log(spot)*i*x );
       std::complex<double>  inte  = exp( -i*x*log(strike) ) *phi*(1./i/x);

       return real( inte );

	};

};

#endif
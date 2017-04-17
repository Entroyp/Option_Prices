// Exercise.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "nr3.h"
#include "GeneralInfo.h"
#include "payoff.h"
#include "erf.h"
#include "EuropeanOption.h"
#include "PathGen.h"
#include "ran.h"
#include "deviates.h"
#include "ludcmp.h"
#include "LSRegression.h"

#include <iostream>
#include <vector>

void test_stovol_simulation()
{
	double spot     = 36.0; 
	double strike   = 40.0;
	double r        = 0.06; 
	double volofvol = 0.15;
	double kappa    = 4.;
	double v0       = 0.2;
	double rho      = -0.3;
	double theta    = 0.2;
	
	double T        = 1.;
	double dt       = 1./50.;
	int M           = int(1e5);
	Ullong seed     = 1290832;

    std::cout<< "********************************************************************************" <<std::endl;
	std::cout<< "Following analysis is to show implementation related to stochastic volatility:" <<std::endl;
	std::cout<< "1) Joint simulation of stock price and stochastic vol, following Heston (1993);"<<std::endl;
	std::cout<< "2) Analytical solution for European options with inverse Fourier transformation;"<<std::endl;
	std::cout<< "3) MC simulation solution for European options."                                 <<std::endl;
	std::cout<< "   Check: i) Analytical prices and MC prices are close."                         <<std::endl;
	std::cout<< "         ii) Option prices with stochastic vol are higher than flat vol."        <<std::endl;
	std::cout<< "4) Comparison of options prices with stochastic vol and flat vol ( BS prices )." <<std::endl;
	std::cout<< "5) Compute the American options with stochastic vol"                             <<std::endl;
	std::cout<< "   Check: i) call option prices for European/American are very close"            <<std::endl;
	std::cout<< "         ii) Americn put option prices are higher compared with European puts."  <<std::endl;
	std::cout<< "================================================================================"<<std::endl<<std::endl;


    PayOff* pay_off_call = new PayOffCall( strike );
	PayOff* pay_off_put  = new PayOffPut( strike );

	BlackScholes Option_BS( spot, theta, r, T , strike );
	std::cout<< "European Call Option Black-Scholes Prices:"<<endl;
    std::cout<< Option_BS.price( pay_off_call ) << endl;

	std::cout<< "European Put  Option Black-Scholes Prices:"<<endl;
	std::cout<< Option_BS.price( pay_off_put ) << endl<<endl;

	std::cout<< "================================================================================"<<std::endl<<std::endl;
    
    StoVol Option_StoVol( spot, r, volofvol, kappa, v0, rho, theta, T, strike );

    double dp_call = Option_StoVol.price( pay_off_call);

    std::cout<<"Stochastic Vol: European Call Option Price - Analytical solution:"<<endl;
    std::cout<< dp_call <<endl;

    double dp_put = Option_StoVol.price( pay_off_put );

    std::cout<<"Stochastic Vol: European Put  Option Price - Analytical solution:"<<endl;
    std::cout<< dp_put <<endl;
	std::cout<< "================================================================================"<<std::endl<<std::endl;
    
    std::vector< std::vector<double>> stock_paths;
    std::vector< std::vector<double>> vol_paths;
	PathGen_StoVol path_gen( spot, r, volofvol, kappa, v0, rho, theta, T, 
    		                 dt, M, seed );
	path_gen.PathGenerator( stock_paths, vol_paths );

    double call_payoff = 0.;
    for( int m = 0; m < M; m++ )
    	call_payoff += (*pay_off_call)( stock_paths[ m ].back() ); 
    std::cout<<"Stochastic Vol: European Call Option Price - MC solution:"<<endl;
    std::cout<<call_payoff*(1./double(M))*exp(-r*T) << std::endl;

    double put_payoff = 0.;
    for( int m = 0; m < M; m++ )
    	put_payoff += (*pay_off_put)( stock_paths[ m ].back() );

    std::cout<<"Stochastic Vol: European Put  Option Price - MC solution:"<<endl;
    std::cout<<put_payoff*(1./double(M))*exp(-r*T) << std::endl;
	std::cout<< "================================================================================="<<std::endl<<std::endl;

	BASIS* basis_Laguerre = new BASIS_Laguerre;
	LSRegression_GBM Regression_Laguerre_call( spot, r, dt, pay_off_call, basis_Laguerre, stock_paths );
	MC_Price res_laguerre_call = Regression_Laguerre_call.Regression();
	std::cout<<"Stochastic Vol: American Call Option Price "<<endl;
	std::cout<< res_laguerre_call.price<<std::endl;

	LSRegression_GBM Regression_Laguerre_put( spot, r, dt, pay_off_put, basis_Laguerre, stock_paths );
	MC_Price res_laguerre_put = Regression_Laguerre_put.Regression();
	std::cout<<"Stochastic Vol: American Put  Option Price "<<endl;
	std::cout<<res_laguerre_put.price<<std::endl;
	std::cout<< "********************************************************************************"<<std::endl<<std::endl;

	delete basis_Laguerre;
    delete pay_off_call;
    delete pay_off_put;

};
int main()
{
	test_stovol_simulation();
}

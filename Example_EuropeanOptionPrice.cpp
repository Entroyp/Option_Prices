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

void test_European_Option_Price()
{
	double spot   = 36.0; 
	double strike = 40.0; 
	double r      = 0.06;   
	double sigma  = 0.2;  
	double T      = 1.0;   

	std::cout<< "****************************************************************************"   <<std::endl;
	std::cout<< "Following analysis is to test European option prices from analytical solutions" <<std::endl;
	std::cout<< "and Monte-Carlo simulations have very close results."                           <<std::endl;
	std::cout<< "============================================================================"   <<std::endl<<std::endl;

	PayOff* pay_off_call = new PayOffCall( strike );
	EuropeanOption CallOption( pay_off_call );
	std::cout<< "Call Option Price from analytical solution: "<<CallOption.price( spot, strike, sigma, r, T ) << std::endl;

	PayOff* pay_off_put = new PayOffPut( strike );
	EuropeanOption PutOption( pay_off_put );
	std::cout<< "Put Option Price from analytical solution: "<<PutOption.price( spot, strike, sigma, r, T ) << std::endl;

	double dt   = 1./50.;
	int M       = int(1e5);
	Ullong seed = 1290832;

	PathGen_GBM stock_path( spot,  sigma, r, T, dt, M, seed );
	std::vector< std::vector<double>> stock_paths = stock_path.PathGenerator();

	double call_payoff = 0.;
	for( int m = 0; m < M; m++ )
		call_payoff += (*pay_off_call)( stock_paths[ m ].back() ); 
	std::cout<<endl;
	std::cout<< "Call Option Price from MC simulation: "<< call_payoff*(1./double(M))*exp(-r*T) << std::endl;

	double put_payoff = 0.;
	for( int m = 0; m < M; m++ )
		put_payoff += (*pay_off_put)( stock_paths[ m ].back() );
	std::cout<< "Put Option Price from MC simulation: "<< put_payoff*(1./double(M))*exp(-r*T) << std::endl;
	std::cout<< "============================================================================"<<std::endl<<std::endl;

	delete pay_off_call;
	delete pay_off_put;

};
int main()
{
	  test_European_Option_Price();
}

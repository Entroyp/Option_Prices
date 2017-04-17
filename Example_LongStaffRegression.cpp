
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

void test_simple_example()
{

	double strike = 1.1; 
	double r = 0.06;   
	double sigma = 0.2;  
	double T = 1.0;   
	double dt = 1.;
	int M  = 8;
	double spot = 1.;

	std::cout<< "****************************************************************************"<<std::endl;
	std::cout<< "Following analysis is to replicate the example in Longstaff-Schwartz ( 2001 )."<<std::endl;
	std::cout<< "The expected result is .1144 ( page 120 )"<<std::endl;
	std::cout<< "============================================================================"<<std::endl<<std::endl;

	PayOff* pay_off_put = new PayOffPut( strike );
	std::vector< std::vector<double>> stock_paths;
	std::vector<double> vector( 3 );

	vector[ 0 ] = 1.09; vector[ 1 ] = 1.08; vector[ 2 ] = 1.34;
	stock_paths.push_back( vector );
	vector[ 0 ] = 1.16; vector[ 1 ] = 1.26; vector[ 2 ] = 1.54;
	stock_paths.push_back( vector );
	vector[ 0 ] = 1.22; vector[ 1 ] = 1.07; vector[ 2 ] = 1.03;
	stock_paths.push_back( vector );
	vector[ 0 ] = 0.93; vector[ 1 ] = 0.97; vector[ 2 ] = 0.92;
	stock_paths.push_back( vector );
	vector[ 0 ] = 1.11; vector[ 1 ] = 1.56; vector[ 2 ] = 1.52;
	stock_paths.push_back( vector );
	vector[ 0 ] = 0.76; vector[ 1 ] = 0.77; vector[ 2 ] = 0.90;
	stock_paths.push_back( vector );
	vector[ 0 ] = 0.92; vector[ 1 ] = 0.84; vector[ 2 ] = 1.01;
	stock_paths.push_back( vector );
	vector[ 0 ] = 0.88; vector[ 1 ] = 1.22; vector[ 2 ] = 1.34;
	stock_paths.push_back( vector );

	BASIS* basis_poly  = new BASIS_Polynomial;
	LSRegression_GBM Regression_poly( spot, r, dt, pay_off_put, basis_poly, stock_paths );
	MC_Price res_poly = Regression_poly.Regression();
	std::cout<<"Price of Regression based on Polynomial Base: "<<res_poly.price << std::endl;

	BASIS* basis_Laguerre = new BASIS_Laguerre;
	LSRegression_GBM Regression_Laguerre( spot, r, dt, pay_off_put, basis_Laguerre, stock_paths );
	MC_Price res_laguerre = Regression_Laguerre.Regression();
	std::cout<<"Price of Regression based on Laguerre Base: "<<res_laguerre.price<<std::endl<<std::endl;
	std::cout<< "****************************************************************************"<<std::endl<<std::endl;

	delete basis_Laguerre;
	delete basis_poly;
};

void test_American_Option_Price()
{
	std::cout<< "****************************************************************************";
	std::cout<<endl;
	std::cout<< "Following analysis is to price American call/put in Table 1 ( Page 127 ) in"<<endl;
	std::cout<< "Longstaff-Schwartz (2001)."<<std::endl;
	std::cout<< "For American put, the expected result is 4.472."<<std::endl;
	std::cout<< "For American call, the expected result should be close to European call price."<<std::endl;
	std::cout<< "=============================================================================";
	std::cout<<endl<<std::endl;

	double spot = 36.0; 
	double strike = 40.0; 
	double r = 0.06;   
	double sigma = 0.2;  
	double T = 1.0;   

	double dt = 1./50.;
	int M  = int(1e5);
	Ullong seed = 1290832;

	PathGen_GBM stock_path( spot,  sigma, r, T, dt, M, seed );
	std::vector< std::vector<double>> stock_paths;
	stock_path.PathGenerator( stock_paths );
	BlackScholes Option_BS( spot, sigma, r, T , strike );

	std::cout<< "American Put Result"<<std::endl;
	std::cout<< "============================================================================="<<endl;

	PayOff* pay_off_put = new PayOffPut( strike );
	std::cout<< "European put option price: " << std::endl;
	std::cout<< "Price: " << Option_BS.price( pay_off_put ) << std::endl;

	BASIS* basis_laguerre = new BASIS_Laguerre;
	LSRegression_GBM Regression_Laguerre_put( spot, r, dt, pay_off_put, basis_laguerre, stock_paths );
	MC_Price res_laguerre = Regression_Laguerre_put.Regression();
	std::cout<< "American put option price with Laguerre Base: "<< std::endl;
	std::cout<< "price: " <<res_laguerre.price<<"    "<< "StdDev: " <<res_laguerre.stddev<<std::endl;

	BASIS* basis_polynomial  = new BASIS_Polynomial;
	LSRegression_GBM Regression_Polynomial_put( spot, r, dt, pay_off_put, basis_polynomial, stock_paths );
	MC_Price res_polynomial = Regression_Polynomial_put.Regression();
	std::cout<<"American put option price with Polynomial Base: "<<std::endl;
	std::cout<< "Price: " <<res_polynomial.price<<"    "<< "StdDev: "<<res_polynomial.stddev<<std::endl;

	std::cout<<endl<<endl;
	std::cout<< "American Call Result"<<std::endl;
	std::cout<< "============================================================================="<<std::endl;

	PayOff* pay_off_call = new PayOffCall( strike );


	std::cout<< "European call option price: " <<  Option_BS.price( pay_off_call ) << std::endl;
	LSRegression_GBM Regression_Laguerre_call( spot, r, dt, pay_off_call, basis_laguerre, stock_paths );
	res_laguerre = Regression_Laguerre_call.Regression();
	std::cout<<"American call option price with Laguerre Base: "<<std::endl;
	std::cout<< "price: " <<res_laguerre.price<<"    "<< "StdDev: " <<res_laguerre.stddev<<std::endl;

	LSRegression_GBM Regression_Polynomial_call( spot, r, dt, pay_off_call, basis_polynomial, stock_paths );
	res_polynomial = Regression_Polynomial_call.Regression();
	std::cout<<"American call option price with Polynomial Base: "<< std::endl;
	std::cout<< "price: " << res_polynomial.price<<"    "<< "StdDev: " <<res_polynomial.stddev<<std::endl;
	std::cout<< "****************************************************************************"<<std::endl<<std::endl;

	delete pay_off_put;
	delete pay_off_call;

	delete basis_laguerre;
	delete basis_polynomial;

};


int main()
{
  test_simple_example();
  test_American_Option_Price();
}

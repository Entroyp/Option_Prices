
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



void American_Option_Price()
{
	std::cout<< "****************************************************************************";
	std::cout<< endl;
	std::cout<< "A pricer for American options based on Longstaff-Schwartz (2001) algorithm."<<endl;
	std::cout<< "Some model assumptions and specifications: "<<endl;
	std::cout<< "1) Stock dynamics is assumed to follow a GBM process under Q-measure;"<<endl;
	std::cout<< "2) Short rate and volatility is assumed to be constant;"<<endl;
	std::cout<< "3) Dividends and funding spreads are assumed to be zero;"<<endl;
	std::cout<< "4) By default, number of MC simulation is 10,000; "<<endl;
	std::cout<< "5) The simulation incremental step size is 1/50;"<<endl;
	std::cout<< "6) Laguerre basis with degree 3 is chosen in the regression."<<endl;
	std::cout<< "=============================================================================";
	std::cout<<endl<<std::endl;

	double spot; 
	double strike; 
	double r;   
	double sigma;  
	double T;   
	string payoff;

	std::cout<<"Enter Spot: ";
    std::cin>>spot;

	std::cout<<"Enter Strike: ";
    std::cin>>strike;

	std::cout<<"Enter Short Rate: ";
    std::cin>>r;

	std::cout<<"Enter Volatility ( in pts ): ";
    std::cin>>sigma;

	std::cout<<"Enter Time to Maturity: ";
	std::cin>>T;

	std::cout<<"Enter Payoff Type ( Put or Call ): ";
	std::cin>>payoff;


	double dt   = 1./50.;
	int M       = int(1e5);
	Ullong seed = 1290832;

	PathGen_GBM stock_path( spot,  sigma, r, T, dt, M, seed );
	std::vector< std::vector<double>> stock_paths = stock_path.PathGenerator();


	std::cout<< "American Option Result"<<std::endl;
	std::cout<< "============================================================================="<<endl;

	PayOff* pay_off;

	if( payoff == PAYOFF_CALL )
	pay_off = new PayOffCall( strike );
	else if( payoff == PAYOFF_PUT )
	pay_off = new PayOffPut( strike );
	else
		throw( "Unsupported payoff type." );

	EuropeanOption PutOption( pay_off );
	std::cout<< "European option price: " << std::endl;
	std::cout<< "Price: " << PutOption.price( spot, strike, sigma, r, T ) << std::endl;

	BASIS* basis_laguerre = new BASIS_Laguerre;
	LSRegression_GBM Regression_Laguerre( spot, r, dt, pay_off, basis_laguerre, stock_paths );
	MC_Price res_laguerre = Regression_Laguerre.Regression();
	std::cout<< "American option price: "<< std::endl;
	std::cout<< "price: " <<res_laguerre.price<<"    "<< "StdDev: " <<res_laguerre.stddev<<std::endl;

	std::cout<<endl<<endl;
	std::cout<< "============================================================================="<<std::endl;

	
	delete pay_off;
	delete basis_laguerre;

};


int main()
{
	American_Option_Price();
}

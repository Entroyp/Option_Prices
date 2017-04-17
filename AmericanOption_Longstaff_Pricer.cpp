
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
	std::cout<< "****************************************************************************"<<endl;
	std::cout<< "A pricer for American options based on Longstaff-Schwartz (2001) algorithm." <<endl;
	std::cout<< "Some model assumptions and specifications: "                                 <<endl;
	std::cout<< "1) Stock dynamics is assumed to follow a GBM process under Q-measure;"       <<endl;
	std::cout<< "2) Short rate and volatility is assumed to be constant;"                     <<endl;
	std::cout<< "3) Dividends and funding spreads are assumed to be zero;"                    <<endl;
	std::cout<< "4) By default, number of MC simulation is 10,000; "                          <<endl;
	std::cout<< "5) The simulation incremental step size is 1/50;"                            <<endl;
	std::cout<< "6) Laguerre basis with degree 3 is chosen in the regression."                <<endl;
	std::cout<< "=============================================================================";
	std::cout<<endl<<std::endl;

	string vol_type;

	std::cout<<"Enter vol type ( StoVol or FlatVol ): ";
	std::cin>>vol_type;

	double spot; 
	double strike; 
	double r;   
	double T;   
	string payoff;

	std::cout<<"Enter Spot: ";
    std::cin>>spot;

	std::cout<<"Enter Strike: ";
    std::cin>>strike;

	std::cout<<"Enter Short Rate: ";
    std::cin>>r;

	if( r <= 0 )
		throw( "short rate needs to be non-negative.");

	std::cout<<"Enter Time to Maturity: ";
	std::cin>>T;

	std::cout<<"Enter Payoff Type ( Put or Call ): ";
	std::cin>>payoff;

	PayOff* pay_off;

	if( payoff == PAYOFF_CALL )
	pay_off = new PayOffCall( strike );
	else if( payoff == PAYOFF_PUT )
	pay_off = new PayOffPut( strike );
	else
		throw( "Unsupported payoff type." );


	double dt   = 1./50.;
	int M       = int(1e5);
	Ullong seed = 1290832;
	double sigma;
    std::vector< std::vector<double>> stock_paths;

	if( vol_type == STO_VOL )
	{

		double volofvol;
		double kappa;
		double v0;
		double rho;
		double theta;
		
		std::cout<<"Enter volatility of volatility ( sigma ): ";
		std::cin>>volofvol;

		std::cout<<"Enter mean-reverting rate ( kappa ): ";
		std::cin>>kappa;

		std::cout<<"Enter long-run volatility ( theta ): ";
		std::cin>>theta;
		sigma = theta;

		std::cout<<"Enter initial volatility ( v0 ): ";
		std::cin>>v0;

		std::cout<<"Enter correlation between stock and vol ( rho ): ";
		std::cin>>rho;

        std::cout<<"Computing..."<<endl;
	    std::cout<<"============================================================================="<<endl;

		std::cout<< "European option price: " << std::endl;
		StoVol Option_StoVol( spot, r, volofvol, kappa, v0, rho, theta, T, strike );
	    std::cout<< "Price: " << Option_StoVol.price( pay_off ) << std::endl;

	    std::vector< std::vector<double>> vol_paths;
		PathGen_StoVol path_gen( spot, r, volofvol, kappa, v0, rho, theta, T, dt, M, seed );
		path_gen.PathGenerator( stock_paths, vol_paths );	

	}
	else if( vol_type == FLAT_VOL )
	{
		std::cout<<"Enter Volatility: ";
	    std::cin>>sigma;

	    std::cout<<"Computing..."<<endl;
	    std::cout<< "============================================================================="<<endl;
		PathGen_GBM stock_path( spot, sigma, r, T, dt, M, seed );
	    stock_path.PathGenerator( stock_paths );
    
	    BlackScholes Option_BS( spot, sigma, r, T , strike );
	    std::cout<< "European option price: " << std::endl;
	    std::cout<< "Price: " << Option_BS.price( pay_off ) << std::endl;

	}
	else
	{
		throw( "Unknown volatility model type.");
	};
    
	std::cout<< "American option price: "<< std::endl;
	BASIS* basis_laguerre = new BASIS_Laguerre;
	LSRegression_GBM Regression_Laguerre( spot, r, dt, pay_off, basis_laguerre, stock_paths );
	MC_Price res_laguerre = Regression_Laguerre.Regression();

	std::cout<< "price: " <<res_laguerre.price<<"    "<< "StdDev: " <<res_laguerre.stddev<<std::endl;
	std::cout<<"Completed!"<<endl;
	std::cout<< "============================================================================="<<std::endl;

	
	delete pay_off;
	delete basis_laguerre;

};


int main()
{
	American_Option_Price();
}

#ifndef GENERAL_INFO_H
#define GENERAL_INFO_H


#define PAYOFF_CALL "Call"
#define PAYOFF_PUT  "Put"



class MC_Price{

public:
	double price;
	double stddev;

	MC_Price( const double& price_, const double& stddev_ ): price(price_), stddev(stddev_)
	{};
	
	MC_Price(){};
};


#endif
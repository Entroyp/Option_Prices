#ifndef PAYOFF_H
#define PAYOFF_H

#include<string>

class PayOff{

public:
	PayOff();
	virtual ~PayOff(){};
	virtual double operator() (const double& s ) const = 0;
	virtual std::string gettype() const = 0;

};

class PayOffCall : public PayOff {
private:
	double K; 
	char Type;

public:
	PayOffCall(const double& k );
	virtual ~PayOffCall() {};
	virtual double operator() (const double& s ) const;
	virtual std::string gettype() const;

};

class PayOffPut : public PayOff {
private:
	double K; 

public:
	PayOffPut(const double& k );
	virtual ~PayOffPut() {};
	virtual double operator() (const double&  s) const;
	virtual std::string gettype() const;

};


#endif
#ifndef BASIS_H
#define BASIS_H

#include<string>
#include<vector>

class BASIS{

public:
	BASIS(){};
	virtual ~BASIS(){};
	virtual std::vector<double> operator() (const double& x ) const = 0;
};

class BASIS_Laguerre : public BASIS{

public:
	BASIS_Laguerre(){};
	virtual ~BASIS_Laguerre() {};
	virtual std::vector<double> operator() (const double& x ) const;
};

class BASIS_Polynomial: public BASIS{

public:
	BASIS_Polynomial(){};
	virtual ~BASIS_Polynomial() {};
	virtual std::vector<double> operator() (const double& x ) const;
};


#endif
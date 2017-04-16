#ifndef EUROPEAN_OPTION_H
#define EUROPEAN_OPTION_H

#include "GeneralInfo.h"
#include "payoff.h"

class EuropeanOption {
 protected:
  PayOff* pay_off;  // Pay-off class (in this instance call or put) 

 public:
  EuropeanOption( PayOff* PayOff );
  virtual ~EuropeanOption() {};
  virtual double price( const double& spot, const double& strike, const double& sigma, const double& r, const double& T ) const;
};


#endif
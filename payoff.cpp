
#include "stdafx.h"
#include "payoff.h"
#include "GeneralInfo.h"
#include <cmath>

PayOff::PayOff() {}

PayOffCall::PayOffCall(const double& _K) { K = _K; }

double PayOffCall::operator() (const double& S) const {
  return std::max(S-K, 0.0); 
}

std::string PayOffCall::gettype() const {
  return PAYOFF_CALL;
};

PayOffPut::PayOffPut(const double& _K) {
  K = _K;
}
double PayOffPut::operator() (const double& S) const {
  return std::max(K-S, 0.0);
}

std::string PayOffPut::gettype() const {
  return PAYOFF_PUT;
};
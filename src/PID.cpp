#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double kp, double ki, double kd) {
  Kp = kp;
  Ki = ki;
  Kd = kd;
  p_error = 0.0;
  i_error = 0.0;
  d_error = 0.0;
  cteo = 0.0;
}

void PID::UpdateError(double cte) {
  p_error = cte;
  i_error = i_error + cte;
  d_error = cte - cteo;
  cteo = cte;
  std::cout << "PE " << p_error << " PI " << i_error << " PD " << d_error << std::endl;
}

double PID::TotalError() {
  std::cout << "PID " <<  (Kp*p_error + Ki*i_error + Kd*d_error) << std::endl;
  return (Kp*p_error + Ki*i_error + Kd*d_error);
}


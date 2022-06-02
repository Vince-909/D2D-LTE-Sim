#ifndef EESM_EFFECTIVE_SINR_H_
#define EESM_EFFECTIVE_SINR_H_

#include <math.h>
#include <vector>
#include <iostream>

static double beta_value [20] =
{
	1.49, 1.53, 1.57, 1.61, 1.69,
	1.69, 1.65, 3.36, 4.56, 6.42,
	7.33, 7.68, 9.21, 10.81, 13.76,
	17.52, 20.57, 22.75, 25.16, 28.38
};

static double GetEesmEffectiveSinr(std::vector <double> sinr)
{
  double eff_sinr;
  double sum_I_sinr = 0;
  std::vector<double>::iterator it;
  int counter = 0;
  for (it = sinr.begin (); it != sinr.end(); it++)
   {
	  if (*it == 0) continue;
	  double s = pow (10, ((*it)/10));
	  sum_I_sinr += s;
	  counter ++;
   }
  eff_sinr = sum_I_sinr / counter;

  eff_sinr = 10 * log10 (eff_sinr); //convert in dB

  return eff_sinr;
}

#endif

#ifndef EMODEL_H_
#define EMODEL_H_

#include <math.h>

#define Ro 94.2

static double Ief(double packetLoss)
{
	return 11 + 40*log(1+10*packetLoss);
};

static int H(double x)
{
	if (x < 0) return 0;
	else return 1;
};

static double Id(double delay)
{
	return 0.024*delay + 0.11*(delay-177.3)*H(delay - 177.3);
};

static double R(double delay, double packetLoss)
{
	return Ro - Ief(packetLoss) - Id(delay);
};

static double MOS(double delay, double packetLoss)
{
	double r = R(delay, packetLoss);
	if (r < 6.5) return 1;
	else if (r <= 100) return 1 + 0.035*r + r*(r - 60)*(100 - r)*7*pow(10.0, -6.0);
	else return 4.5;
};

#endif
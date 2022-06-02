#include "ProportionalFairDownlinkPacketScheduler.h"
#include "../Rrc/RadioBearer.h"
#include <iostream>

ProportionalFairDownlinkPacketScheduler::ProportionalFairDownlinkPacketScheduler()
{
  SetMacEntity (0);
  CreateFlowsToSchedule ();
}

ProportionalFairDownlinkPacketScheduler::~ProportionalFairDownlinkPacketScheduler()
{
  Destroy ();
}


double ProportionalFairDownlinkPacketScheduler::ComputeSchedulingMetric(RadioBearer *bearer, double spectralEfficiency, int subChannel)
{
  /*
   * For the PF scheduler the metric is computed
   * as follows:
   *
   * metric = spectralEfficiency / averageRate
   */
  double metric = (spectralEfficiency * 180000.) / bearer->GetAverageTransmissionRate();

  return metric;
}
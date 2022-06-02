#include "SimpleErrorModel.h"
#include "BlerVsSinr.h"
#include <iostream>

SimpleErrorModel::SimpleErrorModel()
{
}

SimpleErrorModel::~SimpleErrorModel()
{
}

bool SimpleErrorModel::CheckForPhysicalError(std::vector<int> channels, std::vector<int> cqi, std::vector<double> sinr)
{
  /*
   * The device determines if a packet has been received correctly.
   * To this aim, for each sub-channel, used to transmit that packet,
   * the Block Error Rate (BLER) is estimated.
   *
   * The BLER is obtained considering both MCS used for the transmission
   * and SINR that the device has estimated for the considered sub-channel.
   * In particular, the BLER value is drawn using stored BLER-SINR curves
   * stored into trace files, generated using an ad hoc OFDMA tool written in matlab.
   * According to the proper BLER-SINR curve (depending on the used MCS),
   * the device establishes if the packet has been correctly received or not.
   * In the latter case, the packet is considered erroneous and ignored.
   */
	
  bool error = false;

  double randomNumber = (rand () %100 ) / 100.;

  for (int i = 0; i < channels.size (); i++)
  {
	  int cqi_ = cqi.at (i);
	  double sinr_ = sinr.at (channels.at (i));
	  double bler;
	  bler = GetBLER_TU (sinr_, cqi_);
      if (randomNumber < bler)
	  {
		  error = true;
	  }

  }

  return error;
}
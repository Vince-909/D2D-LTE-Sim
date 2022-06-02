#include "BandwidthManager.h"

#define UL_LOW_FREQUENCY_BAND_1 1920 	//MHz
#define UL_HIGH_FREQUENCY_BAND_1 1980 	//MHz
#define DL_LOW_FREQUENCY_BAND_1 2110 	//MHz
#define DL_HIGH_FREQUENCY_BAND_1 2170 	//MHz

#define RBs_FOR_1_4_MHz 6
#define RBs_FOR_3_MHz 15
#define RBs_FOR_5_MHz 25
#define RBs_FOR_10_MHz 50
#define RBs_FOR_15_MHz 75
#define RBs_FOR_20_MHz 100

BandwidthManager::BandwidthManager(double ulBw, double dlBw, int ulOffset, int dlOffset)
{
	m_ulBandwidth = ulBw;
	m_dlBandwidth = dlBw;
	m_ulOffsetBw = ulOffset;
	m_dlOffsetBw = dlOffset;

	m_operativeSubBand = 1;

	m_dlSubChannels.clear();
	m_ulSubChannels.clear();

	if (dlBw == 1.4)
    {
      for (int i = dlOffset; i < dlOffset + RBs_FOR_1_4_MHz; i++)
        {
    	  m_dlSubChannels.push_back(DL_LOW_FREQUENCY_BAND_1 + (i * 0.18));
        }
    }
	else if (dlBw == 3)
    {
      for (int i = dlOffset; i < dlOffset + RBs_FOR_3_MHz; i++)
        {
    	  m_dlSubChannels.push_back(DL_LOW_FREQUENCY_BAND_1 + (i * 0.18));
        }
    }
	else if (dlBw == 5)
    {
      for (int i = dlOffset; i < dlOffset + RBs_FOR_5_MHz; i++)
        {
    	  m_dlSubChannels.push_back(DL_LOW_FREQUENCY_BAND_1 + (i * 0.18));
        }
    }
	else if (dlBw == 10)
    {
      for (int i = dlOffset; i < dlOffset + RBs_FOR_10_MHz; i++)
        {
    	  m_dlSubChannels.push_back(DL_LOW_FREQUENCY_BAND_1 + (i * 0.18));
        }
    }
	else if (dlBw == 15)
    {
      for (int i = dlOffset; i < dlOffset + RBs_FOR_15_MHz; i++)
        {
    	  m_dlSubChannels.push_back(DL_LOW_FREQUENCY_BAND_1 + (i * 0.18));
        }
    }
	else if (dlBw == 20)
    {
      for (int i = dlOffset; i < dlOffset + RBs_FOR_20_MHz; i++)
        {
    	  m_dlSubChannels.push_back(DL_LOW_FREQUENCY_BAND_1 + (i * 0.18));
        }
    }
	else
    {
      for (int i = dlOffset; i < dlOffset + RBs_FOR_5_MHz; i++)
        {
    	  m_dlSubChannels.push_back(DL_LOW_FREQUENCY_BAND_1 + (i * 0.18));
        }
    }



	if (ulBw == 1.4)
    {
      for (int i = ulOffset; i < ulOffset + RBs_FOR_1_4_MHz; i++)
        {
    	  m_ulSubChannels.push_back(UL_LOW_FREQUENCY_BAND_1 + (i * 0.18));
        }
    }
	else if (ulBw == 3)
    {
      for (int i = ulOffset; i < ulOffset + RBs_FOR_3_MHz; i++)
        {
    	  m_ulSubChannels.push_back(UL_LOW_FREQUENCY_BAND_1 + (i * 0.18));
        }
    }
	else if (ulBw == 5)
    {
      for (int i = ulOffset; i < ulOffset + RBs_FOR_5_MHz; i++)
        {
    	  m_ulSubChannels.push_back(UL_LOW_FREQUENCY_BAND_1 + (i * 0.18));
        }
    }
	else if (ulBw == 10)
    {
      for (int i = ulOffset; i < ulOffset + RBs_FOR_10_MHz; i++)
        {
    	  m_ulSubChannels.push_back(UL_LOW_FREQUENCY_BAND_1 + (i * 0.18));
        }
    }
	else if (ulBw == 15)
    {
      for (int i = ulOffset; i < ulOffset + RBs_FOR_15_MHz; i++)
        {
    	  m_ulSubChannels.push_back(UL_LOW_FREQUENCY_BAND_1 + (i * 0.18));
        }
    }
	else if (ulBw == 20)
    {
      for (int i = ulOffset; i < ulOffset + RBs_FOR_20_MHz; i++)
        {
    	  m_ulSubChannels.push_back(UL_LOW_FREQUENCY_BAND_1 + (i * 0.18));
        }
    }
	else
    {
      for (int i = ulOffset; i < ulOffset + RBs_FOR_5_MHz; i++)
        {
    	  m_ulSubChannels.push_back(UL_LOW_FREQUENCY_BAND_1 + (i * 0.18));
        }
    }
}

BandwidthManager::~BandwidthManager()
{
}

std::vector<double> BandwidthManager::GetDlSubChannels(void)
{
  return m_dlSubChannels;
}

std::vector<double> BandwidthManager::GetUlSubChannels(void)
{
  return m_ulSubChannels;
}

int BandwidthManager::GetOperativeSubBand(void)
{
  return m_operativeSubBand;
}

double BandwidthManager::GetUlBandwidth(void)
{
  return m_ulBandwidth;
}

double BandwidthManager::GetDlBandwidth(void)
{
  return m_dlBandwidth;
}


int BandwidthManager::GetUlOffsetBw(void)
{
  return m_ulOffsetBw;
}

int BandwidthManager::GetDlOffsetBw(void)
{
  return m_dlOffsetBw;
}

#include "MacroCellUrbanAreaChannelRealization.h"
#include "../Device/NetworkNode.h"
#include "../Position/Position.h"
#include "../Utility/RandomVariable.h"
#include "ShadowingTrace.h"
#include "../Core/Simulator.h"
#include "../Physical/LtePhy.h"
#include "../Spectrum/BandwidthManager.h"

MacroCellUrbanAreaChannelRealization::MacroCellUrbanAreaChannelRealization(NetworkNode* src, NetworkNode* dst)
{
  SetSamplingPeriod(0.5);

  m_penetrationLoss = 10;
  m_shadowing = 0;
  m_pathLoss = 0;

  SetSourceNode(src);
  SetDestinationNode(dst);

  UpdateModels ();

}

MacroCellUrbanAreaChannelRealization::~MacroCellUrbanAreaChannelRealization()
{
}

void
MacroCellUrbanAreaChannelRealization::SetPenetrationLoss (double pnl)
{
  m_penetrationLoss = pnl;
}

double
MacroCellUrbanAreaChannelRealization::GetPenetrationLoss(void)
{
  return m_penetrationLoss;
}

double
MacroCellUrbanAreaChannelRealization::GetPathLoss(void)
{
  /*
   * According to  ---  insert standard 3gpp ---
   * the Path Loss Model For Urban Environment is
   * L = I + 37.6log10(R)
   * R, in kilometers, is the distance between two nodes
   * I = 128.1 at 2GHz
   */
  double distance;

  NetworkNode* src = GetSourceNode ();
  NetworkNode* dst = GetDestinationNode ();

  distance = src->GetPosition()->CalculateDistance(dst);

  m_pathLoss = 128.1 + (37.6 * log10 (distance));
  return m_pathLoss;
}


void MacroCellUrbanAreaChannelRealization::SetShadowing(double sh)
{
  m_shadowing = sh;
}

double MacroCellUrbanAreaChannelRealization::GetShadowing(void)
{
  return m_shadowing;
}


void MacroCellUrbanAreaChannelRealization::UpdateModels()
{
  //update shadowing
  m_shadowing = 0;
  double probability = GetRandomVariable(101) / 100.0;
  for (int i = 0; i < 201; i++)
    {
	  if (probability <= shadowing_probability[i])
	    {
		  m_shadowing = shadowing_value[i];
          break;
	    }
    }
  SetLastUpdate();
}


std::vector<double> MacroCellUrbanAreaChannelRealization::GetLoss()
{

  if (NeedForUpdate())
    {
	   UpdateModels();
    }

  std::vector<double> loss;

  int now_ms = (int)(Simulator::Init()->Now () * 1000);
  int lastUpdate_ms = (int)(GetLastUpdate () * 1000);
  int index = now_ms - lastUpdate_ms;

  int nbOfSubChannels = GetSourceNode()->GetPhy()->GetBandwidthManager()->GetDlSubChannels().size();

  for (int i = 0; i < nbOfSubChannels; i++)
    {
	  double l = GetPathLoss() + GetPenetrationLoss() + GetShadowing();
	  loss.push_back(l);
    }
  return loss;
}
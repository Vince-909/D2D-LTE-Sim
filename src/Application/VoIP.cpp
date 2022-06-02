#include "VoIP.h"
#include "../Core/Simulator.h"
#include "../Packet/Packet.h"
#include "../Device/UserEquipment.h"
#include "../Rrc/RadioBearer.h"
#include <iomanip>
#include "../QualityOfExperience/QoeManager.h"
#include "ApplicationSink.h"
#include "../D2DManager/D2DManager.h"
#include "../Mac/UeMacEntity.h"
#include "../Spectrum/BandwidthManager.h"
#include "../Physical/LtePhy.h"

VoIP::VoIP()
{
  /*
   * G729 codec generates during the ON period a packet with
   * a fixed size (20 bytes). We must add the RTP header (12 bytes)
   */
  m_size = 20; //application + RTP header
  m_stateON = false;
  SetApplicationType (Application::APPLICATION_TYPE_VOIP);
}

VoIP::~VoIP()
{
  Destroy();
}

void
VoIP::DoStart (void)
{
  Simulator::Init()->Schedule(0.0, &VoIP::Send, this);
  //schedule quality of experience report
  Simulator::Init()->Schedule(GetApplicationSink()->GetQoeManager()->GetReportingInterval(), &QoeManager::ReportQoe, GetApplicationSink()->GetQoeManager());
}

void
VoIP::DoStop (void)
{}

void
VoIP::ScheduleTransmit (double time)
{
  if ((Simulator::Init()->Now() + time) < GetStopTime())
    {
      Simulator::Init()->Schedule(time, &VoIP::Send, this);
    }
}

void
VoIP::Send (void)
{
  if (!m_stateON)
	{
	  m_stateON = true;
	  //start state ON
	  double random = rand() %10000;
	  m_stateDuration = -3*log(1-((double)random/10000));
	  m_endState = Simulator::Init()->Now() + m_stateDuration;
	}

  Packet *packet = new Packet();
  int uid = Simulator::Init()->GetUID();
  packet->SetId(uid);
  packet->SetTimeStamp(Simulator::Init()->Now());
  packet->SetSize(GetSize());
  packet->SetApplicationId(GetApplicationID());
  packet->SetSource(GetApplicationSource()->GetNodeID());
  packet->SetDestination(GetApplicationDestination()->GetNodeID());

  std::cout << GetApplicationID() << " SND " << packet->GetId() << " " <<
	  std::fixed << std::setprecision(3) << packet->GetTimeStamp() << std::endl;
  
  GetApplicationSink()->GetQoeManager()->UpdateSentPackets();
  GetRadioBearer()->Enqueue(packet);
  UserEquipment* host = GetApplicationSource();
  UeMacEntity* hostMac = (UeMacEntity*) host->GetMacEntity();
  int numRb = host->GetPhy()->GetBandwidthManager()->GetUlSubChannels().size();

  if (Simulator::Init()->Now() <= m_endState)
    {
	  ScheduleTransmit (0.02);
    }
  else
    {
	  //schedule OFF Period
      m_stateON = false;
	  double random = rand()%10000;
	  m_stateDuration = -2.23*log(1-((double)random/10000));
	  if (m_stateDuration > 6.9)
	    {
		  m_stateDuration = 6.9;
	    }
	  ScheduleTransmit (m_stateDuration);
    }
}


int VoIP::GetSize(void) const
{
  return m_size;
}
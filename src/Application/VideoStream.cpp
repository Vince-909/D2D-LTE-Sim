#include "VideoStream.h"
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
#include <fstream>
#include <cstring>
#include <cstdlib>

#define TRACE_FILE "foreman_H264_440k.dat"

VideoStream::VideoStream()
{
	SetApplicationType(Application::APPLICATION_TYPE_VIDEOSTREAM);
	m_currentSize = 0;
}

VideoStream::~VideoStream()
{
  Destroy();
}

void
VideoStream::DoStart (void)
{
	if (!(LoadFile(TRACE_FILE)))
	{
		for (int i = 0 ; i < 1500 ; i ++) m_sizes.push_back((rand()%2000) + 1000);
	}
	Simulator::Init()->Schedule(0.0, &VideoStream::Send, this);
}

void
VideoStream::DoStop (void)
{}

void
VideoStream::ScheduleTransmit(double time)
{
  if ((Simulator::Init()->Now() + time) < GetStopTime())
    {
      Simulator::Init()->Schedule(time, &VideoStream::Send, this);
    }
}

void
VideoStream::Send (void)
{
  Packet *packet = new Packet();
  int uid = Simulator::Init()->GetUID();
  packet->SetId(uid);
  packet->SetTimeStamp(Simulator::Init()->Now());
  packet->SetSize(GetSizes().at(m_currentSize++));
  if (m_currentSize == GetSizes().size()) m_currentSize = 0;
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
  
  ScheduleTransmit(0.04);
}


std::vector<int> VideoStream::GetSizes(void)
{
	return m_sizes;
}

bool VideoStream::LoadFile(char* filename)
{
	char buffer[64];
	char* tok;
	std::ifstream ifs (filename, std::ifstream::in);
	if (!(ifs)) return false;
	while (!(ifs.eof()))
	{
		ifs.getline(buffer, 64);
		if (ifs.eof()) break;
		tok = strtok(buffer, "\t");
		tok = strtok(NULL, "\t");
		tok = strtok(NULL, "\t");
		tok = strtok(NULL, "\t");
		int size = atoi(tok);
		m_sizes.push_back(size);
	}
	return true;
}
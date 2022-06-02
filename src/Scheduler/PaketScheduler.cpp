#include "PacketScheduler.h"
#include <iostream>
#include <vector>

PacketScheduler::PacketScheduler()
{
	m_mac = NULL;
}

PacketScheduler::~PacketScheduler()
{
	m_mac = NULL;
}

void PacketScheduler::Destroy(void)
{
	ClearFlowsToSchedule();
	delete m_flowsToSchedule;
	m_mac = NULL;
}

MacEntity* PacketScheduler::GetMacEntity(void)
{
	return m_mac;
}

void PacketScheduler::SetMacEntity(MacEntity* mac)
{
	m_mac = mac;
}

void PacketScheduler::Schedule(void)
{
	DoSchedule();
}

void PacketScheduler::DoSchedule(void)
{
}

void PacketScheduler::StopSchedule(void)
{
	DoStopSchedule();
}

void PacketScheduler::DoStopSchedule(void)
{
}

std::vector<int> PacketScheduler::FlowToSchedule::GetCqiFeedbacks(void)
{
  return m_cqiFeedbacks;
}

std::vector<int>* PacketScheduler::FlowToSchedule::GetListOfAllocatedRBs()
{
  return &m_listOfAllocatedRBs;
}

int PacketScheduler::FlowToSchedule::GetDataToTransmit(void)
{
  return m_dataToTransmit;
}

void PacketScheduler::ClearFlowsToSchedule()
{
  FlowsToSchedule*  records = GetFlowsToSchedule();
  FlowsToSchedule::iterator iter;

  for (iter = records->begin(); iter != records->end(); iter++)
    {
	  delete *iter;
    }

  GetFlowsToSchedule()->clear();
}

void PacketScheduler::CreateFlowsToSchedule(void)
{
  m_flowsToSchedule = new FlowsToSchedule();
}

std::vector<double> PacketScheduler::FlowToSchedule::GetSpectralEfficiency(void)
{
  return m_spectralEfficiency;
}

void PacketScheduler::InsertFlowToSchedule(RadioBearer* bearer, int dataToTransmit, std::vector<double> specEff, std::vector<int> cqiFeedbacks)
{

  FlowToSchedule *flowToSchedule = new FlowToSchedule(bearer, dataToTransmit);
  flowToSchedule->SetSpectralEfficiency(specEff);
  flowToSchedule->SetCqiFeedbacks(cqiFeedbacks);

  GetFlowsToSchedule()->push_back(flowToSchedule);
}

void PacketScheduler::FlowToSchedule::UpdateAllocatedBits(int allocatedBits)
{
  m_allocatedBits += allocatedBits;
  int availableBytes = m_allocatedBits/8;

  int transmittedPackets = ceil
			  (availableBytes/1513.0);

  m_transmittedData = availableBytes - (transmittedPackets * 8);
  if (m_transmittedData < 0)
    {
	  m_transmittedData = 0;
    }
}

int PacketScheduler::FlowToSchedule::GetAllocatedBits(void)
{
  return m_allocatedBits;
}

PacketScheduler::FlowToSchedule::FlowToSchedule(RadioBearer* bearer, int dataToTransmit)
{
  m_bearer = bearer;
  m_allocatedBits = 0;
  m_transmittedData = 0;
  m_dataToTransmit = dataToTransmit;
}

PacketScheduler::FlowToSchedule::~FlowToSchedule()
{
}

void PacketScheduler::FlowToSchedule::SetCqiFeedbacks(std::vector<int> cqiFeedbacks)
{
  m_cqiFeedbacks = cqiFeedbacks;
}

void PacketScheduler::FlowToSchedule::SetSpectralEfficiency(std::vector<double> s)
{
  m_spectralEfficiency = s;
}

PacketScheduler::FlowsToSchedule* PacketScheduler::GetFlowsToSchedule(void)
{
  return m_flowsToSchedule;
}

RadioBearer* PacketScheduler::FlowToSchedule::GetBearer(void)
{
  return m_bearer;
}
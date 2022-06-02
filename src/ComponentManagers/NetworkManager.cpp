#include "NetworkManager.h"
#include <vector>
#include "../Device/UserEquipment.h"
#include "../Cell/Cell.h"
#include "../Rrc/RrcEntity.h"
#include "../Mac/UeMacEntity.h"
#include "../Physical/UeLtePhy.h"
#include "../Position/Position.h"
#include "../Position/Mobility.h"
#include "../Device/ENodeB.h"
#include "../Mac/EnbMacEntity.h"
#include "../Physical/EnbLtePhy.h"
#include "../Cqi/FullbandCqiManager.h"
#include <iostream>
#include "../Channel/LteChannel.h"
#include "../Channel/MacroCellUrbanAreaChannelRealization.h"
#include "../Channel/PropagationLossModel.h"
#include "../Physical/SimpleErrorModel.h"
#include "../D2DManager/D2DManager.h"
#include "../PowerControl/PowerManager.h"

NetworkManager* NetworkManager::ptr=NULL;

NetworkManager::NetworkManager()
{
  m_cellContainer = new std::vector<Cell*>;
  m_eNodeBContainer = new std::vector<ENodeB*>;
  m_userEquipmentContainer = new std::vector<UserEquipment*>;
}

NetworkManager::~NetworkManager()
{
  std::vector<Cell*>::iterator iter;
  for (iter = m_cellContainer->begin();
      iter != m_cellContainer->end(); iter++)
    {
    delete *iter;
    }
  delete m_cellContainer;

  std::vector<ENodeB*>::iterator iter2;
  for (iter2 = m_eNodeBContainer->begin();
      iter2 != m_eNodeBContainer->end(); iter2++)
    {
    delete *iter2;
    }
  delete m_eNodeBContainer;

  std::vector<UserEquipment*>::iterator iter3;
  for (iter3 = m_userEquipmentContainer->begin();
      iter3 != m_userEquipmentContainer->end(); iter3++)
    {
    delete *iter3;
    }
  delete m_userEquipmentContainer;
}


Cell* NetworkManager::CreateCell(int id, double radius, double minDistance)
{
  Cell *cell = new Cell();
  cell->SetIdCell(id);
  cell->SetPosition(0.0, 0.0);
  cell->SetRadius(radius);
  cell->SetMinDistance(minDistance);

  GetCellContainer()->push_back(cell);

  return cell;
}

ENodeB* NetworkManager::CreateEnodeb(int id, Cell* cell, BandwidthManager* manager, LteChannel* uplink, LteChannel* downlink, bool D2DAvailability)
{
	//place ENodeB at cell center
	ENodeB* enb = new ENodeB();
	enb->SetNodeType(NetworkNode::TYPE_ENODEB);
	enb->SetNodeID(id);
	enb->SetCell(cell);
	enb->SetPosition(cell->GetPosition()->GetPosX(), cell->GetPosition()->GetPosY());
	enb->SetD2DAvailability(D2DAvailability);

	RrcEntity* rrc = new RrcEntity();
	rrc->SetDevice(enb);
	enb->SetRrcEntity(rrc);

	EnbMacEntity* mac = new EnbMacEntity();
	mac->SetDevice(enb);
	enb->SetMacEntity(mac);

	EnbLtePhy* phy = new EnbLtePhy();
	phy->SetDevice(enb);
	enb->SetPhy(phy);

	PowerManager* pm = new PowerManager();
	pm->SetPhy(enb->GetPhy());
	pm->SetTxPower();
	enb->GetPhy()->SetPowerManager(pm);

	D2DManager* d2dman = new D2DManager();
	enb->SetD2DManager(d2dman);
	d2dman->SetDevice(enb);

	SimpleErrorModel* errorModel = new SimpleErrorModel();
    enb->GetPhy()->SetErrorModel((ErrorModel*)errorModel);
	
	enb->GetPhy()->SetBandwidthManager(manager);

	enb->GetPhy()->SetDlChannel(downlink);
	enb->GetPhy()->SetUlChannel(uplink);

	//attach ENodeB to the uplink channel
	uplink->AddDevice(enb);

	GetENodeBContainer()->push_back (enb);

  return enb;
}

UserEquipment* NetworkManager::CreateUserEquipment(int id, Cell* cell, double x, double y, double speed, double direction, ENodeB* enb)
{
	UserEquipment* ue = new UserEquipment();
	ue->SetNodeType(NetworkNode::TYPE_UE);
	ue->SetNodeID(id);
	ue->SetCell(cell);
	ue->SetTargetEnb(enb);
	ue->SetPosition(x, y, speed, direction, cell->GetRadius(), cell->GetMinDistance());

	//if position is ilegal, pick another random position

	double distance = ue->GetPosition()->CalculateDistance(enb);

	if ((distance < ue->GetPosition()->GetMobility()->GetInnerBound()) ||
		(distance > ue->GetPosition()->GetMobility()->GetOuterBound()))
		ue->GetPosition()->GetMobility()->Randomize();

	FullbandCqiManager* cqim = new FullbandCqiManager();
	cqim->SetCqiReportingMode(CqiManager::PERIODIC);
	cqim->SetReportingInterval(1);
	cqim->SetDevice(ue);
	ue->SetCqiManager(cqim);

	RrcEntity* rrc = new RrcEntity();
	rrc->SetDevice(ue);
	ue->SetRrcEntity(rrc);

	UeMacEntity* mac = new UeMacEntity();
	mac->SetDevice(ue);
	ue->SetMacEntity(mac);

	UeLtePhy* phy = new UeLtePhy();
	phy->SetDevice(ue);
	ue->SetPhy(phy);

	PowerManager* pm = new PowerManager();
	pm->SetPhy(ue->GetPhy());
	pm->SetTxPower();
	ue->GetPhy()->SetPowerManager(pm);

	D2DManager* d2dman = new D2DManager();
	ue->SetD2DManager(d2dman);
	d2dman->SetDevice(ue);

	SimpleErrorModel* errorModel = new SimpleErrorModel();
    ue->GetPhy()->SetErrorModel((ErrorModel*)errorModel);

	ue->GetPhy()->SetBandwidthManager(ue->GetTargetEnb()->GetPhy()->GetBandwidthManager());

	ue->GetPhy()->SetDlChannel(enb->GetPhy()->GetDlChannel());
	ue->GetPhy()->SetUlChannel(enb->GetPhy()->GetUlChannel());

	//attach Ue to the downlink channel
	enb->GetPhy()->GetDlChannel()->AddDevice(ue);

	//register the Ue to the ENodeB
	enb->RegisterUserEquipment(ue);

	//create new uplink and downlink channnel realizations
	MacroCellUrbanAreaChannelRealization* c_dl = new MacroCellUrbanAreaChannelRealization(enb, ue);
    enb->GetPhy()->GetDlChannel()->GetPropagationLossModel()->AddChannelRealization(c_dl);
    MacroCellUrbanAreaChannelRealization* c_ul = new MacroCellUrbanAreaChannelRealization(ue, enb);
    enb->GetPhy()->GetUlChannel()->GetPropagationLossModel()->AddChannelRealization(c_ul);

	GetUserEquipmentContainer()->push_back(ue);

	return ue;
}

UserEquipment* NetworkManager::CreateUserEquipment(int id, Cell* cell, double speed, ENodeB* enb)
{
	UserEquipment* ue = new UserEquipment();
	ue->SetNodeType(NetworkNode::TYPE_UE);
	ue->SetNodeID(id);
	ue->SetCell(cell);
	ue->SetTargetEnb(enb);
	ue->SetPosition(0.0, 0.0, speed, 0.0, cell->GetRadius(), cell->GetMinDistance());
	ue->GetPosition()->GetMobility()->Randomize();

	FullbandCqiManager* cqim = new FullbandCqiManager();
	cqim->SetCqiReportingMode(CqiManager::PERIODIC);
	cqim->SetReportingInterval(1);
	cqim->SetDevice(ue);
	ue->SetCqiManager(cqim);

	RrcEntity* rrc = new RrcEntity();
	rrc->SetDevice(ue);
	ue->SetRrcEntity(rrc);
	
	UeMacEntity* mac = new UeMacEntity();
	mac->SetDevice(ue);
	ue->SetMacEntity(mac);
	
	UeLtePhy* phy = new UeLtePhy();
	phy->SetDevice(ue);
	ue->SetPhy(phy);

	PowerManager* pm = new PowerManager();
	pm->SetPhy(ue->GetPhy());
	pm->SetTxPower();
	ue->GetPhy()->SetPowerManager(pm);

	D2DManager* d2dman = new D2DManager();
	ue->SetD2DManager(d2dman);
	d2dman->SetDevice(ue);

	SimpleErrorModel* errorModel = new SimpleErrorModel();
    ue->GetPhy()->SetErrorModel((ErrorModel*)errorModel);
	
	ue->GetPhy()->SetBandwidthManager(ue->GetTargetEnb()->GetPhy()->GetBandwidthManager());

	ue->GetPhy()->SetDlChannel(enb->GetPhy()->GetDlChannel());
	ue->GetPhy()->SetUlChannel(enb->GetPhy()->GetUlChannel());

	//attach Ue to the downlink channel
	enb->GetPhy()->GetDlChannel()->AddDevice(ue);

	//register Ue to the ENodeb
	enb->RegisterUserEquipment(ue);

	//create new uplink and downlink channel realizations
	MacroCellUrbanAreaChannelRealization* c_dl = new MacroCellUrbanAreaChannelRealization(enb, ue);
    enb->GetPhy()->GetDlChannel()->GetPropagationLossModel()->AddChannelRealization(c_dl);
    MacroCellUrbanAreaChannelRealization* c_ul = new MacroCellUrbanAreaChannelRealization(ue, enb);
    enb->GetPhy()->GetUlChannel()->GetPropagationLossModel()->AddChannelRealization(c_ul);

	GetUserEquipmentContainer()->push_back(ue);

	return ue;
}

std::vector<Cell*>* NetworkManager::GetCellContainer(void)
{
  return m_cellContainer;
}

std::vector<ENodeB*>* NetworkManager::GetENodeBContainer(void)
{
  return m_eNodeBContainer;
}

std::vector<UserEquipment*>* NetworkManager::GetUserEquipmentContainer(void)
{
  return m_userEquipmentContainer;
}

Cell* NetworkManager::GetCellByID (int idCell)
{
  std::vector<Cell*>* cellContainer = GetCellContainer ();
  std::vector<Cell*>::iterator iter;
  Cell *cell;
  for (iter = cellContainer->begin ();
      iter !=cellContainer->end (); iter++)
    {
    cell = *iter;
    if (cell->GetIdCell() == idCell)
      return cell;
    }
  return false;
}

ENodeB* NetworkManager::GetENodeBByID (int idENodeB)
{
  std::vector<ENodeB*>* eNodeBContainer = GetENodeBContainer ();
  std::vector<ENodeB*>::iterator iter2;
  ENodeB *eNodeB;
  for (iter2 = eNodeBContainer->begin ();
      iter2 !=eNodeBContainer->end (); iter2++)
    {
    eNodeB = *iter2;
    if (eNodeB->GetNodeID() == idENodeB)
      {
      return eNodeB;
      }
    }
  return false;
}

ENodeB* NetworkManager::GetENodeBByCellID (int idCell)
{
  std::vector<ENodeB*>* eNodeBContainer = GetENodeBContainer ();
  std::vector<ENodeB*>::iterator iter2;
  ENodeB *eNodeB;
  for (iter2 = eNodeBContainer->begin ();
      iter2 !=eNodeBContainer->end (); iter2++)
    {
    eNodeB = *iter2;
    if (eNodeB->GetCell()->GetIdCell ()== idCell)
      {
      return eNodeB;
      }
    }
  return false;
}

UserEquipment* NetworkManager::GetUserEquipmentByID (int idUE)
{
  std::vector<UserEquipment*>* userEquipmentContainer = GetUserEquipmentContainer ();
  std::vector<UserEquipment*>::iterator iter3;
  UserEquipment *userEquipment;
  for (iter3 = userEquipmentContainer->begin ();
      iter3 !=userEquipmentContainer->end (); iter3++)
    {
    userEquipment = *iter3;
    if (userEquipment->GetNodeID() == idUE)
      {
      return userEquipment;
      }
    }
  return false;
}

NetworkNode* NetworkManager::GetNetworkNodeByID(int id)
{
  std::vector<ENodeB*>* eNodeBContainer = GetENodeBContainer ();
  std::vector<ENodeB*>::iterator iter2;
  ENodeB *eNodeB;
  for (iter2 = eNodeBContainer->begin ();
      iter2 !=eNodeBContainer->end (); iter2++)
    {
    eNodeB = *iter2;
    if (eNodeB->GetNodeID() == id)
      {
      return eNodeB;
      }
    }

  std::vector<UserEquipment*>* userEquipmentContainer = GetUserEquipmentContainer ();
  std::vector<UserEquipment*>::iterator iter3;
  UserEquipment *userEquipment;
  for (iter3 = userEquipmentContainer->begin ();
      iter3 !=userEquipmentContainer->end (); iter3++)
    {
    userEquipment = *iter3;
    if (userEquipment->GetNodeID() == id)
      {
      return userEquipment;
      }
    }

  return NULL;
}

std::vector<UserEquipment*>* NetworkManager::GetRegisteredUEToENodeB(int idENB)
{
  std::vector<UserEquipment*>* UElist = new std::vector<UserEquipment*>;

  std::vector<UserEquipment*>* userEquipmentContainer = GetUserEquipmentContainer ();
  std::vector<UserEquipment*>::iterator iter;
  UserEquipment *userEquipment;
  for (iter = userEquipmentContainer->begin ();
      iter !=userEquipmentContainer->end (); iter++)
    {
    userEquipment = *iter;
	if (userEquipment->GetTargetEnb()->GetNodeID() == idENB)
      {
      UElist->push_back(userEquipment);
      }
    }
  return UElist;
}

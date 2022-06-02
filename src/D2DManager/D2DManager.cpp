#include "D2DManager.h"
#include "../Core/Simulator.h"
#include "../Packet/Packet.h"
#include "../Packet/PacketBurst.h"
#include "../Device/UserEquipment.h"
#include "../Channel/MacroCellUrbanAreaChannelRealization.h"
#include "../Channel/LteChannel.h"
#include "../Physical/LtePhy.h"
#include "../Channel/PropagationLossModel.h"
#include "../Spectrum/BandwidthManager.h"
#include <vector>
#include "../Spectrum/TransmittedSignal.h"
#include "../Physical/BlerVsSinr.h"
#include "../Mac/AmcModule.h"
#include "../Mac/UeMacEntity.h"
#include "../ComponentManagers/FlowsManager.h"
#include "../Application/Application.h"
#include "../QualityOfExperience/QoeManager.h"
#include "../Device/NetworkNode.h"
#include "../Device/ENodeB.h"
#include "../Physical/EnbLtePhy.h"
#include "../Application/ApplicationSink.h"
#include "../Physical/UeLtePhy.h"
#include "../PowerControl/PowerManager.h"
#include <fstream>
#include "../Position/Position.h"
#include "../Physical/ErrorModel.h"
#include <iostream>
#include <iomanip>

#define NOISE -148.95

D2DManager::D2DManager()
{
	m_device = NULL;
	d2dEnabled = false;
	D2DTestNeeded = false;
}

D2DManager::~D2DManager()
{
}

bool D2DManager::D2DinUse(void)
{
	return d2dEnabled;
}

NetworkNode* D2DManager::GetDevice(void)
{
	return m_device;
}

void D2DManager::SetDevice(NetworkNode* node)
{
	m_device = node;
}

void D2DManager::StartD2DCommunication(UserEquipment* ue1, UserEquipment* ue2)
{
	D2DIdealControlMessage* msg = new D2DIdealControlMessage();
	msg->SetD2DMessageType(D2DIdealControlMessage::D2D_ENABLE);
	msg->SetSource(GetDevice());
	msg->SetDestination(ue2);
	msg->SetD2DRole(D2DIdealControlMessage::RECEIVER);
	GetDevice()->GetPhy()->SendIdealControlMessage(msg);

	msg = new D2DIdealControlMessage();
	msg->SetD2DMessageType(D2DIdealControlMessage::D2D_ENABLE);
	msg->SetSource(GetDevice());
	msg->SetDestination(ue1);
	msg->SetD2DRole(D2DIdealControlMessage::SENDER);
	GetDevice()->GetPhy()->SendIdealControlMessage(msg);

}

void D2DManager::StopD2DCommunication(UserEquipment* ue1, UserEquipment* ue2)
{
	D2DIdealControlMessage* msg = new D2DIdealControlMessage();
	msg->SetD2DMessageType(D2DIdealControlMessage::D2D_DISABLE);
	msg->SetSource(GetDevice());
	msg->SetDestination(ue1);
	msg->SetD2DRole(D2DIdealControlMessage::SENDER);
	GetDevice()->GetPhy()->SendIdealControlMessage(msg);

	msg = new D2DIdealControlMessage();
	msg->SetD2DMessageType(D2DIdealControlMessage::D2D_DISABLE);
	msg->SetSource(GetDevice());
	msg->SetDestination(ue2);
	msg->SetD2DRole(D2DIdealControlMessage::RECEIVER);
	GetDevice()->GetPhy()->SendIdealControlMessage(msg);

}

void D2DManager::ReceiveD2DIdealControlMessage(D2DIdealControlMessage* msg)
{
	if (msg->GetD2DMessageType() == D2DIdealControlMessage::NEED_TEST)
	{
		SetNeedForD2DTest(true);
		SetD2DTarget(msg->GetTestData().destinationUe);
	}
	else if (msg->GetD2DMessageType() == D2DIdealControlMessage::TEST_DATA)
		{
			//perform the test
			double estimatedMos = DoD2DTest(msg->GetTestData());
			//report test result to the ENodeB
			D2DIdealControlMessage* newmsg = new D2DIdealControlMessage();
			newmsg->SetD2DMessageType(D2DIdealControlMessage::TEST_RESULT);
			newmsg->SetSource(GetDevice());
			ENodeB* enb = ((UserEquipment*) GetDevice())->GetTargetEnb();
			newmsg->SetDestination((NetworkNode*) enb);
			newmsg->SetTestData(estimatedMos);
			GetDevice()->GetPhy()->SendIdealControlMessage(newmsg);
			delete newmsg;
	}
	else if (msg->GetD2DMessageType() == D2DIdealControlMessage::TEST_RESULT)
	{
		ENodeB* thisEnb = (ENodeB*) GetDevice();
		ENodeB::UserEquipmentRecord* record = thisEnb->GetUserEquipmentRecord(msg->GetSource()->GetNodeID());
		m_lastQoeReport = record->GetQoe();
		m_lastTestResult = msg->GetTestData().result;

		UserEquipment* senderUe;
		std::vector<Application*>* apps = FlowsManager::Init()->GetApplicationsContainer();
		for (int i = 0 ; i < apps->size() ; i ++)
			if (apps->at(i)->GetApplicationDestination()->GetNodeID() == msg->GetSource()->GetNodeID())
				{
					senderUe = apps->at(i)->GetApplicationSource();
					break;
				}

		//save the test results in a file
		std::ofstream out;
		out.open("D2DManagementOutput.txt", std::ofstream::out | std::ofstream::app);
		int m;
		for (m = 1 ; m <= apps->size() ; m ++)
		if ((apps->at(m-1)->GetApplicationDestination()->GetNodeID() == msg->GetSource()->GetNodeID()) ||
			(apps->at(m-1)->GetApplicationSource()->GetNodeID() == senderUe->GetNodeID())) break;
		out << m << " CEL " << std::fixed << std::setprecision(2) << m_lastQoeReport <<
		" " << m_lastTestResult << " " << std::fixed << std::setprecision(1) <<
		1000*senderUe->GetPosition()->CalculateDistance(msg->GetSource()) << "\t" <<
		std::fixed << std::setprecision(3) << Simulator::Init()->Now();

		//start d2d communication if test was succesful
		if ((m_lastTestResult >= m_lastQoeReport) && (m_lastTestResult != 1.0))
		{
			out << "\tSTART D2D" << std::endl;
			StartD2DCommunication(senderUe, (UserEquipment*) msg->GetSource());
		}
		else
		{
			out << std::endl;
			LteChannel* ulCh = GetDevice()->GetPhy()->GetUlChannel();
			PropagationLossModel* plm = ulCh->GetPropagationLossModel();
			plm->DelChannelRealization(senderUe, (UserEquipment*) msg->GetSource());
		}
		out.close();
	}
	else if (msg->GetD2DMessageType() == D2DIdealControlMessage::D2D_ENABLE)
	{
		d2dEnabled = true;
		UeLtePhy* uePhy = (UeLtePhy*) GetDevice()->GetPhy();
		if (msg->GetD2DRole() == D2DIdealControlMessage::SENDER)
		{
			uePhy->DoSetBandwidthManager();
		}
		else if (msg->GetD2DRole() == D2DIdealControlMessage::RECEIVER)
		{
			//add the receiving UE to the uplink channel
			GetDevice()->GetPhy()->GetUlChannel()->AddDevice(GetDevice());
		}
		delete msg;
	}
	else if (msg->GetD2DMessageType() == D2DIdealControlMessage::D2D_DISABLE)
	{
		d2dEnabled = false;
		UeLtePhy* uePhy = (UeLtePhy*) GetDevice()->GetPhy();
		if (msg->GetD2DRole() == D2DIdealControlMessage::SENDER)
		{
		}
		else
		{
			//detach the UE from the channel
			GetDevice()->GetPhy()->GetUlChannel()->DelDevice(GetDevice());
		}
		delete msg;
	}
	else if (msg->GetD2DMessageType() == D2DIdealControlMessage::CHANNEL_MAP)
	{
		if (GetDevice()->GetNodeType() == NetworkNode::TYPE_ENODEB)
		{
			UserEquipment* receiverUe;
			std::vector<Application*>* apps = FlowsManager::Init()->GetApplicationsContainer();
			for (int i = 0 ; i < apps->size() ; i ++)
				if (apps->at(i)->GetApplicationSource()->GetNodeID() == msg->GetSource()->GetNodeID())
				{
					receiverUe = apps->at(i)->GetApplicationDestination();
					break;
				}
			msg->SetSource((NetworkNode*) this);
			msg->SetDestination((NetworkNode*) receiverUe);
			GetDevice()->GetPhy()->SendIdealControlMessage(msg);
			delete msg;
		}
		else if (GetDevice()->GetNodeType() == NetworkNode::TYPE_UE)
		{
			((UeLtePhy*) (GetDevice()->GetPhy()))->SetChannelsForRx(msg->GetChannels());
			((UeLtePhy*) (GetDevice()->GetPhy()))->SetMcsIndexForRx(msg->GetMcs());
		}
	}
	
}

double D2DManager::DoD2DTest(D2DIdealControlMessage::TestData data)
{
	Application* app;
	std::vector<Application*>* apps = FlowsManager::Init()->GetApplicationsContainer();
	for (int i = 0 ; i < apps->size() ; i ++)
		if (apps->at(i)->GetApplicationDestination()->GetNodeID() == GetDevice()->GetNodeID())
		{
			app = apps->at(i);
			break;
		}

	QoeManager* mQoe = app->GetApplicationSink()->GetQoeManager();
	double estimatedDelay = mQoe->GetTotalDelay()/mQoe->GetTotalReceivedPackets() - 0.002; //seconds
	double estimatedPacketLoss;

	TransmittedSignal* txSignal = new TransmittedSignal();
	//calculate d2d transmission power
	txSignal->SetValues(data.txSignalValues);
	PropagationLossModel* plm = GetDevice()->GetPhy()->GetUlChannel()->GetPropagationLossModel();
	int errors = 0;
	for (int test = 0 ; test < 100 ; test ++)
	{
		bool error = false;
		std::vector<double> sinr;
		sinr.clear();
		TransmittedSignal* rxSignal = plm->AddLossModel(data.sourceUe, GetDevice(), txSignal);
		double interference = 0;
		double noise_interference = 10. * log10(pow(10., NOISE/10) + interference); // dB
		for (int it = 0 ; it < rxSignal->Getvalues().size() ; it ++)
		{
			if (rxSignal->Getvalues().at(it) != 0.0) sinr.push_back( rxSignal->Getvalues().at(it) - noise_interference);
			else sinr.push_back(0.0);
		}
		
		if (GetDevice()->GetPhy()->GetErrorModel() != NULL)
		{
			std::vector<int> cqi_;
			for (int i = 0; i < data.mcsIndexTx.size(); i++)
			{
				AmcModule *amc = GetDevice()->GetMacEntity()->GetAmcModule();
				int cqi =  amc->GetCQIFromMCS(data.mcsIndexTx.at(i));
				cqi_.push_back(cqi);
			}
			error = GetDevice()->GetPhy()->GetErrorModel()->CheckForPhysicalError(data.channelsTx, cqi_, sinr);
		}
		else error = false;
		if (error) errors ++;
	}
	estimatedPacketLoss = errors / 100.;

	double estimatedMos = mQoe->CalculateMos(estimatedDelay, estimatedPacketLoss);

	return estimatedMos;
}

void D2DManager::SetNeedForD2DTest(bool need)
{
	D2DTestNeeded = need;
}

bool D2DManager::D2DTestIsNeeded(void)
{
	return D2DTestNeeded;
}

void D2DManager::SetD2DTarget(UserEquipment* target)
{
	m_D2Dtarget = target;
}

UserEquipment* D2DManager::GetD2DTarget(void)
{
	return m_D2Dtarget;
}
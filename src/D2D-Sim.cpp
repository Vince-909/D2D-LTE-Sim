#include <iostream>
#include <time.h>
#include "Core\Simulator.h"
#include "ComponentManagers\FlowsManager.h"
#include "ComponentManagers\FrameManager.h"
#include "ComponentManagers\NetworkManager.h"
#include "Channel\LteChannel.h"
#include "Application\Application.h"
#include "Cell\Cell.h"
#include "Device\ENodeB.h"
#include "Device\UserEquipment.h"
#include <glut.h>
#include "Spectrum\BandwidthManager.h"
#include "Application\ApplicationSink.h"
#include "QualityOfExperience\Emodel.h"
#include "Position\Position.h"
#include <math.h>
#include "QualityOfExperience\QoeManager.h"
#include <fstream>
#include <iomanip>
#include "Scheduler\DownlinkPacketScheduler.h"
#include "Mac\EnbMacEntity.h"
#include "Physical\BlerVsSinr.h"

#define USE_D2D_COM true

void printQOE(int, int);

int main(int argc, char** argv)
{
	double programStartTime = time(NULL);
	//Provide seed for rand()
	long seed;
	if (argc == 2) seed = atoi(argv[1]);
	else seed = time(NULL);
	srand(seed);
	std::cout << "Simulating with SEED: " << seed << std::endl;

	//Create output file
	std::ofstream out;
	out.open("D2DManagementOutput.txt", std::ofstream::out | std::ofstream::trunc);
	out.close();

	//Initialize core components
	Simulator *simulator = Simulator::Init();
	NetworkManager* networkManager = NetworkManager::Init();
	FrameManager* frameManager = FrameManager::Init();
	FlowsManager* flowsManager = FlowsManager::Init();

	//Create a cell at (0,0)
	int idCell = 0;
	double radius = 0.7; //km
	double minDist = 0.005; //km
	Cell* cell = networkManager->CreateCell(idCell, radius, minDist);

	//Create UL&DL channels
	LteChannel* ulCh = new LteChannel;
	LteChannel* dlCh = new LteChannel;

	//Create spectrum
	BandwidthManager* spectrum = new BandwidthManager(5, 5, 0, 0);

	int nodeId = 1;
	int appId = 1;
	//Create ENodeB at cell center
	ENodeB* enb = networkManager->CreateEnodeb(nodeId++, cell, spectrum, ulCh, dlCh, USE_D2D_COM);
	enb->SetDLScheduler(ENodeB::DLScheduler_TYPE_PROPORTIONAL_FAIR);
	enb->SetULScheduler(ENodeB::ULScheduler_TYPE_ROUNDROBIN);
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	//                           //
	//     CREATE EQUIPMENT      //
	//                           //
	///////////////////////////////

	int numberOfVoipTransmitters = 20;
	for (int cc = 0 ; cc < numberOfVoipTransmitters ; cc ++)
	{
		//Create sender UE
		double direction = 3.145;
		double speed;
		if (cc < 1) speed = 4; // kph
		else
		speed = 0;
		UserEquipment* ue1;
		if (cc < 1)
			ue1 = networkManager->CreateUserEquipment(nodeId++, cell, 0.390, 0.0, speed, direction, enb);
		else
			ue1 = networkManager->CreateUserEquipment(nodeId++, cell, speed, enb);

		//Create receiver UE
		direction = 0;
		if (cc < 1) speed = 0; // kph
		else
			speed = 0;
		UserEquipment* ue2;
		if (cc < 1)
			ue2 = networkManager->CreateUserEquipment(nodeId++, cell, 0.290, 0.0, speed, direction, enb);
		else
			ue2 = networkManager->CreateUserEquipment(nodeId++, cell, speed, enb);

		//Create Application
		Application::ApplicationType appType = Application::APPLICATION_TYPE_VOIP;
		Application* app = flowsManager->CreateApplication(appId++, ue1, ue2, appType, 0.0, 70);
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	simulator->SetStop(73);
	simulator->Run();

	printQOE(1, numberOfVoipTransmitters);

	std::cout << std::endl << "Simulation executed in " << (int) (time(NULL) - programStartTime) << " seconds." << std::endl;

	return 0;
}

void printQOE(int firstApp, int lastApp)
{
	std::vector<Application*>* apps = FlowsManager::Init()->GetApplicationsContainer();
	std::cout << std::endl;
	double totalMos = 0;
	for (int p = firstApp-1 ; p < lastApp; p ++)
	{
		double totalDelay = apps->at(p)->GetApplicationSink()->GetQoeManager()->GetTotalDelay();
		int totalReceivedPackets = apps->at(p)->GetApplicationSink()->GetQoeManager()->GetTotalReceivedPackets();
		int totalSentPackets = apps->at(p)->GetApplicationSink()->GetQoeManager()->GetTotalSentPackets();
		double delay =  totalDelay/totalReceivedPackets;
		double packetLoss = 1 - ((double) totalReceivedPackets / totalSentPackets);
		double mos = floor(100*MOS(1000*delay, packetLoss))/100;
		totalMos += mos;
		std::cout << apps->at(p)->GetApplicationID() << " " << std::setprecision(3) << delay
			<< " " << packetLoss << " " << std::setprecision(2) << mos << std::endl;
	}
	std::cout << std::endl << "TOT " << totalMos/(lastApp-firstApp+1) << std::endl;
}
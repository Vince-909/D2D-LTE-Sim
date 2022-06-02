#ifndef UPLINKPACKETSCHEDULER_H_
#define UPLINKPACKETSCHEDULER_H_

#include "PacketScheduler.h"
#include <vector>

class NetworkNode;

class UplinkPacketScheduler : public PacketScheduler
{

public:

	UplinkPacketScheduler();
	~UplinkPacketScheduler();

	struct UserToSchedule
	{
	    NetworkNode* m_userToSchedule;
	    int m_transmittedData;	//bytes
	    int m_dataToTransmit;		//bytes
	    double m_averageSchedulingGrant; // in bytes

	    std::vector<int> m_listOfAllocatedRBs;
	    int m_selectedMCS;
	    std::vector<double> m_channelContition; //similar to the CQI for the DL
	};

	typedef std::vector<UserToSchedule*> UsersToSchedule;

	void CreateUsersToSchedule(void);
	void DeleteUsersToSchedule(void);
	void ClearUsersToSchedule(void);
	UsersToSchedule* GetUsersToSchedule(void);

	void SelectUsersToSchedule(void);

	virtual void DoSchedule(void);
	virtual void DoStopSchedule(void);

	virtual void RBsAllocation();

private:

	UsersToSchedule* m_usersToSchedule;

};

#endif
#ifndef ENODEB_H_
#define ENODEB_H_

#include "NetworkNode.h"
#include <vector>

class UserEquipment;
class PacketScheduler;
class UserEquipment;

class ENodeB : public NetworkNode
{

public:

	struct UserEquipmentRecord
	  {
		UserEquipmentRecord();
		virtual ~UserEquipmentRecord();
		UserEquipmentRecord(UserEquipment*);

		UserEquipment* m_UE;
		void SetUE(UserEquipment*);
        UserEquipment* GetUE(void);

        std::vector<int> m_cqiFeedback;
		void SetCQI(std::vector<int>);
		std::vector<int> GetCQI(void);

		int m_schedulingRequest; // in bytes
		void SetSchedulingRequest(int);
		int GetSchedulingRequest(void);

		int m_averageSchedulingGrants; // in bytes
		void UpdateSchedulingGrants(int);
		int GetSchedulingGrants(void);

		double m_qoe;
		void SetQoe(double);
		double GetQoe(void);

		std::vector<double> m_uplinkChannelStatusIndicator;
		void SetUplinkChannelStatusIndicator(std::vector<double>);
		std::vector<double> GetUplinkChannelStatusIndicator(void);

	  };

	typedef std::vector<UserEquipmentRecord*> UserEquipmentRecords;
	
	enum DLSchedulerType
	{
	    DLScheduler_TYPE_PROPORTIONAL_FAIR
	 };
	enum ULSchedulerType
	{
		ULScheduler_TYPE_ROUNDROBIN,
		ULScheduler_TYPE_D2DFAIR
	};

	ENodeB();
	~ENodeB();

	void RegisterUserEquipment(UserEquipment*);
	UserEquipmentRecords* GetUserEquipmentRecords(void);
	UserEquipmentRecord* GetUserEquipmentRecord(int);
	int GetNbOfUserEquipmentRecords(void);

	void ResourceBlocksAllocation(void);
	void DownlinkResourceBlockAllocation(void);
	void UplinkResourceBlockAllocation(void);

	PacketScheduler* GetDLScheduler(void);
    void SetDLScheduler (DLSchedulerType type);
    
	PacketScheduler* GetULScheduler(void);
    void SetULScheduler (ULSchedulerType type);

	bool CheckD2DAvailability(void);
	void SetD2DAvailability(bool);

	void Visualize();

private:

	UserEquipmentRecords *m_userEquipmentRecords;
	bool useD2DCommunications;

};

#endif

#ifndef PACKETSCHEDULER_H_
#define PACKETSCHEDULER_H_

#include <vector>

class MacEntity;
class RadioBearer;

class PacketScheduler
{

public:

	PacketScheduler();
	~PacketScheduler();
	void Destroy(void);

	void Schedule(void);
	virtual void DoSchedule(void);
	
	void StopSchedule(void);
	virtual void DoStopSchedule(void);

	struct FlowToSchedule
	  {
	    FlowToSchedule(RadioBearer*,int);
	    virtual ~FlowToSchedule();
	    RadioBearer* m_bearer;
	    int m_allocatedBits;		//bits
	    int m_transmittedData;	//bytes
	    int m_dataToTransmit;		//bytes

	    std::vector<double> m_spectralEfficiency;
	    std::vector<int> m_listOfAllocatedRBs;
	    std::vector<int> m_listOfSelectedMCS;
	    std::vector<int> m_cqiFeedbacks;

		RadioBearer* GetBearer (void);

		void UpdateAllocatedBits(int);
		int GetAllocatedBits(void);
		int GetTransmittedData (void);
		void SetDataToTransmit(int);
		int GetDataToTransmit(void);

		void SetSpectralEfficiency (std::vector<double>);
		std::vector<double> GetSpectralEfficiency(void);

		std::vector<int>* GetListOfAllocatedRBs();
		std::vector<int>* GetListOfSelectedMCS();

		void SetCqiFeedbacks (std::vector<int>);
		std::vector<int> GetCqiFeedbacks(void);

	  };

	typedef std::vector<FlowToSchedule*> FlowsToSchedule;

	void CreateFlowsToSchedule(void);
	void DeleteFlowsToSchedule(void);
	void ClearFlowsToSchedule(void);

	FlowsToSchedule* GetFlowsToSchedule(void);
	void InsertFlowToSchedule(RadioBearer*, int, std::vector<double>, std::vector<int>);

	MacEntity* GetMacEntity(void);
	void SetMacEntity(MacEntity*);

private:

	MacEntity* m_mac;
	FlowsToSchedule *m_flowsToSchedule;

};

#endif
#ifndef D2DMANAGER_H_
#define D2DMANAGER_H_

#include "../Icmp/D2DIdealControlMessage.h"

class NetworkNode;
class UserEquipment;
struct TestData;

class D2DManager
{

public:

	D2DManager();
	~D2DManager();

	void TestD2DCommunication(UserEquipment*, UserEquipment*);
	void ReceiveD2DIdealControlMessage(D2DIdealControlMessage*);
	double DoD2DTest(D2DIdealControlMessage::TestData);
	void StartD2DCommunication(UserEquipment*, UserEquipment*);
	void StopD2DCommunication(UserEquipment*, UserEquipment*);

	bool D2DinUse(void);

	void ReportTestResults(void);
	
	NetworkNode* GetDevice(void);
	void SetDevice(NetworkNode*);

	void SetNeedForD2DTest(bool);
	bool D2DTestIsNeeded(void);

	void SetD2DTarget(UserEquipment*);
	UserEquipment* GetD2DTarget(void);

private:

	NetworkNode* m_device;
	double m_lastTestResult;
	double m_lastQoeReport;
	bool d2dEnabled;
	bool D2DTestNeeded;
	UserEquipment* m_D2Dtarget;
};

#endif
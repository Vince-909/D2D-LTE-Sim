#ifndef D2DIDEALCONTROLMESSAGE_H_
#define D2DIDEALCONTROLMESSAGE_H_

#include "IdealControlMessage.h"
#include <vector>

class UserEquipment;

class D2DIdealControlMessage : public IdealControlMessage
{
	
public:

	enum D2DMessageType
	{
		NEED_TEST,
		TEST_DATA,
		TEST_RESULT,
		D2D_ENABLE,
		D2D_DISABLE,
		CHANNEL_MAP
	};

	enum D2DRole
	{
		SENDER,
		RECEIVER
	};

	struct TestData
	{
		UserEquipment* sourceUe;
		std::vector<int> channelsTx;
		std::vector<int> mcsIndexTx;
		std::vector<double> txSignalValues;
		UserEquipment* destinationUe;
		double result;
	};

	D2DIdealControlMessage();
	~D2DIdealControlMessage();

	D2DMessageType GetD2DMessageType(void);
	void SetD2DMessageType(D2DMessageType);

	TestData GetTestData(void);
	void SetTestData(UserEquipment*, std::vector<int>, std::vector<int>, std::vector<double>);
	void SetTestData(double);
	void SetTestData(UserEquipment*);

	D2DRole GetD2DRole(void);
	void SetD2DRole(D2DRole);

	std::vector<int>* GetMcs(void);
	void SetMcs(std::vector<int>*);

	std::vector<int>* GetChannels(void);
	void SetChannels(std::vector<int>*);

private:

	D2DMessageType m_d2dType;
	TestData m_testData;
	D2DRole m_D2DRole;
	std::vector<int>* channelstoReceive;
	std::vector<int>* mcsIndextoReceive;

};

#endif
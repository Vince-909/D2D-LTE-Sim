#ifndef POWERMANAGER_H_
#define POWERMANAGER_H_

class LtePhy;
class PowerControlIdealControlMessage;

class PowerManager
{

public:

	PowerManager();
	~PowerManager();

	LtePhy* GetPhy(void);
	void SetPhy(LtePhy*);

	void SetTxPower(void);

	void SetTxPowerForCellular(double);
	double GetTxPowerForCellular(void);

	double GetTxPowerForD2D(void);
	void SetTxPowerForD2D(double);

	void ReceivePowerControlIdealControlMessage(PowerControlIdealControlMessage*);

private:

	LtePhy* m_phy;
	double m_txPowerForCellular;
	double m_txPowerForD2D;

};

#endif
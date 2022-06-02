#ifndef POSITION_H_
#define POSITION_H_

class Mobility;
class NetworkNode;

class Position
{

public:

	Position(double, double);
	Position(double, double, double, double, double, double);
	~Position();

	double GetPosX();
	double GetPosY();

	void SetPosX(double);
	void SetPosY(double);

	Mobility* GetMobility();
	void SetMobility(Mobility*);

	double CalculateDistance(NetworkNode*);

private:

	double m_posX;
	double m_posY;
	Mobility* m_mob;

};

#endif

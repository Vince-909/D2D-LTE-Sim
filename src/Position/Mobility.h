#ifndef MOBILITY_H_
#define MOBILITY_H_

class Position;

class Mobility
{

public:

	Mobility(double, double, double, double, Position*);
	~Mobility();

	double GetInnerBound();
	double GetOuterBound();
	Position* GetPosition();
	void SetPosition(Position*);

	double GetSpeed(void);

	void Randomize();
	void Move();

private:

	double m_speed;				//kph
	double m_direction;			//rad
	double m_straightTravel;	//km
	double m_maxStraightTravel;	//km
	double m_innerBound;		//km
	double m_outerBound;		//km
	Position* m_pos;

};

#endif

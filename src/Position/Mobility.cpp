#include "Mobility.h"
#include <cstdlib>
#include <cmath>
#include "Position.h"

Mobility::Mobility(double speed, double direction, double outerBound, double innerBound, Position* pos)
{
	if (speed > 120)  m_speed = 120;
	else m_speed = speed;
	m_direction = direction;
	m_innerBound = innerBound;
	m_outerBound = outerBound;
	m_straightTravel = 0;
	if (m_speed == 0) m_maxStraightTravel = 1.0;
	else if (m_speed <= 3) m_maxStraightTravel = 0.2;
	else if (m_speed <= 30) m_maxStraightTravel = 0.4;
	else m_maxStraightTravel = 1.0;
	m_pos = pos;
}

Mobility::~Mobility()
{
}

double Mobility::GetInnerBound()
{
	return m_innerBound;
}

double Mobility::GetOuterBound()
{
	return m_outerBound;
}

Position* Mobility::GetPosition()
{
	return m_pos;
}

void Mobility::SetPosition(Position* pos)
{
	m_pos = pos;
}

void Mobility::Randomize()
{
	//Pick a random position taking into account cell radius
	//and minimum distance from ENodeB
	int ob = (int)(GetOuterBound()*1000);
	int ib = (int)(GetInnerBound()*1000);
	double x = (rand()%(ob-ib))+1+ib;
	double y = rand()%(int)(floor(sqrt((ob*ob-x*x)))-ib)+1+ib;
	if (rand()%2 == 0) x *= -1;
	if (rand()%2 == 0) y *= -1;
	m_pos->SetPosX(x/1000);
	m_pos->SetPosY(y/1000);
	m_direction = rand()%6282+1;
	m_direction /= 1000.0;
}

void Mobility::Move()
{
	long double distanceToMove = m_speed/3600000; //for 1ms frame
	if (distanceToMove+m_straightTravel > m_maxStraightTravel)
	{
		m_direction = rand()%6282+1;
		m_direction /= 1000.0;
		m_straightTravel = 0.0;
	}

	double distanceToMoveX = distanceToMove*cos(m_direction);
	double distanceToMoveY = distanceToMove*sin(m_direction);
	//Check if new position is legal.
	//If not, change direction and recalculate.
	while ((sqrt(pow(distanceToMoveX+m_pos->GetPosX(),2)+pow(distanceToMoveY+m_pos->GetPosY(),2)) < GetInnerBound()) ||
		(sqrt(pow(distanceToMoveX+m_pos->GetPosX(),2)+pow(distanceToMoveY+m_pos->GetPosY(),2)) > GetOuterBound()))
	{
		m_direction = rand()%6282+1;
		m_direction /= 1000.0;

		distanceToMoveX = distanceToMove*cos(m_direction);
		distanceToMoveY = distanceToMove*sin(m_direction);
	}

	m_pos->SetPosX(distanceToMoveX+m_pos->GetPosX());
	m_pos->SetPosY(distanceToMoveY+m_pos->GetPosY());
	m_straightTravel += distanceToMove;
}

double Mobility::GetSpeed(void)
{
	return m_speed;
}
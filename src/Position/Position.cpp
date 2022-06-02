#include "Position.h"
#include <iostream>
#include "Mobility.h"
#include "../Device/NetworkNode.h"

Position::Position(double x, double y)
{
	m_posX = x;
	m_posY = y;
	m_mob = NULL;
}

Position::Position(double x, double y, double speed, double direction, double outerBound, double innerBound)
{
	m_posX = x;
	m_posY = y;
	m_mob = new Mobility(speed, direction, outerBound, innerBound, this);
}

Position::~Position()
{
	if (m_mob != NULL) delete m_mob;
}

double Position::GetPosX()
{
	return m_posX;
}

double Position::GetPosY()
{
	return m_posY;
}

Mobility* Position::GetMobility()
{
	return m_mob;
}

void Position::SetPosX(double x)
{
	m_posX = x;
}

void Position::SetPosY(double y)
{
	m_posY = y;
}

void Position::SetMobility(Mobility* mob)
{
	m_mob = mob;
}

double Position::CalculateDistance(NetworkNode* node)
{
	double a = m_posX - node->GetPosition()->GetPosX();
	double b = m_posY - node->GetPosition()->GetPosY();
	a = a*a;
	b = b*b;
	return sqrt(a+b);
}
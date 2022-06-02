#include "Cell.h"
#include <iostream>
#include "../Position/Position.h"

Cell::Cell()
{
	m_position = NULL;
}

Cell::~Cell()
{
	delete m_position;
}

int Cell::GetIdCell()
{
	return m_idCell;
}

void Cell::SetIdCell(int id)
{
	m_idCell = id;
}

double Cell::GetRadius()
{
	return m_radius;
}

void Cell::SetRadius(double r)
{
	m_radius = r;
}

double Cell::GetMinDistance()
{
	return m_minDistance;
}

void Cell::SetMinDistance(double min)
{
	m_minDistance = min;
}

Position* Cell::GetPosition()
{
	return m_position;
}

void Cell::SetPosition(double x, double y)
{
	m_position = new Position(x, y);
}
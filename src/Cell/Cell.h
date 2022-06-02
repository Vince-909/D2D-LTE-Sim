#ifndef CELL_H_
#define CELL_H_

class Position;

class Cell
{

public:

	Cell();
	~Cell();

	int GetIdCell(void);
	void SetIdCell(int);

	double GetRadius(void);
	void SetRadius(double);

	double GetMinDistance(void);
	void SetMinDistance(double);

	Position* GetPosition(void);
	void SetPosition(double, double);

	void Visualize();

private:

	int m_idCell;
	double m_radius;		// Km
	double m_minDistance;	// km
	Position* m_position;

};

#endif

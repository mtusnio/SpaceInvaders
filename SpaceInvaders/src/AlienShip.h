#ifndef H_ALIENSHIP
#define H_ALIENSHIP

#include "Ship.h"


class AlienShip : public Ship
{
public:
	AlienShip();
	virtual ~AlienShip() { };

	// How many points the ship is worth
	void SetPoints(int points) { m_Points = points; };
	int GetPoints() const { return m_Points; };

	virtual void Destroy();
private:
	int m_Points;
};


#endif
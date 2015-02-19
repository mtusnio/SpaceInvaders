#ifndef H_PLAYERSHIP
#define H_PLAYERSHIP

#include "Ship.h"

class PlayerShip : public Ship
{
public:
	PlayerShip() { };
	virtual ~PlayerShip() { };

	virtual void Damage(unsigned int dmg);
};

#endif
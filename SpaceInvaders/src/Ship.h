#ifndef H_SHIP
#define H_SHIP

#include "Entity.h"

class Weapon;

class Ship : public Entity
{
public:
	Ship();
	virtual ~Ship();

	Weapon * GetWeapon() const { return m_Weapon; };
	void SetWeapon(Weapon * wp);

	virtual void Destroy();

	
private:
	Weapon * m_Weapon;
};


#endif
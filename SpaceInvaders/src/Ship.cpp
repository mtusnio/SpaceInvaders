#include "Ship.h"
#include "Weapon.h"

Ship::Ship()
{
	m_Weapon = nullptr;
}

Ship::~Ship()
{
	if(m_Weapon)
		delete m_Weapon;
}


void Ship::Destroy()
{
	Entity::Destroy();

	//TODO: Explosion effects
}



void Ship::SetWeapon(Weapon * weapon)
{
	_ASSERT(weapon->GetOwner() == nullptr);

	if(m_Weapon)
		delete m_Weapon;

	if(weapon)
	{
		weapon->SetOwner(this);
		m_Weapon = weapon;
	}
}
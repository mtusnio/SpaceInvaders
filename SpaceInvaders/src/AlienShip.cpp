#include "AlienShip.h"

#include "IScene.h"

AlienShip::AlienShip()
{
	m_Points = 0;
}

void AlienShip::Destroy()
{
	GetScene()->SendGameEvent("alien_destroyed", this);

	Ship::Destroy();
}
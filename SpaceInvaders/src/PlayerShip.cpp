#include "PlayerShip.h"
#include "IScene.h"

void PlayerShip::Damage(unsigned int dmg)
{
	GetScene()->SendGameEvent("player_damaged", this);
}
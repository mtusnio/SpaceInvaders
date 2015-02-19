#include "Projectile.h"

#include "IScene.h"
#include "Console.h"

Projectile::Projectile()
{
	m_Speed = 0;
	m_Direction = sf::Vector2f(1.0f, 0.0f);
}

void Projectile::Configure(const sf::Vector2f & dir, int speed, int damage)
{
	m_Speed = speed;
	m_Direction = dir;
	m_Damage = damage;
}

void Projectile::RunPreFrame()
{
	sf::Time diff = GetScene()->GetDeltaTime();

	sf::Vector2f moveVec = m_Direction * ((float)m_Speed * diff.asSeconds());
	move(moveVec);
}

void Projectile::Collide(Entity * entity)
{
	entity->Damage(m_Damage);
	Destroy();

	Console::LogDebug("Projectile collided. Projectile collision group: " + std::to_string(GetCollisionGroup()) + " and obj's collision group: " + std::to_string(entity->GetCollisionGroup()));
}


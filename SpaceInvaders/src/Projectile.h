#ifndef H_PROJECTILE
#define H_PROJECTILE

#include "Entity.h"


class Projectile : public Entity
{
public:
	Projectile();
	virtual ~Projectile() { };

	virtual void Configure(const sf::Vector2f & dir, int speed, int damage);

	virtual void Collide(Entity * collider);

	virtual void RunPreFrame();
private:
	int m_Speed;
	int m_Damage;
	sf::Vector2f m_Direction;
};


#endif
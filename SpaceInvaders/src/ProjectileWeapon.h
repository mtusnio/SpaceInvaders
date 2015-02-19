#ifndef H_PROJECTILEWEAPON
#define H_PROJECTILEWEAPON

#include "Weapon.h"

namespace sf
{
	class Texture;
};

class ProjectileWeapon : public Weapon
{
public:
	ProjectileWeapon();
	virtual ~ProjectileWeapon() { };

	virtual void ConfigureProjectiles(sf::Texture * tex, int projSpeed, float xScale = 1.0f, float yScale = 1.0f);

	virtual void Fire();

private:
	sf::Texture * m_ProjTex;
	int m_ProjSpeed;
	float m_ProjScale[2];
};



#endif
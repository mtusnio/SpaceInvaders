#include "ProjectileWeapon.h"

#include <SFML/Graphics.hpp>

#include "Ship.h"
#include "IScene.h"
#include "Projectile.h"

ProjectileWeapon::ProjectileWeapon()
{
	m_ProjTex = nullptr;
	m_ProjScale[0] = 1.0f;
	m_ProjScale[1] = 1.0f;
	m_ProjSpeed = 0;
}

void ProjectileWeapon::Fire()
{
	Ship * owner = GetOwner();
	_ASSERT(owner != nullptr && owner->GetScene() != nullptr);

	sf::Vector2f forw = owner->GetForwardVector();

	Projectile * proj = new Projectile();
	proj->setTexture(*m_ProjTex);
	proj->setScale(m_ProjScale[0], m_ProjScale[1]);

	auto bounds = proj->getGlobalBounds();
	proj->setOrigin(bounds.width/2.0f, bounds.height/2.0f);

	proj->setPosition(owner->getPosition());

	proj->Configure(forw, m_ProjSpeed, 1 /* Damage */);
	proj->SetCollisionGroup(owner->GetCollisionGroup());

	owner->GetScene()->AddEntity(proj);
}

void ProjectileWeapon::ConfigureProjectiles(sf::Texture * tex, int projSpeed, float xScale /* = 1.0f */, float yScale /* = 1.0f */)
{
	m_ProjTex = tex;
	m_ProjSpeed = projSpeed;
	m_ProjScale[0] = xScale;
	m_ProjScale[1] = yScale;
}
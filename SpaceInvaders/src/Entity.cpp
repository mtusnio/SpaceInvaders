#include "Entity.h"

#include <math.h> 

#include "IScene.h"

Entity::Entity()
{
	m_EntID = INVALID_ID;
	m_ParentScene = nullptr;
	m_CollisionGroup = -1;
	m_Health = INVULNERABLE;
}

void Entity::Init(const sf::Texture & texture, float scaleX, float scaleY)
{
	setTexture(texture);
	setScale(scaleX, scaleY);

	auto bounds = getLocalBounds();
	setOrigin(bounds.width/2.0f, bounds.height/2.0f);	
}

bool Entity::IsColliding(Entity * ent) const
{
	_ASSERT(ent != nullptr);

	if(GetCollisionGroup() == -1 || ent->GetCollisionGroup() == -1 || ent->GetCollisionGroup() == GetCollisionGroup())
		return false;

	return getGlobalBounds().intersects(ent->getGlobalBounds());
}

void Entity::Collide(Entity * ent)
{

}


void Entity::Destroy()
{
	_ASSERT(m_ParentScene);

	m_ParentScene->DeleteEntity(this);
}


void Entity::Damage(unsigned int dmg)
{
	if(IsInvulnerable())
		return;

	if(m_Health <= dmg)
	{
		m_Health = 0;
		Destroy();
		return;
	}

	m_Health = m_Health - dmg;
}

sf::Vector2f Entity::GetForwardVector() const
{
	float rot = getRotation() - 90;
	return sf::Vector2f(cosf(rot * (float)M_PI/180.0f), -sinf(rot * (float)M_PI/180.0f));
}
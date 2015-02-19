#ifndef H_ENTITY
#define H_ENTITY

#include <SFML/Graphics.hpp>

class IScene;

class Entity : public sf::Sprite
{
public:
	// Type to hold unique entity IDs. Can be safely incremented by 1
	// starting with FIRST_ID. With the amount of objects we use
	// we should never out of valid IDs.
	typedef unsigned long long int ID;
	static const ID INVALID_ID = 0;
	static const ID FIRST_ID = 1;
	static const unsigned int INVULNERABLE = 0;

	Entity();
	virtual ~Entity() { };


	// Helper method for initialising entities. This method will set the object's origin to its
	// centre after adding texture & scaling
	virtual void Init(const sf::Texture & texture, float scaleX = 1.0f, float scaleY = 1.0f);

	void SetScene(IScene * scene, ID id) { m_ParentScene = scene; m_EntID = id; }

	// Returns the (unique in this scene) ID of the object.
	ID GetID() { return m_EntID; }

	// Returns the scene this entity is currently assigned to
	IScene * GetScene() const { return m_ParentScene; }

	// Both functions are ran exactly once every frame.
	// RunPostFrame is ran after PreFrame has been run,
	// as well as after any frame simulation logic has happened.
	virtual void RunPreFrame() { };
	virtual void RunPostFrame() { };
	
	// Returns true if this entity collides with ent.
	virtual bool IsColliding(Entity * ent) const;
	
	// This function is ran every frame when this
	// entity collides with collider
	virtual void Collide(Entity * collider);

	// Destroys the object and performs all actions associated with that.
	// If you simply want to remove an entity from the scene and avoid
	// any additional behaviour associated with this function use
	// IScene's RemoveEntity/DeleteEntity methods
	virtual void Destroy();

	// Collision group number. This object will ignore collisions with all objects of the
	// same collision group. -1 means the object should not collide with anything at all
	void SetCollisionGroup(int num) { m_CollisionGroup = num; };
	int GetCollisionGroup() const { return m_CollisionGroup; };


	// Changes the health of the object. Setting health to the INVULNERABLE const
	// will make it undamageable
	unsigned int GetHealth() const { return m_Health; }
	void SetHealth(int health) { m_Health = health; }
	bool IsInvulnerable() const { return m_Health == INVULNERABLE; };

	// Damages the entity, if health goes below or hits 0 the object is destroyed
	virtual void Damage(unsigned int dmg);


	sf::Vector2f GetForwardVector() const;
private:
	int m_CollisionGroup;

	ID m_EntID;

	IScene * m_ParentScene;

	unsigned int m_Health;
};


#endif
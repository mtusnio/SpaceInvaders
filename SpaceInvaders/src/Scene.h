#ifndef H_SCENE
#define H_SCENE

#include <SFML/System/Clock.hpp>

#include <unordered_map>

#include "IScene.h"
#include "TextureCacher.h"
#include "Entity.h"

// Basic implementation of the IScene interface, offers collision detection,
// entity and time handling, as well as basic frame simulation
class Scene : public IScene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Clear();

	virtual void Init();

	virtual void StartFrame();
	virtual void Simulate();
	virtual void EndFrame();

	virtual std::vector<Entity*> GetEntities() const;

	virtual void AddEntity(Entity * ent);
	virtual bool RemoveEntity(Entity * ent);
	virtual bool DeleteEntity(Entity * ent);

	virtual void HandleEvent(const sf::Event & event);

	virtual sf::Time GetDeltaTime() const { return m_Delta; };
	virtual sf::Time GetElapsedTime() const { return m_Clock.getElapsedTime(); };

	virtual TextureCacher & GetTextureCache() const { return *m_TextureCache; }
protected:
	enum KeyState
	{
		RELEASED,
		RELEASED_THIS_FRAME,
		PRESSED,
		PRESSED_THIS_FRAME,
	};

	// Returns true if the entity, although it's present on the entity list, is going to be
	// removed in the next cleanup phase
	inline bool IsAwaitingRemoval(Entity * ent) const;


	TextureCacher * m_TextureCache;
	std::vector<Entity*> m_Entities;

private:
	virtual void RunSceneLogic() = 0;

	void CheckCollisions();
	void DeletedEntitiesCleanup();

	sf::Clock m_Clock;

	sf::Time m_FrameStart;
	sf::Time m_Delta;


	// Entities to remove/delete at cleanup stages, after collisions, post/pre frame functions
	// have already been resolved
	std::unordered_map<Entity::ID, Entity*> m_ToRemove;
	std::unordered_map<Entity::ID, Entity*> m_ToDelete;

	Entity::ID m_NextID;
};


inline bool Scene::IsAwaitingRemoval(Entity * ent) const
{
	return m_ToRemove.find(ent->GetID()) != m_ToRemove.end();
}

#endif
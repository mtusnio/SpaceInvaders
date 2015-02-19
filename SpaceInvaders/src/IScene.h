#ifndef H_ISCENE
#define H_ISCENE

#include <vector>

namespace sf
{
	class Sprite;
	class Event;
}

class Entity;
class IUserInterface;

class IScene
{
public:
	virtual ~IScene() {};

	// Initialisation of the scene.
	// Should be run before any simulation is done,
	// otherwise the scene might not work properly
	// due to missing components
	virtual void Init() = 0;

	// Initialises state for the beginning of the frame, called before
	// Simulate
	virtual void StartFrame() = 0;

	// Simulates one frame
	virtual void Simulate() = 0;

	// Finishes the frame, should be called after Simulate and all renderer
	// actions have finished
	virtual void EndFrame() = 0;

	// Returns a vector of all entities associated with this scene
	virtual std::vector<Entity*> GetEntities() const = 0;

	// Associate an entity with this scene
	virtual void AddEntity(Entity * ent) = 0;

	// Removes the entity from this scene but does not delete it.
	// Useful if you want to readd it later or add it to 
	// a different scene
	// Returns true if object was found and removed,
	// false otherwise
	virtual bool RemoveEntity(Entity * ent) = 0;

	// Returns true if object was found and deleted,
	// false otherwise
	virtual bool DeleteEntity(Entity * ent) = 0;

	// Handles input/output events
	virtual void HandleEvent(const sf::Event & event) = 0;


	// Returns time it took to render previous frame
	virtual sf::Time GetDeltaTime() const = 0;

	// Returns total time since the beginning of the scene
	virtual sf::Time GetElapsedTime() const = 0;

	// Sends an event for the scene to handle
	virtual void SendGameEvent(const std::string & eventName, Entity * sender) = 0; 
};


#endif
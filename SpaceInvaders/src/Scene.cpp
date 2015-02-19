#include "Scene.h"
#include "Console.h"

Scene::Scene()
{
	m_NextID = Entity::FIRST_ID;
	m_TextureCache = new TextureCacher();
}

Scene::~Scene()
{
	if(m_TextureCache)
		delete m_TextureCache;

	Clear();
}

void Scene::Init()
{
	Clear();
	m_Clock.restart();
	m_FrameStart = m_FrameStart.Zero;
}

void Scene::Clear()
{
	for(auto it = m_Entities.begin(); it != m_Entities.end(); it++)
	{
		delete *it;
	}

	m_Entities.clear();
}

void Scene::StartFrame()
{
	m_FrameStart = m_Clock.getElapsedTime();
}


void Scene::Simulate()
{
	for(Entity * x : m_Entities) 
	{ 
		if(!IsAwaitingRemoval(x))
			x->RunPreFrame(); 
	}

	CheckCollisions();
	DeletedEntitiesCleanup();

	RunSceneLogic();

	for(Entity * x : m_Entities)
	{ 
		if(!IsAwaitingRemoval(x))
			x->RunPostFrame(); 
	}

	DeletedEntitiesCleanup();
}

void Scene::EndFrame()
{
	m_Delta = m_Clock.getElapsedTime() - m_FrameStart;
}

std::vector<Entity*> Scene::GetEntities() const
{
	// Clear the list of all entities awaiting removal before we return it
	std::vector<Entity*> ents = m_Entities;

	ents.erase(std::remove_if(ents.begin(), ents.end(), [this] (Entity * ent)
	{
		return m_ToRemove.find(ent->GetID()) != m_ToRemove.end();
	}), ents.end());

	return ents;
}

void Scene::AddEntity(Entity * ent)
{
	_ASSERT(ent != nullptr);

	if(ent->GetScene())
		ent->GetScene()->RemoveEntity(ent);

	if(std::find(m_Entities.begin(), m_Entities.end(), ent) != m_Entities.end())
		return;

	m_Entities.push_back(ent);
	ent->SetScene(this, m_NextID);
	m_NextID++;
}

bool Scene::RemoveEntity(Entity * ent)
{
	_ASSERT(ent != nullptr);

	auto it = std::find(m_Entities.begin(), m_Entities.end(), ent);
	if(it == m_Entities.end())
		return false;

	if(IsAwaitingRemoval(ent))
		return false;

	m_ToRemove[ent->GetID()] = ent;

	return true;
}

bool Scene::DeleteEntity(Entity * ent)
{
	if(RemoveEntity(ent))
	{
		m_ToDelete[ent->GetID()] = ent;
		return true;
	}

	return false;
}


void Scene::HandleEvent(const sf::Event & event)
{

}


void Scene::CheckCollisions()
{
	
	for(Entity * ent : m_Entities)
	{
		if(IsAwaitingRemoval(ent))
			continue;

		for(Entity * col : m_Entities)
		{
			if(ent == col || IsAwaitingRemoval(col))
				continue;

			if(ent->IsColliding(col))
			{
				ent->Collide(col);

				// We should stop colliding if we get flagged for removal
				if(IsAwaitingRemoval(ent))
					break;
			}
		}

	}
	
}


void Scene::DeletedEntitiesCleanup()
{
	m_Entities.erase(std::remove_if(m_Entities.begin(), m_Entities.end(), [this] (Entity * ent)
	{
		return m_ToRemove.find(ent->GetID()) != m_ToRemove.end();
	}), m_Entities.end());

	for(std::pair<Entity::ID, Entity*> ent : m_ToDelete)
	{
		delete ent.second;
	}

	m_ToRemove.clear();
	m_ToDelete.clear();
}
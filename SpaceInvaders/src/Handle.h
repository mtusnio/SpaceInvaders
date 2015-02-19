#ifndef H_HANDLE
#define H_HANDLE

#include "Entity.h"
#include "IScene.h"


// Handle for entities, will return null if its entity had been removed from the scene at some point
// Current implementation's Get() function is O(n), but with some changes to the scene class this can easily be O(1)
template<class T>
class Handle
{
public:
	Handle();
	Handle(T * obj);
	virtual ~Handle();

	virtual T * Get() const;
	virtual void Set(T * obj);

	operator T*() const { return Get(); }

	T * operator->() const { return Get(); };
	T & operator*() const { return *Get(); };

	Handle<T> & operator=(T * obj) { Set(obj); return *this; };
	Handle<T> & operator=(const Handle<T> & handle) { Set(handle.Get()); return *this; };
	
	bool operator==(T * obj) const { return Get() == obj; };
	bool operator!=(T * obj) const { return !(*this == obj); };

	bool operator!() const { return Get() == nullptr; };
private:
	Entity::ID m_ID;
	IScene * m_Scene;
};

template<class T>
Handle<T>::Handle()
{
	Set(nullptr);
}

template<class T>
Handle<T>::Handle(T * obj)
{
	Set(obj);	
}

template<class T>
Handle<T>::~Handle()
{

}

template<class T>
T * Handle<T>::Get() const
{
	if(m_ID == Entity::INVALID_ID || m_Scene == nullptr)
		return nullptr;

	std::vector<Entity*> entVec = m_Scene->GetEntities();

	auto it = std::find_if(entVec.begin(), entVec.end(), [this] (Entity * ent)
	{
		return (ent->GetID() == m_ID);
	});
	
	if(it != entVec.end())
		return static_cast<T*>(*it);

	return nullptr;
}

template<class T>
void Handle<T>::Set(T * obj)
{
	if(obj)
	{
		m_ID = obj->GetID();
		m_Scene = obj->GetScene();
	}
	else
	{
		m_ID = Entity::INVALID_ID;
		m_Scene = nullptr;
	}
}

#endif
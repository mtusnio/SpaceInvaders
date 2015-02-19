#include "UserInterface.h"

#include <algorithm>
#include <SFML/Graphics.hpp>

UserInterface::UserInterface(sf::RenderWindow & wnd)
{
	m_Window = &wnd;
}

UserInterface::~UserInterface()
{
	Clear();
}


void UserInterface::Clear()
{
	for(sf::Drawable * obj : m_Cache)
	{
		delete obj;
	}

	m_Cache.clear();
	m_Elements.clear();
}

void UserInterface::AddElement(const std::string & name, sf::Drawable * sprite)
{
	// Do not reinsert the same element. Note, if the element doesn't exist
	// this call will create one in the unordered map as nullptr, however
	// this doesn't bother us that much since we already need that spot
	if(m_Elements[name] == sprite)
		return;

	// Ignore results of the previous condition
	if(m_Elements[name] != nullptr)
		RemoveElement(name);

	m_Elements[name] = sprite;
	m_Cache.push_back(sprite);
}

bool UserInterface::RemoveElement(const std::string & name)
{
	if(m_Elements.find(name) == m_Elements.end())
		return false;

	m_Cache.erase(std::remove(m_Cache.begin(), m_Cache.end(), m_Elements[name]), m_Cache.end());
	if(m_Elements[name] != nullptr)
		delete m_Elements[name];
	m_Elements.erase(name);

	return true;
}

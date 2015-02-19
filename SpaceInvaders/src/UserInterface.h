#ifndef H_USERINTERFACE
#define H_USERINTERFACE

#include "IUserInterface.h"

#include <string>
#include <unordered_map>

class IScene;
namespace sf
{
	class RenderWindow;
};

class UserInterface : public IUserInterface
{
public:
	UserInterface(sf::RenderWindow & wnd);
	virtual ~UserInterface();
	
	// Removes & destroys all UI elements
	virtual void Clear();

	virtual std::vector<sf::Drawable*> GetElements() const { return m_Cache; };

	sf::RenderWindow & GetWindow() { return *m_Window; };
protected:
	virtual void AddElement(const std::string & name, sf::Drawable * sprite);
	// Returns true if the element was found and removed, false if it didn't exist
	virtual bool RemoveElement(const std::string & name);
	virtual sf::Drawable * GetElement(const std::string & name) { return m_Elements[name]; };
private:
	std::unordered_map<std::string, sf::Drawable*> m_Elements;
	// Cache for our map to easily return all elements without the underlying map structure
	// and costly conversions
	std::vector<sf::Drawable*> m_Cache;
	sf::RenderWindow * m_Window;
};


#endif
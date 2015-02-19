#ifndef H_IUSERINTERFACE
#define H_IUSERINTERFACE

#include <vector>

namespace sf
{
	class Drawable;
};

class IUserInterface
{
public:
	virtual ~IUserInterface() { };

	// Needs to be called before the interface is used
	virtual void Init() = 0;

	// Returns all UI elements (as sprites) to render
	virtual std::vector<sf::Drawable*> GetElements() const = 0;

	// Method for handling events sent to the UI
	virtual void SendEvent(const std::string & event) = 0;
};

#endif
#ifndef H_SPACEINVADERSUI
#define H_SPACEINVADERSUI

#include "UserInterface.h"

class TextureCacher;
class SpaceInvadersScene;

namespace sf
{
	class RenderWindow;
	class Font;
};

class SpaceInvadersUI : public UserInterface
{
public:
	SpaceInvadersUI(SpaceInvadersScene & scene, sf::RenderWindow & wnd);
	virtual ~SpaceInvadersUI();

	virtual void Init();
	virtual void SendEvent(const std::string & event);
private:
	void CreateLifeCounter(TextureCacher & cache);
	void CreateScoreCounter(TextureCacher & cache);
	void CreateStageCounter(TextureCacher & cache);


	void UpdateScore();
	void UpdateLives();
	void UpdateStage();

	void DisplayMessage(const std::string & headline, const std::string & caption);
	void HideMessage();

	const SpaceInvadersScene * m_Scene;

	// Font we are going to be using in our UI, saves
	// us the hassle of using a cacher
	sf::Font * m_Font;
};

#endif
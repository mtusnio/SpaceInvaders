#include "SpaceInvadersUI.h"

#include <SFML/Graphics.hpp>

#include "SpaceInvadersScene.h"
#include "TextureCacher.h"
#include "Console.h"

SpaceInvadersUI::SpaceInvadersUI(SpaceInvadersScene & scene, sf::RenderWindow & wnd) :
	UserInterface(wnd)
{
	m_Scene = &scene;
	m_Font = nullptr;
}

SpaceInvadersUI::~SpaceInvadersUI()
{
	if(m_Font)
		delete m_Font;

}

void SpaceInvadersUI::Init()
{
	_ASSERT(m_Scene != nullptr);

	Clear();

	if(!m_Font)
	{
		m_Font = new sf::Font();
		m_Font->loadFromFile("NASDAQER.ttf");
	}
	
	TextureCacher & cache = m_Scene->GetTextureCache();
	CreateLifeCounter(cache);
	CreateScoreCounter(cache);
	CreateStageCounter(cache);

	Console::Log("Interface initialised");
}

void SpaceInvadersUI::SendEvent(const std::string & event)
{
	if(event == "alien_destroyed")
		UpdateScore();
	else if(event == "player_damaged")
		UpdateLives();
	else if(event == "defeat")
		DisplayMessage("You have failed!", "Press R to restart");
	else if(event == "victory")
		DisplayMessage("Ooops... they might have actually come in peace.", "Oh well... Press N for the next (and more difficult) stage");
	else if(event == "restart")
	{
		Init();
	}
	else if(event == "next_stage")
	{
		HideMessage();
		UpdateStage();
	}
}

void SpaceInvadersUI::CreateLifeCounter(TextureCacher & cache)
{
	sf::Texture * livesTexture = cache.LoadTexture("sprites/ui_icon_lives.png");

	if(livesTexture == nullptr)
	{
		Console::Log("Could not create lives counter");
		return;
	}
	livesTexture->setRepeated(true);

	sf::Sprite * spr = new sf::Sprite(*livesTexture);
	spr->setScale(0.75f, 0.75f);
	spr->setPosition(5.0f, 5.0f);
	AddElement("lives_indicator", spr);

	UpdateLives();
}

void SpaceInvadersUI::CreateScoreCounter(TextureCacher & cache)
{
	if(!m_Font)
		return;

	sf::Text * txt = new sf::Text();
	txt->setFont(*m_Font);
	txt->setColor(sf::Color::White);
	

	AddElement("score_indicator", txt);
	UpdateScore();
}

void SpaceInvadersUI::CreateStageCounter(TextureCacher & cache)
{
	if(!m_Font)
		return;

	auto size = GetWindow().getSize();
	sf::Text * txt = new sf::Text();
	txt->setCharacterSize(18);
	txt->setFont(*m_Font);
	txt->setColor(sf::Color::White);
	
	txt->setPosition(size.x/2.0f - txt->getGlobalBounds().width/2.0f, 0.0f);
	AddElement("stage_indicator", txt);
	UpdateStage();
}

void SpaceInvadersUI::UpdateScore()
{
	const int MINIMUM_LENGTH = 4;

	sf::Text * txt = static_cast<sf::Text*>(GetElement("score_indicator"));

	if(!txt)
	{
		Console::Log("Score indicator does not exist", Console::Severity::WARNING);
		return;
	}

	int score = m_Scene->GetPointsCount();

	std::string scoreStr = std::to_string(score);

	// Zero padding
	int padding = MINIMUM_LENGTH - (int)scoreStr.length();
	for(int i = 0; i < padding; i++)
	{
		scoreStr = "0" + scoreStr;
	}
	
	auto size = GetWindow().getView().getSize();
	txt->setString(scoreStr);
	txt->setPosition(size.x - txt->getGlobalBounds().width, 0.0f);
}

void SpaceInvadersUI::UpdateLives()
{
	int lives = m_Scene->GetLivesLeft();
	if(lives <= 0)
		return;

	sf::Sprite * spr = static_cast<sf::Sprite*>(GetElement("lives_indicator"));

	if(!spr)
	{
		Console::Log("Lives indicator does not exist", Console::Severity::WARNING);
		return;
	}

	auto size = spr->getTexture()->getSize();
	spr->setTextureRect(sf::IntRect(0, 0, size.x * lives, size.y));
}

void SpaceInvadersUI::UpdateStage()
{
	sf::Text * txt = static_cast<sf::Text*>(GetElement("stage_indicator"));

	if(!txt)
	{
		Console::Log("Stage indicator does not exist", Console::Severity::WARNING);
		return;
	}

	int stage = m_Scene->GetStage() + 1;

	std::string str = "Stage: " + std::to_string(stage);
	txt->setString(str);
}


void SpaceInvadersUI::DisplayMessage(const std::string & headline, const std::string & caption)
{
	HideMessage();
	
	auto size = GetWindow().getView().getSize();

	sf::Text * txt = new sf::Text(headline, *m_Font, 40U);
	txt->setPosition(size.x/2.0f - txt->getGlobalBounds().width/2.0f, size.y/3.0f);
	AddElement("message_headline", txt);

	txt = new sf::Text(caption, *m_Font);
	txt->setPosition(size.x/2.0f - txt->getGlobalBounds().width/2.0f, size.y/2.0f);
	AddElement("message_caption", txt);
}

void SpaceInvadersUI::HideMessage()
{
	RemoveElement("message_headline");
	RemoveElement("message_caption");
}
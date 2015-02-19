#include "SpaceInvadersScene.h"

#include <random>

#include "Console.h"
#include "PlayerShip.h"
#include "ProjectileWeapon.h"
#include "SpaceInvadersUI.h"

SpaceInvadersScene::SpaceInvadersScene(float boundsX, float boundsY)
{
	m_PlayerShip = nullptr;
	m_GameBounds = sf::FloatRect(0, 0, boundsX, boundsY);
	m_StartingShips = 0;
	m_PointsCount = 0;
	m_Lives = 0;
	m_UI = nullptr;
	m_Stage = 0;
}

void SpaceInvadersScene::Init()
{
	Scene::Init();

	//CreateBackground("sprites/ui_bg_main_tile.png");
	CreatePlayerShip(3 /* Starting lives */);
	CreateEnemies(ENEMIES_ROWS, ENEMIES_PER_ROW);
	CreateCover();

	m_GameState = State::PLAYING;
	m_Stage = 0;
	m_PointsCount = 0;

	Console::Log("SpaceInvaders initialised");
}

void SpaceInvadersScene::Clear()
{
	Scene::Clear();

	m_EnemyShips.clear();
}

void SpaceInvadersScene::AddPlayerShip(Ship * ship)
{
	if(ship != nullptr && std::find(m_Entities.begin(), m_Entities.end(), ship) == m_Entities.end())
		AddEntity(ship);

	m_PlayerShip = ship;
}

std::pair<float, float> SpaceInvadersScene::GetPlayerAreaBounds() const
{
	return std::make_pair(25.0f, m_GameBounds.width - 25.0f); 
}

void SpaceInvadersScene::SendGameEvent(const std::string & event, Entity * sender)
{
	_ASSERT(sender != nullptr);

	if(event == "alien_destroyed")
	{
		_ASSERT(typeid(*sender) == typeid(AlienShip));
		
		AlienShip * ship = static_cast<AlienShip*>(sender);
		m_EnemyShips.erase(std::remove(m_EnemyShips.begin(), m_EnemyShips.end(), sender), m_EnemyShips.end());
		m_PointsCount += ship->GetPoints();

		if(m_EnemyShips.size() == 0)
			Win();
	}
	else if(event == "player_damaged")
	{
		m_Lives--;
		if(m_Lives < 0)
		{
			Lose();
		}
	}

	if(m_UI)
		m_UI->SendEvent(event);
}


void SpaceInvadersScene::RunSceneLogic()
{
	CleanEntitiesOutOfBounds();


	HandlePlayerControls();

	if(m_GameState == State::PLAYING)
	{
		MoveFleet();
	
		if(m_NextFleetFire < GetElapsedTime())
			FleetAttack();
	}
}

void SpaceInvadersScene::CreateBackground(const std::string & spritePath)
{
	sf::Texture * bg = m_TextureCache->LoadTexture(spritePath);

	if(!bg)
	{
		Console::Log("Could not load background image, background will not be rendered", Console::Severity::ERROR);
		return;
	}

	bg->setRepeated(true);

	// Use an entity for the background
	Entity * ent = new Entity();

	ent->setTexture(*bg);
	ent->setTextureRect(sf::IntRect(0,0,(int)m_GameBounds.width,(int)m_GameBounds.height));

	AddEntity(ent);
}

void SpaceInvadersScene::CreatePlayerShip(int startingLives)
{
	m_PlayerShip = nullptr;

	sf::Texture * tex = m_TextureCache->LoadTexture("sprites/game_character_hero.png");

	if(!tex)
	{
		Console::Log("Could not create player ship!", Console::Severity::ERROR);
		return;
	}

	Ship * ship = new PlayerShip();
	ship->Init(*tex, 0.5f, 0.5f);

	auto gameBounds = GetPlayerAreaBounds();
	auto bounds = ship->getGlobalBounds();
	sf::Vector2f pos(gameBounds.first + bounds.width/2.0f, m_GameBounds.height - bounds.height/2.0f - 25.0f);

	ship->setPosition(pos);
	ship->rotate(180);
	ship->SetCollisionGroup(CollisionGroup::PLAYER);
	AddPlayerShip(ship);

	ProjectileWeapon * weap = new ProjectileWeapon();
	weap->ConfigureProjectiles(m_TextureCache->LoadTexture("sprites/game_fx_projectile_rocket_001.png"), GAME_PROJECTILE_SPEED, 0.5f, 0.5f);
	ship->SetWeapon(weap);
	m_NextFire = m_NextFire.Zero;
	m_Lives = startingLives;
	
}

void SpaceInvadersScene::CreateEnemies(int enemiesRows, int enemiesPerRow)
{
	const int POINTS_PER_ROW = 20;
	const float ENEMY_WIDTH = 55;
	const float ENEMY_HEIGHT = 60;
	const float X_SPACING = 17;
	const float Y_SPACING = 10;
	const float Y_MARGIN = 35;

	const int TEXTURES_COUNT = 5;
	const std::string paths[TEXTURES_COUNT] = { "game_character_enemy_001.png", "game_character_enemy_001.png", "game_character_enemy_002.png", "game_character_enemy_002.png", "game_character_enemy_003.png" };
	sf::Texture * textures[TEXTURES_COUNT];
	for(int i = 0; i < enemiesRows; i++)
	{
		textures[i] = m_TextureCache->LoadTexture("sprites/" + paths[i]);
	}

	AlienShip * enemy = nullptr;
	auto playerBounds = GetPlayerAreaBounds();
	for(int row = 0; row < enemiesRows; row++)
	{
		int currentTextureNum = enemiesRows - 1 - (row % TEXTURES_COUNT);
		if(!textures[currentTextureNum])
		{
			Console::Log("Could not create a row of enemy ships!", Console::Severity::ERROR);
			continue;
		}

		for(int i = 0; i < enemiesPerRow; i++)
		{
			enemy = new AlienShip();
			enemy->Init(*textures[currentTextureNum], 0.5f, 0.5f);

			auto bounds = enemy->getGlobalBounds();
			enemy->SetHealth(1);
			enemy->SetCollisionGroup(CollisionGroup::ALIENS);
			enemy->setPosition(playerBounds.first + bounds.width/2.0f + i * (X_SPACING + ENEMY_WIDTH), Y_MARGIN + bounds.height/2.0f + row * (ENEMY_HEIGHT + Y_SPACING));
			AddEntity(enemy);

			enemy->SetPoints(POINTS_PER_ROW * (enemiesRows - row + 1));
			m_EnemyShips.push_back(enemy);

			// We want our aliens to be armed!
			ProjectileWeapon * weap = new ProjectileWeapon();
			weap->ConfigureProjectiles(m_TextureCache->LoadTexture("sprites/game_fx_projectile_rocket_001.png"), GAME_PROJECTILE_SPEED, 0.5f, 0.5f);
			enemy->SetWeapon(weap);
		}
	}
		
	m_StartingShips = enemiesRows * enemiesPerRow;
	m_FleetMovingRight = true;
	m_NextFleetFire = GetElapsedTime() + CalculateFleetAttackDelay();
}

void SpaceInvadersScene::CreateCover()
{
	if(!m_PlayerShip)
		return;

	sf::Texture * coverTx = m_TextureCache->LoadTexture("sprites/game_obstacle_barrier_001.png");

	if(!coverTx)
	{
		Console::Log("Could not create obstacles!", Console::Severity::ERROR);
		return;
	}

	const int COUNT = 4;
	const float MARGIN = 70.0f;
	const float Y_SPACING = 15.0f;

	for(int i = 0; i < COUNT; i++)
	{
		Entity * ent = new Entity();
		ent->Init(*coverTx, 0.8f, 0.8f);

		auto bounds = ent->getGlobalBounds();
		

		auto playerBounds = GetPlayerAreaBounds();
		float width = playerBounds.second - playerBounds.first;
		float spacing = width/(float)COUNT;

		ent->SetHealth(4);
		ent->setPosition(MARGIN + bounds.width/2.0f + playerBounds.first + spacing * i , m_PlayerShip->getPosition().y - m_PlayerShip->getGlobalBounds().height/2.0f - bounds.height/2.0f - Y_SPACING);
		ent->SetCollisionGroup(CollisionGroup::PLAYER);
		AddEntity(ent);
	}
}

void SpaceInvadersScene::Win()
{
	m_GameState = State::VICTORY;

	if(m_UI)
		m_UI->SendEvent("victory");

	Console::LogDebug("Victory!");
}

void SpaceInvadersScene::Lose()
{
	_ASSERT(m_PlayerShip != nullptr);

	m_GameState = State::DEFEAT;
	m_PlayerShip->Destroy();

	if(m_UI)
		m_UI->SendEvent("defeat");

	Console::LogDebug("Defeat!");
}

void SpaceInvadersScene::AdvanceStage()
{
	m_Stage++;

	CreateEnemies(ENEMIES_ROWS, ENEMIES_PER_ROW);
	
	m_GameState = State::PLAYING;

	if(m_UI)
		m_UI->SendEvent("next_stage");
	
}

void SpaceInvadersScene::HandlePlayerControls()
{
	if(m_GameState == State::PLAYING)
	{
		RunGameControls();
	}
	else if(m_GameState == State::DEFEAT)
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			Restart();
			m_UI->SendEvent("restart");
		}
	}
	else if(m_GameState == State::VICTORY)
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::N))
			AdvanceStage();
	}
}

void SpaceInvadersScene::RunGameControls()
{
	if(!m_PlayerShip)
		return;

	const int PLAYER_SHIP_SPEED = 700;

	sf::Time delta = GetDeltaTime();

	sf::Vector2f move;

	float diff = delta.asSeconds() * PLAYER_SHIP_SPEED;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		move.x -= diff;
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		move.x += diff;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_PlayerShip->GetWeapon() && GetElapsedTime() > m_NextFire)
	{
		const float RATE_OF_FIRE = 0.65f;
		Console::LogDebug("Player fired a weapon");
		m_PlayerShip->GetWeapon()->Fire();
		m_NextFire = GetElapsedTime() + sf::seconds(RATE_OF_FIRE);
	}

	// Cheat to kill all but one
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::L) && m_EnemyShips.size() > 0)
	{
		auto ships = m_EnemyShips;
		for(Entity * ent : ships)
		{
			ent->Destroy();
		}
	}
	
	// Correct the movement vector if it moves the object out of the player bounds
	bool dontMove = false;
	std::pair<float, float> gameBounds = GetPlayerAreaBounds();
	sf::FloatRect shipBounds = m_PlayerShip->getLocalBounds();
	auto position = m_PlayerShip->getPosition();
	// Boundry check for left/right
	if(position.x + move.x + shipBounds.width/2.0f > gameBounds.second)
	{
		m_PlayerShip->setPosition(gameBounds.second - shipBounds.width/2.0f, position.y);
		dontMove = true;
	}
	else if(position.x + move.x - shipBounds.width/2.0f < gameBounds.first)
	{
		m_PlayerShip->setPosition(gameBounds.first + shipBounds.width/2.0f, position.y);
		dontMove = true;
	}

	if(!dontMove)
		m_PlayerShip->move(move);
}

void SpaceInvadersScene::MoveFleet()
{
	if(!m_PlayerShip)
		return;

	const float DOWN_MOVE_SPEED = 25.0f;
	auto fleetBounds = GetFleetBounds();
	auto playerBounds = GetPlayerAreaBounds();
	float speed = CalculateFleetSpeed() * GetDeltaTime().asSeconds();

	if(!m_FleetMovingRight)
		speed = -speed;

	// Checking if we should move horizontally or if we've reached the point when we should go down
	sf::Vector2f move;
	if((fleetBounds.left + fleetBounds.width + speed > playerBounds.second) ||
		(fleetBounds.left + speed < playerBounds.first ))
	{
		move.y = DOWN_MOVE_SPEED;
		m_FleetMovingRight = !m_FleetMovingRight;
	}
	else
		move.x = speed;

	for(Entity * ent : m_EnemyShips)
	{
		ent->move(move);
	}

	// Check victory conditions
	auto playerPos = m_PlayerShip->getPosition();
	auto playerShipBnds = m_PlayerShip->getGlobalBounds();
	if(fleetBounds.top + fleetBounds.height + move.y > playerPos.y - playerShipBnds.height/2.0f)
	{
		Console::LogDebug("Aliens win!");
		Lose();
	}
}

void SpaceInvadersScene::FleetAttack()
{
	if(m_EnemyShips.size() == 0)
		return;

	// Selects a random ships to fire
	std::random_device rd;
    std::mt19937 gen(rd());
	std::uniform_int_distribution<> rand(1, CountAliveShips());

	int ship = rand(gen) - 1;
	Ship * sh = m_EnemyShips[ship];

	_ASSERT(sh != nullptr);
	if(sh->GetWeapon())
		m_EnemyShips[ship]->GetWeapon()->Fire();

	m_NextFleetFire = GetElapsedTime() + CalculateFleetAttackDelay();
}

sf::FloatRect SpaceInvadersScene::GetFleetBounds() const
{
	sf::FloatRect bounds;
	
	if(m_EnemyShips.size() == 0)
		return bounds;

	float left = FLT_MAX;
	float top = FLT_MAX;
	float right = 0.0f;
	float bottom = 0.0f;

	for(Entity * ship : m_EnemyShips)
	{
		_ASSERT(ship != nullptr);

		auto pos = ship->getPosition();
		auto bnds = ship->getGlobalBounds();
		left = std::min(pos.x - bnds.width/2.0f, left);
		top = std::min(pos.y - bnds.height/2.0f, top);
		right = std::max(pos.x + bnds.width/2.0f, right);
		bottom = std::max(pos.y + bnds.height/2.0f, bottom);
	}

	bounds.left = left;
	bounds.top = top;
	bounds.width = right - left;
	bounds.height = bottom - top;

	return bounds;
}

float SpaceInvadersScene::CalculateFleetSpeed() const
{
	if(m_StartingShips == 0)
		return 0.0f;

	const float BASE_DIFFICULTY_MODIFIER = 1.75f;
	const float PER_SHIP_DIFFICULTY_MODIFIER = 0.2f;

	const float BASE_SPEED = 13.0f + m_Stage * BASE_DIFFICULTY_MODIFIER;
	const float SPEED_PER_MISSING_SHIP = 0.61f + m_Stage * PER_SHIP_DIFFICULTY_MODIFIER;

	int shipsAlive = CountAliveShips();

	if(shipsAlive == 0)
		return 0.0f;

	return BASE_SPEED + float(std::pow(m_StartingShips - shipsAlive, 1.5f)) * SPEED_PER_MISSING_SHIP;
}

sf::Time SpaceInvadersScene::CalculateFleetAttackDelay() const
{
	const float DIFFICULTY_DECREASE = 0.15f;
	const float MIN_DELAY = 0.6f - DIFFICULTY_DECREASE * m_Stage;
	const float MAX_DELAY = 1.2f - DIFFICULTY_DECREASE * m_Stage;

	std::random_device rd;
    std::mt19937 gen(rd());
	std::uniform_real_distribution<> rand(MIN_DELAY, MAX_DELAY);
	return sf::seconds((float)rand(gen));
}

void SpaceInvadersScene::CleanEntitiesOutOfBounds()
{
	for(Entity * ent : m_Entities)
	{
		if(!IsAwaitingRemoval(ent))
		{
			if(!m_GameBounds.intersects(ent->getGlobalBounds()))
			{
				DeleteEntity(ent);
				Console::LogDebug("Deleted unseen entity");
			}
		}
	}
}


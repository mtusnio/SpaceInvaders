#ifndef H_SPACEINVADERSSCENE
#define H_SPACEINVADERSSCENE

#include "Scene.h"
#include "Handle.h"
#include "AlienShip.h"

class SpaceInvadersUI;

// Scene simulating the space invaders game!
class SpaceInvadersScene : public Scene
{
public:
	SpaceInvadersScene(float boundsX, float boundsY);
	virtual ~SpaceInvadersScene() { };

	virtual void Init();
	virtual void Clear();
	virtual void Restart() { Init(); }

	virtual void AddPlayerShip(Ship * ship);
	virtual Ship * GetPlayerShip() const { return m_PlayerShip; };

	// Returns minimum (as first) x for the player ship
	// and maximum (as second) x for the player ship
	virtual std::pair<float, float> GetPlayerAreaBounds() const;

	virtual void SendGameEvent(const std::string & event, Entity * sender);

	// Points count for scoring
	int GetPointsCount() const { return m_PointsCount; }

	// Returns how many more times the player can die
	int GetLivesLeft() const { return m_Lives; };

	// Current stage (starts with 0 for the first one)
	int GetStage() const { return m_Stage; };

	void SetUI(SpaceInvadersUI * ui) { m_UI = ui; };
private:
	// Speed for all projectiles
	static const int GAME_PROJECTILE_SPEED = 825;
	static const int ENEMIES_PER_ROW = 15;
	static const int ENEMIES_ROWS = 5;

	enum CollisionGroup
	{
		PLAYER = 0,
		ALIENS = 1,
	};

	enum State
	{
		PLAYING,
		VICTORY,
		DEFEAT,
	};

	virtual void RunSceneLogic();

	void CreateBackground(const std::string & spritePath);
	void CreatePlayerShip(int startingLives);
	void CreateEnemies(int enemiesRows, int enemiesPerRow);
	void CreateCover();

	// Methods to run when victory/defeat has been achieved
	void Win();
	void Lose();
	void AdvanceStage();

	void HandlePlayerControls();
	void RunGameControls();

	// One frame tick for the fleet
	void MoveFleet();
	// Fires a projectile from a random ship
	void FleetAttack();
	sf::FloatRect GetFleetBounds() const;
	// Returns how many units the fleet should move per second
	float CalculateFleetSpeed() const;
	// Returns amount of seconds before another attack can be performed by the fleet
	sf::Time CalculateFleetAttackDelay() const;
	int CountAliveShips() const { return  std::count_if(m_EnemyShips.begin(), m_EnemyShips.end(), [] (Entity * ent) { return ent != nullptr; }); };

	bool m_FleetMovingRight;

	// Cleans all entities that might've flown outside of the map
	void CleanEntitiesOutOfBounds();

	Handle<Ship> m_PlayerShip;

	sf::FloatRect m_GameBounds;

	sf::Time m_NextFire;
	sf::Time m_NextFleetFire;

	std::vector<Handle<AlienShip>> m_EnemyShips;

	int m_StartingShips;

	int m_PointsCount;
	int m_Lives;
	int m_Stage;

	SpaceInvadersUI * m_UI;

	// Stops any alien etc. simulations
	State m_GameState;
};


#endif
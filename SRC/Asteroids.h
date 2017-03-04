#ifndef __ASTEROIDS_H__
#define __ASTEROIDS_H__

#include "GameUtil.h"
#include "GameSession.h"
#include "IKeyboardListener.h"
#include "IGameWorldListener.h"
#include "IScoreListener.h"
#include "ScoreKeeper.h"
#include "Player.h"
#include "IPlayerListener.h"
#include "LifeBonus.h"
#include "ShieldPowerUp.h"
#include "Shield.h"

class GameObject;
class Spaceship;
class GUILabel;
class AlienSpaceship;
class BulletUpgrade;
class AlienAI;

class Asteroids : public GameSession, public IKeyboardListener, public IGameWorldListener, 
	public IScoreListener, public IPlayerListener
{
public:
	Asteroids(int argc, char *argv[]);
	virtual ~Asteroids(void);

	virtual void Start(void);
	virtual void Stop(void);
	virtual void CreateGUI(void);

	// Declaration of IKeyboardListener interface ////////////////////////////////
	void OnKeyPressed(uchar key, int x, int y);
	void OnKeyReleased(uchar key, int x, int y);
	void OnSpecialKeyPressed(int key, int x, int y);
	void OnSpecialKeyReleased(int key, int x, int y);

	//Declaration of IScoreListener interface
	virtual void OnScoreChanged(int score);
	ScoreKeeper mScoreKeeper;

	// Declaration of IGameWorldListener interface //////////////////////////////
	void OnWorldUpdated(GameWorld* world) {}
	void OnObjectAdded(GameWorld* world, shared_ptr<GameObject> object) {}
	void OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object);

	// Override the default implementation of ITimerListener ////////////////////
	void OnTimer(int value);

	// Declaration of IPlayerListener interface ////////////////
	virtual void OnPlayerKilled(int lives_left);
	virtual void OnBonusPicked(int lives);
	virtual void OnShieldPicked(void);
	virtual void OnBulletUpgradePicked(void);
	virtual void OnEnemyHit(int life_left);
	virtual void OnPlayerHit(int life_left);

	shared_ptr<GameObject> CreateExplosion();

private:
	Player mPlayer;
	shared_ptr<GUILabel> mLivesLabel;
	shared_ptr<GameObject> mBackground;
	shared_ptr<LifeBonus> mLifeBonus;
	shared_ptr<ShieldPowerUp> mShieldPowerUp;
	shared_ptr<Spaceship> mSpaceship;
	shared_ptr<AlienSpaceship> mAlienSpaceship;
	shared_ptr<GUILabel> mScoreLabel;
	shared_ptr<GUILabel> mGameOverLabel;
	shared_ptr<GUILabel> mAlienHealthLabel;
	shared_ptr<GUILabel> mPlayerHealthLabel;
	shared_ptr<BulletUpgrade> mBulletUpgrade;
	shared_ptr<Sprite> bullet_sprite;
	shared_ptr<AlienAI> mAlienAI;

	uint mLevel;
	uint mAsteroidCount = 0;
	uint mMiniAsteroidCount = 0;

	void ResetSpaceship();
	void CreateSpaceship();
	void CreateAsteroids(const uint num_asteroids);
	void CreateMiniAsteroids(const uint num_miniasteroids, shared_ptr<GameObject> object);
	void CreateBonuses(void);
	void CreateBackground(void);
	void CreateAlienSpaceship(void);
	
	const static uint SHOW_GAME_OVER = 0;
	const static uint START_NEXT_LEVEL = 1;
	const static uint CREATE_NEW_PLAYER = 2;
	const static uint CREATE_NEW_BONUS = 3;
	const static uint CREATE_NEW_SHIELD = 4;
	const static uint CREATE_NEW_BULLET_UPGRADE = 5;
};



#endif
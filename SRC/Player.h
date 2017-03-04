#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameUtil.h"
#include "GameObject.h"
#include "GameObjectType.h"
#include "IPlayerListener.h"
#include "IGameWorldListener.h"
#include "AlienSpaceship.h"
#include"Spaceship.h"

class Player : public IGameWorldListener
{
public:
	Player() { mLives = 3; }
	virtual ~Player() {}

	void OnWorldUpdated(GameWorld* world) {
		if (alien != nullptr && alien->isColliding()){
				FireEnemyHit();
		}
		
		if (player->IsHit()){
			FirePlayerHit();
		}
	}

	void OnObjectAdded(GameWorld* world, shared_ptr<GameObject> object) 
	{

	}

	void OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object)
	{
		if (object->GetType() == GameObjectType("Spaceship")) {
			mLives -= 1;
			FirePlayerKilled();
		}
		if (object->GetType() == GameObjectType("LifeBonus"))
		{
			mLives += 1;
			FireBonusPicked();
		}
		if (object->GetType() == GameObjectType("ShieldPowerUp"))
		{
			FireShieldPicked();
		}
		if (object->GetType() == GameObjectType("BulletUpgrade"))
		{
			FireBulletUpgradePicked();
		}
	}

	void AddListener(shared_ptr<IPlayerListener> listener)
	{
		mListeners.push_back(listener);
	}

	void FirePlayerKilled()
	{
		// Send message to all listeners
		for (PlayerListenerList::iterator lit = mListeners.begin();
			lit != mListeners.end(); ++lit) {
			(*lit)->OnPlayerKilled(mLives);
		}
	}

	void FireBonusPicked()
	{
		// Send message to all listeners
		for (PlayerListenerList::iterator lit = mListeners.begin();
			lit != mListeners.end(); ++lit) {
			(*lit)->OnBonusPicked(mLives);
		}
	}
	void FireShieldPicked()
	{
		// Send message to all listeners
		for (PlayerListenerList::iterator lit = mListeners.begin();
			lit != mListeners.end(); ++lit) {
			(*lit)->OnShieldPicked();
		}
	}
	void FireBulletUpgradePicked()
	{
		// Send message to all listeners
		for (PlayerListenerList::iterator lit = mListeners.begin();
			lit != mListeners.end(); ++lit) {
			(*lit)->OnBulletUpgradePicked();
		}
	}
	void FireEnemyHit()
	{
		// Send message to all listeners
		for (PlayerListenerList::iterator lit = mListeners.begin();
			lit != mListeners.end(); ++lit) {
			(*lit)->OnEnemyHit(alien->GetLife());
		}
	}
	void FirePlayerHit()
	{
		// Send message to all listeners
		for (PlayerListenerList::iterator lit = mListeners.begin();
			lit != mListeners.end(); ++lit) {
			(*lit)->OnPlayerHit(player->GetHealth());
		}
	}


	void SetEnemy(shared_ptr<AlienSpaceship> enemy){
		alien = enemy;
	}
	void SetPlayer(shared_ptr<Spaceship> p){
		player = p;
	}


private:
	int mLives;
	shared_ptr<AlienSpaceship> alien;
	shared_ptr<Spaceship> player;
	typedef std::list< shared_ptr<IPlayerListener> > PlayerListenerList;
	PlayerListenerList mListeners;
};

#endif

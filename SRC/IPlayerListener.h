#ifndef __IPLAYERLISTENER_H__
#define __IPLAYERLISTENER_H__

class IPlayerListener
{
public:
	virtual void OnPlayerKilled(int lives_left) = 0;
	virtual void OnBonusPicked(int lives) = 0;
	virtual void OnShieldPicked(void) = 0;
	virtual void OnBulletUpgradePicked(void) = 0;
	virtual void OnEnemyHit(int life_left) = 0;
	virtual void OnPlayerHit(int health) = 0;

};

#endif

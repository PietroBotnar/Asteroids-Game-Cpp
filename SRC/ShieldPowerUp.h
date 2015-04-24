#ifndef __SHIELDPOWERUP_H__
#define __SHIELDPOWERUP_H__

#include "GameObject.h"

class ShieldPowerUp : public GameObject
{
public:
	ShieldPowerUp(void);
	~ShieldPowerUp(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);
	virtual void Update(int t);

protected:
	int shieldbonus_lifespan;
};

#endif

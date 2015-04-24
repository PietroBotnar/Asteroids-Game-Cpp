#ifndef __LIFEBONUS_H__
#define __LIFEBONUS_H__

#include "GameObject.h"

class LifeBonus : public GameObject
{
public:
	LifeBonus(void);
	~LifeBonus(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);
	virtual void Update(int t);

protected:
	int lifebonus_lifespan;
};

#endif
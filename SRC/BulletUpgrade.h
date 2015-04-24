#ifndef __BULLETUPGRADE_H__
#define __BULLETUPGRADE_H__

#include "GameObject.h"

class BulletUpgrade : public GameObject
{
public:
	BulletUpgrade(void);
	~BulletUpgrade(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);
	virtual void Update(int t);

protected:
	int bulletupgrade_lifespan;
};
#endif

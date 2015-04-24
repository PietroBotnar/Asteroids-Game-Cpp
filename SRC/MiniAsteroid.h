#ifndef __MINIASTEROID_H__
#define __MINIASTEROID_H__

#include "GameObject.h"

class MiniAsteroid : public GameObject
{
public:
	MiniAsteroid(shared_ptr<GameObject> o);
	~MiniAsteroid(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);

};

#endif
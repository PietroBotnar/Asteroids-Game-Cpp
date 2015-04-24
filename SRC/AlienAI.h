#ifndef _ALIENAI_H__
#define _ALIENAI_H__

#include "GameObject.h"
#include "BoundingShape.h"


class AlienAI : public GameObject
{
public:
	AlienAI(shared_ptr<AlienSpaceship> alien_spaceship) : GameObject("AlienAI")
	{
		mAlienSpaceship = alien_spaceship;
		mPosition = mAlienSpaceship->GetPosition();
	}
	virtual ~AlienAI(){}

	void Update(int t){
		mPosition = mAlienSpaceship->GetPosition();
		GameObject::Update(t);
	}

	bool CollisionTest(shared_ptr<GameObject> o)
	{
		if (o->GetType() != GameObjectType("Bullet") && o->GetType() != GameObjectType("Spaceship")){ return false; }
		if (mBoundingShape.get() == NULL) return false;
		if (o->GetBoundingShape().get() == NULL) return false;
		return mBoundingShape->CollisionTest(o->GetBoundingShape());
	}
	void OnCollision(const GameObjectList &objects)
	{
		mAlienSpaceship->SetEngage(true);
		mAlienSpaceship->SetMove(true);
	}
private:
	shared_ptr<AlienSpaceship> mAlienSpaceship;
};

#endif
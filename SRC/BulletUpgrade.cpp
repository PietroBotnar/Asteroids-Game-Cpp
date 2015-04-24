#include <stdlib.h>
#include "BoundingShape.h"
#include "BulletUpgrade.h"

BulletUpgrade::BulletUpgrade(void) : GameObject("BulletUpgrade")
{
	mPosition.x = rand() % 100;
	mPosition.y = rand() % 100;
	mPosition.z = 0.0f;
	bulletupgrade_lifespan = 5000;
}

BulletUpgrade::~BulletUpgrade(void){}

bool BulletUpgrade::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() != GameObjectType("Spaceship")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}
void BulletUpgrade::OnCollision(const GameObjectList& objects)
{
	mWorld->FlagForRemoval(GetThisPtr());
}
void BulletUpgrade::Update(int t)
{
	// Update position
	GameObject::Update(t);
	// Reduce lifespan
	bulletupgrade_lifespan -= t;
	// Ensure lifespan isn't negative
	if (bulletupgrade_lifespan < 0) { bulletupgrade_lifespan = 0; }
	// If lifespan is zero then re-position the bonus and reset lifespan 
	if (bulletupgrade_lifespan == 0) {
		mPosition.x = rand() / 2;
		mPosition.y = rand() / 2;
		mPosition.z = 0.0;
		bulletupgrade_lifespan = 5000;
		GameObject::Update(t);
	}
}
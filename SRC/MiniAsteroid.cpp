#include <stdlib.h>
#include "MiniAsteroid.h"
#include "BoundingShape.h"

MiniAsteroid::MiniAsteroid(shared_ptr<GameObject> o) : GameObject("MiniAsteroid")
{
	mPosition = o->GetPosition();
	mAngle = rand() % 360;
	mRotation = rand() % 90;
	mVelocity.x = 30.0 * cos(DEG2RAD*mAngle);
	mVelocity.y = 30.0 * sin(DEG2RAD*mAngle);
	mVelocity.z = 0.0;
}

MiniAsteroid::~MiniAsteroid(void)
{
}

bool MiniAsteroid::CollisionTest(shared_ptr<GameObject> o)
{
	if (GetType() == o->GetType()) return false;
	if (o->GetType() != GameObjectType("SpaceShip") && o->GetType() != GameObjectType("Bullet") &&
		o->GetType() != GameObjectType("Shield")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void MiniAsteroid::OnCollision(const GameObjectList& objects)
{
	mWorld->FlagForRemoval(GetThisPtr());
}
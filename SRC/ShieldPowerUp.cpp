#include <stdlib.h>
#include "ShieldPowerUp.h"
#include "BoundingShape.h"

ShieldPowerUp::ShieldPowerUp(void) : GameObject("ShieldPowerUp")
{
	mPosition.x = rand() % 100;
	mPosition.y = rand() % 100;
	mPosition.z = 0.0f;
	shieldbonus_lifespan = 5000;
	mAngle = 180;
}

ShieldPowerUp::~ShieldPowerUp(void){}

bool ShieldPowerUp::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() != GameObjectType("Spaceship")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}
void ShieldPowerUp::OnCollision(const GameObjectList& objects)
{
	mWorld->FlagForRemoval(GetThisPtr());
}
void ShieldPowerUp::Update(int t)
{
	// Update position
	GameObject::Update(t);
	// Reduce lifespan
	shieldbonus_lifespan -= t;
	// Ensure lifespan isn't negative
	if (shieldbonus_lifespan < 0) { shieldbonus_lifespan = 0; }
	// If lifespan is zero then re-position the bonus and reset lifespan 
	if (shieldbonus_lifespan == 0) {
		mPosition.x = rand() / 2;
		mPosition.y = rand() / 2;
		mPosition.z = 0.0;
		shieldbonus_lifespan = 5000;
		GameObject::Update(t);
	}
}
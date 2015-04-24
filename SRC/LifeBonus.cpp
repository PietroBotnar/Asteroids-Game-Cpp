#include <stdlib.h>
#include "LifeBonus.h"
#include "BoundingShape.h"

LifeBonus::LifeBonus(void) : GameObject("LifeBonus")
{
	mPosition.x = rand() / 2;
	mPosition.y = rand() / 2;
	mPosition.z = 0.0;
	lifebonus_lifespan = 5000;
}

LifeBonus::~LifeBonus(void)
{
}

bool LifeBonus::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() != GameObjectType("Spaceship")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void LifeBonus::OnCollision(const GameObjectList& objects)
{
	mWorld->FlagForRemoval(GetThisPtr());
}

/** Update LifeBonus, re-positioning it if necessary. */
void LifeBonus::Update(int t)
{
	// Update position
	GameObject::Update(t);
	// Reduce lifespan
	lifebonus_lifespan -= t;
	// Ensure lifespan isn't negative
	if (lifebonus_lifespan < 0) { lifebonus_lifespan = 0; }
	// If lifespan is zero then re-position the bonus and reset lifespan 
	if (lifebonus_lifespan == 0) {
		mPosition.x = rand() / 2;
		mPosition.y = rand() / 2;
		mPosition.z = 0.0;
		lifebonus_lifespan = 5000;
		GameObject::Update(t);
	}

}
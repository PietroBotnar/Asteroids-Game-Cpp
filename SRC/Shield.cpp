#include "Shield.h"
#include "BoundingShape.h"

Shield::Shield(GLVector3f position, int lifespan) : GameObject("Shield"){
	mPosition = position;
	shield_lifespan = lifespan;
	isActive = true;
}
Shield::~Shield(void){}

bool Shield::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() != GameObjectType("AlienSpaceship")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}
void Shield::OnCollision(const GameObjectList& objects)
{

}
void Shield::Update(int t)
{
	// Reduce lifespan
	shield_lifespan -= t;
	// Ensure lifespan isn't negative
	if (shield_lifespan < 0) { shield_lifespan = 0; }
	// If lifespan is zero then re-position the bonus and reset lifespan 
	if (shield_lifespan == 0) {
		isActive = false;
	}
}
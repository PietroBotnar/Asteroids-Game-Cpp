#include "GameUtil.h"
#include "GameWorld.h"
#include "Bullet.h"
#include "Spaceship.h"
#include "BoundingSphere.h"
#include "Shield.h"
#include "AnimationManager.h"
#include "Animation.h"

using namespace std;

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/**  Default constructor. */
Spaceship::Spaceship()
	: GameObject("Spaceship"), mThrust(0)
{
}

/** Construct a spaceship with given position, velocity, acceleration, angle, and rotation. */
Spaceship::Spaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r)
	: GameObject("Spaceship", p, v, a, h, r), mThrust(0)
{
}

/** Copy constructor. */
Spaceship::Spaceship(const Spaceship& s)
	: GameObject(s), mThrust(0)
{
}

/** Destructor. */
Spaceship::~Spaceship(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update this spaceship. */
void Spaceship::Update(int t)
{
	// Call parent update function
	GameObject::Update(t);
	mShield->SetPosition(mPosition);
	if (!(mShield->IsActive())){
		mWorld->RemoveObject(mShield);
	}
}

/** Render this spaceship. */
void Spaceship::Render(void)
{
	GameObject::Render();
}

void Spaceship::Thrust(float t)
{
	mThrust = t;
	// Increase acceleration in the direction of ship
	mAcceleration.x = mThrust*cos(DEG2RAD*mAngle);
	mAcceleration.y = mThrust*sin(DEG2RAD*mAngle);
}

/** Set the rotation. */
void Spaceship::Rotate(float r)
{
	mRotation = r;
}

/** Shoot a bullet. */
void Spaceship::Shoot(void)
{
	// Check the world exists
	if (!mWorld) return;
	// Construct a unit length vector in the direction the spaceship is headed
	GLVector3f spaceship_heading(cos(DEG2RAD*mAngle), sin(DEG2RAD*mAngle), 0);
	spaceship_heading.normalize();

	// Calculate the point at the node of the spaceship from position and heading
	GLVector3f bullet_position =  mPosition+(spaceship_heading * 9);

	// Construct a vector for the bullet's velocity
	GLVector3f bullet_velocity = mVelocity + spaceship_heading * bullet_speed;
	// Construct a new bullet
	shared_ptr<GameObject> bullet (new Bullet("Bullet",bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000));

	bullet->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
	bullet->SetSprite(mBulletSprite);
	bullet->SetScale(0.02f);

	// Add the new bullet to the game world
	mWorld->AddObject(bullet);
}

void Spaceship::CreateShield(int t)
{
	mShield = make_shared<Shield>(mPosition, t);
	mShield->SetBoundingShape(make_shared<BoundingSphere>(mShield->GetThisPtr(), 15.0f));
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("shield");
	shared_ptr<Sprite> spaceship_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	mShield->SetSprite(spaceship_sprite);
	mShield->SetScale(0.1f);
	mWorld->AddObject(mShield);
}

void Spaceship::SetShieldVisibility(bool b)
{
	if (b){
		mShield->SetActive(true);
		mWorld->AddObject(mShield);
	}
	else{
		mWorld->FlagForRemoval(mShield);
	}
}

bool Spaceship::CollisionTest(shared_ptr<GameObject> o)
{
	isHit = false;
	if (o->GetType() != GameObjectType("AlienBullet") && o->GetType() != GameObjectType("AlienSpaceship")
		&& o->GetType() != GameObjectType("Asteroid") && o->GetType() != GameObjectType("MiniAsteroid")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Spaceship::OnCollision(const GameObjectList &objects)
{
	for (GameObjectList::const_iterator it = objects.begin(); it != objects.end(); ++it)
	{
		if ((*it)->GetType() == GameObjectType("AlienBullet")){ mHealth--; }
		else{ mHealth = 0; }
	}
	SetShieldVisibility(false);
	if (mHealth < 0){ mHealth == 0; }
	else if (mHealth == 0){
		mWorld->FlagForRemoval(GetThisPtr());
	}
	isHit = true;
} 
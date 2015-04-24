#include "AlienSpaceship.h"
#include "BoundingShape.h"
#include "BoundingSphere.h"
#include "Bullet.h"
#include "Spaceship.h"

#define PI 3.14159265

AlienSpaceship::AlienSpaceship(int life) : GameObject("AlienSpaceship")
{
	mLife = life;
	mPosition.x = rand() / 2;
	mPosition.y = 100;
	mPosition.z = 0;
	mAngle = rand() % 360;
	mVelocity.x = 10.0 * cos(DEG2RAD*mAngle);
	mVelocity.y = 10.0 * sin(DEG2RAD*mAngle);
	mVelocity.z = 0.0;
}

AlienSpaceship::~AlienSpaceship()
{
}

void AlienSpaceship::Update(int t)
{
	if (engage){
		//Set the angle towards the enemy spaceship (i.e. my spaceship) and reset velocity towards it
		mAngle = (atan2f(mPosition.y - mEnemy->GetPosition().y, mPosition.x - mEnemy->GetPosition().x) * 180 / PI) - 180.0f;
		mVelocity.x = 5.0 * cos(DEG2RAD*mAngle);
		mVelocity.y = 5.0 * sin(DEG2RAD*mAngle);
		change_direction_rate -= t;
		if (change_direction_rate < 0){ change_direction_rate = 0; }
		if (change_direction_rate == 0){
			if (changeDirection){
				changeDirection = false;
			}
			else{ changeDirection = true; }
			change_direction_rate = 2000;
		}
		if (move){
			if (changeDirection){ MoveAnticlowise(); }
			else{ MoveClockwise(); }
			move = false;
		}
		shooting_rate -= t;
		if (shooting_rate < 0){ shooting_rate = 0; }
		if (shooting_rate == 0){
			Shoot();
			shooting_rate = 500;
		}
		engage_timer -= t;
		if (engage_timer < 0){ engage_timer = 0; }
		if (engage_timer == 0){
			engage = false;
			engage_timer = 5000;
		}
	}
	GameObject::Update(t);
}

void AlienSpaceship::Rotate(float r)
{
	mRotation = r;

}

void AlienSpaceship::MoveAnticlowise()
{
	if (mEnemy->GetPosition().x <= mPosition.x){ // alien at the right

		if (mEnemy->GetPosition().y >= mPosition.y){ // alien at the bottom right
			mVelocity.y += 30 * sin(DEG2RAD * 30);
			mVelocity.x += 30 * cos(DEG2RAD * 30);
		}
		else{										//alien at top right
			mVelocity.y += 30 * sin(DEG2RAD * 30);
			mVelocity.x -= 30 * cos(DEG2RAD * 30);
		}
	}
	else{											//alien at the left
		if (mEnemy->GetPosition().y >= mPosition.y){ //alien at bottom left
			mVelocity.y -= 30 * sin(DEG2RAD * 30);
			mVelocity.x += 30 * cos(DEG2RAD * 30);
		}
		else{										//alien at top left
			mVelocity.y -= 30 * sin(DEG2RAD * 30);
			mVelocity.x -= 30 * cos(DEG2RAD * 30);
		}
	}
}

void AlienSpaceship::MoveClockwise()
{
	if (mEnemy->GetPosition().x <= mPosition.x){ // alien at the right

		if (mEnemy->GetPosition().y >= mPosition.y){ // alien at the bottom right
			mVelocity.y -= 30 * sin(DEG2RAD * 30);
			mVelocity.x -= 30 * cos(DEG2RAD * 30);
		}
		else{										//alien at top right
			mVelocity.y -= 30 * sin(DEG2RAD * 30);
			mVelocity.x += 30 * cos(DEG2RAD * 30);
		}
	}
	else{											//alien at the left
		if (mEnemy->GetPosition().y >= mPosition.y){ //alien at bottom left
			mVelocity.y += 30 * sin(DEG2RAD * 30);
			mVelocity.x -= 30 * cos(DEG2RAD * 30);
		}
		else{										//alien at top left
			mVelocity.y += 30 * sin(DEG2RAD * 30);
			mVelocity.x += 30 * cos(DEG2RAD * 30);
		}
	}
}

void AlienSpaceship::Shoot()
{
	// Check the world exists
	if (!mWorld) return;
	// Construct a unit length vector in the direction the spaceship is headed
	GLVector3f spaceship_heading(cos(DEG2RAD*mAngle), sin(DEG2RAD*mAngle), 0);
	spaceship_heading.normalize();
	// Calculate the point at the node of the spaceship from position and heading
	GLVector3f bullet_position = mPosition + (spaceship_heading * 4);
	//Set the bullet's velocity
	float bullet_speed = 45;
	// Construct a vector for the bullet's velocity
	GLVector3f bullet_velocity = spaceship_heading * bullet_speed;
	// Construct a new bullet
	shared_ptr<GameObject> bullet(new Bullet("AlienBullet", bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000));
	bullet->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
	bullet->SetSprite(mBulletSprite);
	bullet->SetScale(0.02f);
	// Add the new bullet to the game world
	mWorld->AddObject(bullet);
}

bool AlienSpaceship::CollisionTest(shared_ptr<GameObject> o)
{
	colliding = false;
	if (o->GetType() != GameObjectType("Bullet") && o->GetType() != GameObjectType("Shield")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void AlienSpaceship::OnCollision(const GameObjectList &objects)
{
	colliding = true;
	mLife -= 1;
	if (mLife < 0){ mLife = 0; }
	if (mLife == 0){
 		mWorld->FlagForRemoval(GetThisPtr());
		isInWorld = false;
	}
}


#ifndef __SPACESHIP_H__
#define __SPACESHIP_H__

#include "GameUtil.h"
#include "GameObject.h"
#include "Shape.h"

class Shield;
class Sprite;

class Spaceship : public GameObject
{
public:
	Spaceship();
	Spaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r);
	Spaceship(const Spaceship& s);
	virtual ~Spaceship(void);

	virtual void Update(int t);
	virtual void Render(void);

	virtual void Thrust(float t);
	virtual void Rotate(float r);
	virtual void Shoot(void);
	virtual void CreateShield(int t);
	virtual void SetShieldVisibility(bool b);
	shared_ptr<Shield> GetShield(void){ return mShield; };
	int GetHealth(){ return mHealth; }
	void SetHealth(int h){ mHealth = h; }
	bool IsHit(){ return isHit; }

	void SetBulletSprite(shared_ptr<Sprite> bullet_sprite) { mBulletSprite = bullet_sprite; }
	void SetBulletSpeed(int speed) { bullet_speed = speed; }
	void IncreaseBulletSpeed(int speed){ bullet_speed += speed; }

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList &objects);

	

private:
	float mThrust;
	int bullet_speed = 30;
	shared_ptr<Sprite> mBulletSprite;
	shared_ptr<Shield> mShield;
	bool isHit = false;
	int mHealth;
};

#endif
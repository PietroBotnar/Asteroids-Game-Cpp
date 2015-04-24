#ifndef __ALIENSPACWSHIP_H__
#define __ALIENSPACWSHIP_H__

#include "GameObject.h"

class Spaceship;

class AlienSpaceship : public GameObject
{
public:
	AlienSpaceship(int life);
	virtual ~AlienSpaceship();

	virtual void Rotate(float r);
	virtual void Shoot();
	virtual void Update(int t);

	void SetBulletSprite(shared_ptr<Sprite> bullet_sprite) { mBulletSprite = bullet_sprite; }
	void SetEnemy(shared_ptr<Spaceship> enemy) {mEnemy = enemy; }
	void SetEngage(bool e){ engage = e; }
	void SetMove(bool m){ move = m; }
	int GetLife(){ return mLife; }
	void SetLife(int life){ mLife = life; }
	bool IsInWorld(){ return isInWorld; }
	void SetInWorld(bool t){ isInWorld = t; }
	void MoveClockwise();
	void MoveAnticlowise();

	bool isColliding(){ return colliding; }
	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList &objects);

private:
	shared_ptr<Spaceship> mEnemy;
	bool engage = false;
	bool move = false;
	shared_ptr<Sprite> mBulletSprite;
	float mThrust;
	int mLife;
	int shooting_rate = 500; //0.5sec
	int change_direction_rate = 2000; //2sec
	int engage_timer = 10000; //10sec
	bool changeDirection = false;
	bool colliding = false;
	bool isInWorld = false;
};


#endif
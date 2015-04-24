#ifndef __SHIELD_H__
#define __SHIELD_H__

#include "GameObject.h"

class Shield : public GameObject{
public:
	Shield(GLVector3f vector, int lifespan);
	~Shield(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);
	void SetActive(bool b){ isActive = b; };
	bool IsActive(){ return isActive; };
	void SetLifespan(int t){ shield_lifespan = t; };
	virtual void Update(int t);

protected:
	int shield_lifespan;
	bool isActive;
};

#endif

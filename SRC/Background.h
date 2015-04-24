#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "GameObject.h"

class Sprite;

class Background : public GameObject
{
public:
	Background(uint width, uint height, uint frame_w, uint frame_h, string filename);
	~Background(void);


protected:
	shared_ptr<Sprite> background_sprite;
};
#endif

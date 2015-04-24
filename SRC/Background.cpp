#include "Background.h"
#include "Sprite.h"
#include "Animation.h"
#include "AnimationManager.h"

Background::Background(uint w, uint h, uint fw, uint fh, string filename) : GameObject("Background")
{
	Animation *bg_anim = AnimationManager::GetInstance().CreateAnimationFromFile("background", w, h, fw, fh, filename);
	background_sprite = make_shared<Sprite>(bg_anim->GetWidth(), bg_anim->GetHeight(), bg_anim);
	background_sprite->SetLoopAnimation(true);
	SetSprite(background_sprite);
	SetScale(0.25);
}

Background::~Background(void)
{
}
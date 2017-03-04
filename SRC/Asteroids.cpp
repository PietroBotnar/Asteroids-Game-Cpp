#include "Asteroid.h"
#include "Asteroids.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "GameUtil.h"
#include "GameWindow.h"
#include "GameWorld.h"
#include "GameDisplay.h"
#include "Spaceship.h"
#include "BoundingShape.h"
#include "BoundingSphere.h"
#include "GUILabel.h"
#include "Explosion.h"
#include "MiniAsteroid.h"
#include "LifeBonus.h"
#include "Background.h"
#include "Shield.h"
#include "BulletUpgrade.h"
#include "AlienSpaceship.h"
#include "AlienAI.h"

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Constructor. Takes arguments from command line, just in case. */
Asteroids::Asteroids(int argc, char *argv[])
	: GameSession(argc, argv)
{
	mLevel = 0;
	mAsteroidCount = 0;
}

/** Destructor. */
Asteroids::~Asteroids(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Start an asteroids game. */
void Asteroids::Start()
{
	// Create a shared pointer for the Asteroids game object - DO NOT REMOVE
	shared_ptr<Asteroids> thisPtr = shared_ptr<Asteroids>(this);

	// Add this class as a listener of the game world
	mGameWorld->AddListener(thisPtr.get());

	mGameWorld->AddListener(&mScoreKeeper);
	mScoreKeeper.AddListener(thisPtr);

	// Add a player (watcher) to the game world
	mGameWorld->AddListener(&mPlayer);

	// Add this class as a listener of the player
	mPlayer.AddListener(thisPtr);


	// Add this as a listener to the world and the keyboard
	mGameWindow->AddKeyboardListener(thisPtr);

	// Create an ambient light to show sprite textures
	GLfloat ambient_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuse_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glEnable(GL_LIGHT0);

	//Initialise animations
	Animation *explosion_anim = AnimationManager::GetInstance().CreateAnimationFromFile("explosion", 64, 1024, 64, 64, "texture/explosion_fs.png");
	Animation *asteroid1_anim = AnimationManager::GetInstance().CreateAnimationFromFile("asteroid1", 128, 8192, 128, 128, "texture/asteroid1_fs.png");
	Animation *asteroid2_anim = AnimationManager::GetInstance().CreateAnimationFromFile("asteroid2", 128, 8192, 128, 128, "texture/asteroid2_fs.png");
	Animation *lifebonus_anim = AnimationManager::GetInstance().CreateAnimationFromFile("lifebonus", 128, 1024, 128, 128, "texture/lifebonus_fs.png");
	Animation *alien_spaceship_anim = AnimationManager::GetInstance().CreateAnimationFromFile("alien_spaceship", 310, 330, 310, 330, "texture/revenant.png");
	Animation *spaceship_anim = AnimationManager::GetInstance().CreateAnimationFromFile("spaceship", 500, 500, 500, 500, "texture/player.png");
	Animation *shieldbonus_anim = AnimationManager::GetInstance().CreateAnimationFromFile("shield_powerup", 128, 1024, 128, 128, "texture/shieldbonus_fs.png");
	Animation *shield_anim = AnimationManager::GetInstance().CreateAnimationFromFile("shield", 300, 300, 300, 300, "texture/shield3.png");
	Animation *bulletupgrade_anim = AnimationManager::GetInstance().CreateAnimationFromFile("bulletupgrade", 128, 1024, 128, 128, "texture/bulletupgrade_fs.png");
	Animation *bullet_anim = AnimationManager::GetInstance().CreateAnimationFromFile("bullet1", 64, 36, 64, 36, "texture/bullet1.png");
	Animation *bullet2_anim = AnimationManager::GetInstance().CreateAnimationFromFile("bullet2", 128, 128, 128, 128, "texture/bullet2.png");
	Animation *alien_bullet_anim = AnimationManager::GetInstance().CreateAnimationFromFile("alien_bullet", 64, 36, 64, 36, "texture/alien_bullet.png");
	bullet_sprite =	make_shared<Sprite>(bullet_anim->GetWidth(), bullet_anim->GetHeight(), bullet_anim);

	mLevel = 1;

	//Create GUI label
	CreateGUI();
	//Create a background
	CreateBackground();
	// Create a spaceship and add it to the world
	CreateSpaceship();
	//Create an alien spaceship and add it to the world
	CreateAlienSpaceship();
	// Create bonuses
	CreateBonuses();
	// Create some asteroids and add them to the world
	CreateAsteroids(10);
	
	//load audio files here
	mGameWorld->LoadAudioFile("player_shot", "audio\\player_shot.wav");
	mGameWorld->LoadAudioFile("enemy_shot", "audio\\enemy_shot.wav");
	mGameWorld->LoadAudioFile("bang_m", "audio\\bangMedium.wav");
	mGameWorld->LoadAudioFile("bang_l", "audio\\bangLarge.wav");
	mGameWorld->LoadAudioFile("bonus1", "audio\\bonus1.wav");
	mGameWorld->LoadAudioFile("bonus2", "audio\\bonus2.wav");
	mGameWorld->LoadAudioFile("bonus3", "audio\\bonus3.wav");
	mGameWorld->LoadAudioFile("explosion", "audio\\spaceship_explosion.wav");
	mGameWorld->LoadAudioFile("impact", "audio\\impact.wav");
	mGameWorld->LoadAudioFile("gameover", "audio\\game_over.mp3");
	////load music
	mGameWorld->LoadMusicStream("background", "audio\\star_commander.wav");

	mGameWorld->PlayMusic("background");

	// Start the game
	GameSession::Start();
}

/** Stop the current game. */
void Asteroids::Stop()
{
	// Stop the game
	GameSession::Stop();
}

// PUBLIC INSTANCE METHODS IMPLEMENTING IKeyboardListener /////////////////////

void Asteroids::OnKeyPressed(uchar key, int x, int y)
{
	switch (key)
	{
	case ' ':
		mSpaceship->Shoot();
		break;
	default:
		break;
	}
}

void Asteroids::OnKeyReleased(uchar key, int x, int y) {}

void Asteroids::OnSpecialKeyPressed(int key, int x, int y)
{
	switch (key)
	{
	// If up arrow key is pressed start applying forward thrust
	case GLUT_KEY_UP: mSpaceship->Thrust(20); break;
	// If left arrow key is pressed start rotating anti-clockwise
	case GLUT_KEY_LEFT: mSpaceship->Rotate(90); break;
	// If right arrow key is pressed start rotating clockwise
	case GLUT_KEY_RIGHT: mSpaceship->Rotate(-90); break;
	// Default case - do nothing
	default: break;
	}
}

void Asteroids::OnSpecialKeyReleased(int key, int x, int y)
{
	switch (key)
	{
	// If up arrow key is released stop applying forward thrust
	case GLUT_KEY_UP: mSpaceship->Thrust(0); break;
	// If left arrow key is released stop rotating
	case GLUT_KEY_LEFT: mSpaceship->Rotate(0); break;
	// If right arrow key is released stop rotating
	case GLUT_KEY_RIGHT: mSpaceship->Rotate(0); break;
	// Default case - do nothing
	default: break;
	} 
}


// PUBLIC INSTANCE METHODS IMPLEMENTING IGameWorldListener ////////////////////


void Asteroids::OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object)
{
	if (object->GetType() == GameObjectType("Asteroid"))
	{
		shared_ptr<GameObject> explosion = CreateExplosion();
		mGameWorld->PlayAudio("bang_l");
		explosion->SetPosition(object->GetPosition());
		explosion->SetRotation(object->GetRotation());
		mGameWorld->AddObject(explosion);
		CreateMiniAsteroids(4, object);
		mAsteroidCount--;
	}
	if (object->GetType() == GameObjectType("MiniAsteroid"))
	{
		shared_ptr<GameObject> explosion = CreateExplosion();
		mGameWorld->PlayAudio("bang_m");
		explosion->SetPosition(object->GetPosition());
		explosion->SetRotation(object->GetRotation());
		explosion->SetScale(0.4f);
		mGameWorld->AddObject(explosion);
		
		mMiniAsteroidCount--;
		if (mAsteroidCount <= 0 && mMiniAsteroidCount <= 0){
			if (!mAlienSpaceship->IsInWorld()){
				SetTimer(500, START_NEXT_LEVEL);
			}
		}
	}
	if (object->GetType() == GameObjectType("AlienSpaceship"))
	{
		shared_ptr<GameObject> explosion = CreateExplosion();
		explosion->SetPosition(object->GetPosition());
		explosion->SetRotation(object->GetRotation());
		//explosion->SetScale();
		mGameWorld->AddObject(explosion);
		mGameWorld->PlayAudio("explosion");
		if (mAsteroidCount <= 0 && mMiniAsteroidCount <= 0){
			SetTimer(500, START_NEXT_LEVEL);
		}
	}

}

// PUBLIC INSTANCE METHODS IMPLEMENTING ITimerListener ////////////////////////

void Asteroids::OnTimer(int value)
{
	if (value == CREATE_NEW_PLAYER)
	{
		if (mAlienSpaceship != nullptr){ mAlienSpaceship->SetEngage(false); }
		mSpaceship->Reset();
		mGameWorld->AddObject(mSpaceship);
		mSpaceship->SetShieldVisibility(true);
		mSpaceship->GetShield()->SetLifespan(5000);
		mSpaceship->SetBulletSprite(bullet_sprite);
		mSpaceship->SetBulletSpeed(30);
		mSpaceship->SetHealth(mLevel * 20);
	}

	if (value == START_NEXT_LEVEL)
	{
		mLevel++;
		int num_asteroids = 10 + 2 * mLevel;
		CreateAsteroids(num_asteroids);
		mAlienSpaceship->SetLife(mLevel * 10);
		mAlienSpaceship->SetEngage(false);
		mAlienSpaceship->SetInWorld(true);
		mGameWorld->AddObject(mAlienSpaceship);
		mSpaceship->SetHealth(mLevel * 20);
	}
	if (value == SHOW_GAME_OVER)
	{
		mAlienSpaceship->SetEngage(false);
		mGameOverLabel->SetVisible(true);
		mGameWorld->StopMusic();
		mGameWorld->PlayAudio("gameover");
	}

	if (value == CREATE_NEW_BONUS)
	{	
		mGameWorld->AddObject(mLifeBonus);
	}
	if (value == CREATE_NEW_SHIELD)
	{
		mGameWorld->AddObject(mShieldPowerUp);
	}
	if (value == CREATE_NEW_BULLET_UPGRADE)
	{
		mGameWorld->AddObject(mBulletUpgrade);
	}
}


// PROTECTED INSTANCE METHODS /////////////////////////////////////////////////
void Asteroids::CreateSpaceship()
{
	// Create a raw pointer to a spaceship that can be converted to
	// shared_ptrs of different types because GameWorld implements IRefCount
	mSpaceship = make_shared<Spaceship>();
	mSpaceship->SetBoundingShape(make_shared<BoundingSphere>(mSpaceship->GetThisPtr(), 5.0f));
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("spaceship");
	shared_ptr<Sprite> spaceship_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	mSpaceship->SetSprite(spaceship_sprite);
	mSpaceship->SetScale(0.05f);    
	
	mSpaceship->SetBulletSprite(bullet_sprite);
	// Reset spaceship back to centre of the world
	mSpaceship->Reset();
	mSpaceship->SetHealth(mLevel * 20);
	
	// Return the spaceship so it can be added to the world
	mGameWorld->AddObject(mSpaceship);
	mSpaceship->CreateShield(5000);
	mPlayer.SetPlayer(mSpaceship);
}

void Asteroids::CreateAlienSpaceship(void)
{
	mAlienSpaceship = make_shared<AlienSpaceship>(10);
	mAlienSpaceship->SetBoundingShape(make_shared<BoundingSphere>(mAlienSpaceship->GetThisPtr(), 10.0f));
	Animation *alien_anim_ptr = AnimationManager::GetInstance().GetAnimationByName("alien_spaceship");
	shared_ptr<Sprite> alien_spaceship_sprite =
		make_shared<Sprite>(alien_anim_ptr->GetWidth(), alien_anim_ptr->GetHeight(), alien_anim_ptr);
	mAlienSpaceship->SetSprite(alien_spaceship_sprite);
	mAlienSpaceship->SetScale(0.10f);
	Animation *alienbullet_anim_ptr = AnimationManager::GetInstance().GetAnimationByName("alien_bullet");
	shared_ptr<Sprite> alien_bullet_sprite =
		make_shared<Sprite>(alienbullet_anim_ptr->GetWidth(), alienbullet_anim_ptr->GetHeight(), alienbullet_anim_ptr);
	mAlienSpaceship->SetBulletSprite(alien_bullet_sprite);
	mAlienSpaceship->SetEnemy(mSpaceship);
	mGameWorld->AddObject(mAlienSpaceship);
	mAlienSpaceship->SetInWorld(true);
	mPlayer.SetEnemy(mAlienSpaceship);

	
	//Create alien AI
	mAlienAI = make_shared<AlienAI>(mAlienSpaceship);
	mAlienAI->SetBoundingShape(make_shared<BoundingSphere>(mAlienAI->GetThisPtr(), 40.0f));
	mGameWorld->AddObject(mAlienAI);
}



void Asteroids::CreateAsteroids(const uint num_asteroids)
{
	mAsteroidCount = num_asteroids;
	for (uint i = 0; i < num_asteroids; i++) {
		Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("asteroid1");
		shared_ptr<Sprite> asteroid_sprite
			= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
		asteroid_sprite->SetLoopAnimation(true);
		shared_ptr<GameObject> asteroid = make_shared<Asteroid>();
		asteroid->SetBoundingShape(make_shared<BoundingSphere>(asteroid->GetThisPtr(), 10.0f));
		asteroid->SetSprite(asteroid_sprite);
		asteroid->SetScale(0.2f);
		mGameWorld->AddObject(asteroid);
	}
}

void Asteroids::CreateMiniAsteroids(const uint num_miniasteroids, shared_ptr<GameObject> object)
{
	mMiniAsteroidCount += num_miniasteroids;
	for (uint i = 0; i < num_miniasteroids; i++) {
		Animation *minianim_ptr = AnimationManager::GetInstance().GetAnimationByName("asteroid2");
		shared_ptr<Sprite> miniasteroid_sprite
			= make_shared<Sprite>(minianim_ptr->GetWidth(), minianim_ptr->GetHeight(), minianim_ptr);
		miniasteroid_sprite->SetLoopAnimation(true);
		shared_ptr<GameObject> miniasteroid = make_shared<MiniAsteroid>(object);
		miniasteroid->SetBoundingShape(make_shared<BoundingSphere>(miniasteroid->GetThisPtr(), 5.0f));
		miniasteroid->SetSprite(miniasteroid_sprite);
		miniasteroid->SetScale(0.1f);
		mGameWorld->AddObject(miniasteroid);
	}
}

void Asteroids::CreateBonuses(void)
{
	//Create life bonus and add it to the world
	mLifeBonus = make_shared<LifeBonus>();
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("lifebonus");
	shared_ptr<Sprite> lifebonus_sprite = make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	mLifeBonus->SetSprite(lifebonus_sprite);
	mLifeBonus->SetBoundingShape(make_shared<BoundingSphere>(mLifeBonus->GetThisPtr(), 10.0f));
	mLifeBonus->SetScale(0.1f);
	mGameWorld->AddObject(mLifeBonus);
	
	//Create shield bonus and add it to the world
	mShieldPowerUp = make_shared<ShieldPowerUp>();
	Animation *anim_ptr2 = AnimationManager::GetInstance().GetAnimationByName("shield_powerup");
	shared_ptr<Sprite> shieldpowerup_sprite = make_shared<Sprite>(anim_ptr2->GetWidth(), anim_ptr2->GetHeight(), anim_ptr2);
	mShieldPowerUp->SetSprite(shieldpowerup_sprite);
	mShieldPowerUp->SetBoundingShape(make_shared<BoundingSphere>(mShieldPowerUp->GetThisPtr(), 10.0f));
	mShieldPowerUp->SetScale(0.1f);
	mGameWorld->AddObject(mShieldPowerUp);
	
	//Create bullet upgrade bonus and add it to the world
	mBulletUpgrade = make_shared<BulletUpgrade>();
	Animation *anim_ptr3 = AnimationManager::GetInstance().GetAnimationByName("bulletupgrade");
	shared_ptr<Sprite> bulletupgrade_sprite = make_shared<Sprite>(anim_ptr3->GetWidth(), anim_ptr3->GetHeight(), anim_ptr3);
	mBulletUpgrade->SetSprite(bulletupgrade_sprite);
	mBulletUpgrade->SetBoundingShape(make_shared<BoundingSphere>(mBulletUpgrade->GetThisPtr(), 10.0f));
	mBulletUpgrade->SetScale(0.1f);
	mGameWorld->AddObject(mBulletUpgrade);
	
}

void Asteroids::CreateBackground(void)
{
	mBackground = make_shared<Background>(2048, 2048, 2048, 2048, "texture/space_bg.png");
	mGameWorld->AddObject(mBackground);
}

void Asteroids::CreateGUI()
{
	// Add a (transparent) border around the edge of the game display
	mGameDisplay->GetContainer()->SetBorder(GLVector2i(10, 10));
	// Create a new GUILabel and wrap it up in a shared_ptr
	mScoreLabel = make_shared<GUILabel>("Score: 0");
	mScoreLabel->SetColor(GLVector3f(50, 50, 0));
	// Set the vertical alignment of the label to GUI_VALIGN_TOP
	mScoreLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> score_component
		= static_pointer_cast<GUIComponent>(mScoreLabel);
	mGameDisplay->GetContainer()->AddComponent(score_component, GLVector2f(0.0f, 1.0f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mLivesLabel = make_shared<GUILabel>("Lives: 3");
	mLivesLabel->SetColor(GLVector3f(0, 50, 0));
	// Set the vertical alignment of the label to GUI_VALIGN_BOTTOM
	mLivesLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOM);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> lives_component = static_pointer_cast<GUIComponent>(mLivesLabel);
	mGameDisplay->GetContainer()->AddComponent(lives_component, GLVector2f(0.0f, 0.0f));


	// Create a new GUILabel and wrap it up in a shared_ptr
	mGameOverLabel = shared_ptr<GUILabel>(new GUILabel("GAME OVER"));
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mGameOverLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mGameOverLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	// Set the visibility of the label to false (hidden)
	mGameOverLabel->SetVisible(false);
	
	// Add the GUILabel to the GUIContainer  
	shared_ptr<GUIComponent> game_over_component
		= static_pointer_cast<GUIComponent>(mGameOverLabel);
	mGameDisplay->GetContainer()->AddComponent(game_over_component, GLVector2f(0.5f, 0.5f));

	
	mAlienHealthLabel = make_shared<GUILabel>();
	mAlienHealthLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	mAlienHealthLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_RIGHT);
	mAlienHealthLabel->SetColor(GLVector3f(20,0,0));
	shared_ptr<GUIComponent> alien_component
		= static_pointer_cast<GUIComponent>(mAlienHealthLabel);
	mGameDisplay->GetContainer()->AddComponent(alien_component, GLVector2f(1.0f, 1.0f));

	mPlayerHealthLabel = make_shared<GUILabel>();
	mPlayerHealthLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOM);
	mPlayerHealthLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_RIGHT);
	mPlayerHealthLabel->SetColor(GLVector3f(0,50,0));
	shared_ptr<GUIComponent> health_component = static_pointer_cast<GUIComponent>(mPlayerHealthLabel);
	mGameDisplay->GetContainer()->AddComponent(health_component, GLVector2f(1.0f, 0.0f));
}

void Asteroids::OnScoreChanged(int score)
{
	// Format the score message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Score: " << score;
	// Get the score message as a string
	std::string score_msg = msg_stream.str();
	mScoreLabel->SetText(score_msg);
}
void Asteroids::OnEnemyHit(int life_left)
{
	// Format the message using an string-based stream
	std::ostringstream msg_stream;
	for (int i = 0; i < life_left; ++i){ msg_stream << "X"; }
	msg_stream << " :Alien";
	// Get the message as a string
	std::string score_msg = msg_stream.str();
	mAlienHealthLabel->SetText(score_msg);
	mAlienHealthLabel->SetVisible(true);
	if (life_left == 0){
		mAlienHealthLabel->SetVisible(false);
	}
	else {
		mGameWorld->PlayAudio("impact");
	}
}

void Asteroids::OnPlayerHit(int health)
{
	// Format the message using an string-based stream
	std::ostringstream msg_stream;
	for (int i = 0; i < health; i++){ msg_stream << "X"; }
	msg_stream << " :Health";
	// Get the message as a string
	std::string score_msg = msg_stream.str();
	mPlayerHealthLabel->SetText(score_msg);
	mPlayerHealthLabel->SetVisible(true);
	if (health == 0){
		mPlayerHealthLabel->SetVisible(false);
	}
	else {
		mGameWorld->PlayAudio("impact");
	}
}

void Asteroids::OnPlayerKilled(int lives_left)
{
	shared_ptr<GameObject> explosion = CreateExplosion();
	explosion->SetPosition(mSpaceship->GetPosition());
	explosion->SetRotation(mSpaceship->GetRotation());
	mGameWorld->AddObject(explosion);

	mGameWorld->PlayAudio("explosion");

	// Format the lives left message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Lives: " << lives_left;
	// Get the lives left message as a string
	std::string lives_msg = msg_stream.str();
	mLivesLabel->SetText(lives_msg);
	if (lives_left > 0) { SetTimer(1000, CREATE_NEW_PLAYER); }
	if (lives_left == 0) { SetTimer(500, SHOW_GAME_OVER); }
}

void Asteroids::OnBonusPicked(int lives)
{
	std::ostringstream msg_stream;
	msg_stream << "Lives: " << lives;
	// Get the lives left message as a string
	std::string lives_msg = msg_stream.str();
	mLivesLabel->SetText(lives_msg);
	SetTimer(30000, CREATE_NEW_BONUS);
	mGameWorld->PlayAudio("bonus1");
}

void Asteroids::OnShieldPicked(void)
{
	if (mSpaceship->GetShield()->IsActive()){
		mSpaceship->GetShield()->SetLifespan(10000);
	}
	else{
		mSpaceship->GetShield()->SetLifespan(10000);
		mSpaceship->SetShieldVisibility(true);
	}
	SetTimer(60000, CREATE_NEW_SHIELD);
	mGameWorld->PlayAudio("bonus2");
}

void Asteroids::OnBulletUpgradePicked(void)
{
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("bullet2");
	shared_ptr<Sprite> bullet_sprite2 =	make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	mSpaceship->SetBulletSprite(bullet_sprite2);
	mSpaceship->IncreaseBulletSpeed(30);
	SetTimer(90000, CREATE_NEW_BULLET_UPGRADE);
	mGameWorld->PlayAudio("bonus3");
}


shared_ptr<GameObject> Asteroids::CreateExplosion()
{
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("explosion");
	shared_ptr<Sprite> explosion_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	explosion_sprite->SetLoopAnimation(false);
	shared_ptr<GameObject> explosion = make_shared<Explosion>();
	explosion->SetSprite(explosion_sprite);
	explosion->Reset();
	return explosion;
}



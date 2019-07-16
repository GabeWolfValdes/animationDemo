//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Framework.h"

#include "Game.h"
#include "MathEngine.h"
#include "Texture.h"
#include "InputMan.h"
#include "GameObjectMan.h"
#include "CameraMan.h"
#include "GameObjectAnim.h"
#include "Timer.h"
#include "Skeleton.h"
#include "Animation.h"
#include "GraphicsDebugMessage.h"
#include "ShaderObject.h"

#include "ShaderMan.h"

#include "AnimMan.h"
#include "ModelMan.h"
#include "Image.h"
#include "GraphicsObject_Sprite.h"
#include "GraphicsObject_WireframeConstantColor.h"
#include "GraphicsObject_FlatTexture.h"
#include "SpriteModel.h"
#include "GameObject2D.h"
#include "ImageMan.h"

#include "SpuModel.h"
#include "AnimationSkinModel.h"
#include "GameObjectRigid.h"


#include "SSBO.h"


// TO DO: ----------------------------------
//extern GameObjectAnim *pFirstBone;


//-----------------------------------------------------------------------------
//  Game::Game()
//		Game Engine Constructor
//-----------------------------------------------------------------------------
Game::Game(const char * const pWindowName, const int Width, const int Height)
	:Engine(pWindowName, Width, Height)
{
	assert(pWindowName);
	this->globalTimer.Tic();
	this->intervalTimer.Tic();
}


//-----------------------------------------------------------------------------
// Game::Initialize()
//		Allows the engine to perform any initialization it needs to before 
//      starting to run.  This is where it can query for any required services 
//      and load any non-graphic related content. 
//-----------------------------------------------------------------------------
void Game::Initialize()
{
	// Start the managers
	InputMan::Create();
}


//-----------------------------------------------------------------------------
// Game::LoadContent()
//		Allows you to load all content needed for your engine,
//	    such as objects, graphics, etc.
//-----------------------------------------------------------------------------
void Game::LoadContent()
{
	// During init, enable debug output

	//GraphicsEnableDebug();

	// Camera setup

	Camera *pCam0 = new Camera(Camera::Type::PERSPECTIVE_3D);
	pCam0->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);

	pCam0->setPerspective(35.0f, float(pCam0->getScreenWidth()) / float(pCam0->getScreenHeight()), 1.0f, 20000.0f);
	//for human
	pCam0->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(-500.0f, -500.0f, 500.0f), Vect(2000.0f, 0.0f, 0.0f));

	pCam0->updateCamera();
	CameraMan::Add(Camera::Name::CAMERA_0, pCam0);
	CameraMan::SetCurrent(Camera::Name::CAMERA_0, Camera::Type::PERSPECTIVE_3D);


	pCam0 = new Camera(Camera::Type::PERSPECTIVE_3D);
	pCam0->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);

	//for teddy
	pCam0->setPerspective(35.0f, float(pCam0->getScreenWidth()) / float(pCam0->getScreenHeight()), 1.0f, 20000.0f);
	pCam0->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 50.0f), Vect(400.0f, 0.0f, 0.0f));

	pCam0->updateCamera();
	CameraMan::Add(Camera::Name::CAMERA_2, pCam0);
	CameraMan::SetCurrent(Camera::Name::CAMERA_2, Camera::Type::PERSPECTIVE_3D);

	//for frigate
	pCam0 = new Camera(Camera::Type::PERSPECTIVE_3D);
	pCam0->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
	pCam0->setPerspective(35.0f, float(pCam0->getScreenWidth()) / float(pCam0->getScreenHeight()), 1.0f, 20000.0f);
	pCam0->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 50.0f), Vect(0.0f, 7000.0f, 0.0f));
	pCam0->updateCamera();
	CameraMan::Add(Camera::Name::CAMERA_3, pCam0);


	//PA1 adding 2d
	Camera *pCam2D = new Camera(Camera::Type::ORTHOGRAPHIC_2D);

	pCam2D->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
	pCam2D->setOrthographic(-pCam2D->getScreenWidth() / 2.0f, pCam2D->getScreenWidth() / 2.0f,
		-pCam2D->getScreenHeight() / 2.0f, pCam2D->getScreenHeight() / 2.0f,
		1.0f, 1000.0f);
	pCam2D->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 0.0f, -1.0f), Vect(0.0f, 0.0f, 2.0f));

	// Holder for the current 2D  cameras
	CameraMan::Add(Camera::Name::CAMERA_1, pCam2D);
	CameraMan::SetCurrent(Camera::Name::CAMERA_1, Camera::Type::ORTHOGRAPHIC_2D);

	// Load the model
	SpriteModel *pSpriteModel = new SpriteModel("SpriteModel.azul");
	assert(pSpriteModel);
	ModelMan::Add(pSpriteModel);

	// Create/Load Shader 
	ShaderObject *pShaderObject_sprite = new ShaderObject(ShaderObject::Name::SPRITE, "spriteRender");
	assert(pShaderObject_sprite);

	



	// Textures

	Texture::Add("Rocks.tga", Texture::Name::ROCKS);
	Texture::Add("Stone.tga", Texture::Name::STONES);
	Texture::Add("RedBrick.tga", Texture::Name::RED_BRICK);
	Texture::Add("Duckweed.tga", Texture::Name::DUCKWEED);
	Texture::Add("Aliens.tga", Texture::Name::INVADERS);


	//PA1 adding image for 2d
	Image *pImageGreen = new Image(Image::Name::Alien_Green, Texture::Name::INVADERS, Rect(136.0f, 64.0f, 85.0f, 63.0f));
	assert(pImageGreen);
	ImageMan::Add(pImageGreen);

	// Sprite
	GraphicsObject_Sprite *pGraphics_Sprite = new GraphicsObject_Sprite(pSpriteModel, pShaderObject_sprite, pImageGreen, Rect(455, 155, 150, 150));
	GameObject2D *pA1 = new GameObject2D(pGraphics_Sprite);
	GameObjectMan::Add(pA1, GameObjectMan::ObjType::FLAT);
	pA1->posX = 900.0f;
	pA1->posY = 450.0f;
	pA1->SetName("ALIEN");


	//COMPUTE SHADERS
	ShaderObject* pShader = new ShaderObject(ShaderObject::Name::TEST_COMPUTE, "computeTest", GL_COMPUTE_SHADER);
	ShaderMan::Add(pShader);

	pShader = new ShaderObject(ShaderObject::Name::WORLD_MATRIX_COMPUTE, "worldMatrixCompute", GL_COMPUTE_SHADER);
	ShaderMan::Add(pShader);

	pShader = new ShaderObject(ShaderObject::Name::BONE_WORLD_COMPUTE, "boneWorldCompute", GL_COMPUTE_SHADER);
	ShaderMan::Add(pShader);

	pShader = new ShaderObject(ShaderObject::Name::BONE_WORLD_COMPUTE_TEST, "boneWorldComputeTest", GL_COMPUTE_SHADER);
	ShaderMan::Add(pShader);

	pShader = new ShaderObject(ShaderObject::Name::SKINLIGHT, "skinLightRender");
	ShaderMan::Add(pShader);

	AnimMan::Initialize();





	//frigate 
	ShaderObject *texShader = new ShaderObject(ShaderObject::Name::TEXTURE_SIMPLE, "textureRender");
	assert(texShader);

	GameObject* pGameObj;
	SpuModel *frigate = new SpuModel("space_frigate");
	Texture::Add(frigate->getTexName(), Texture::Name::FRIGATE);
	GraphicsObject_FlatTexture *frigateGraphics = new GraphicsObject_FlatTexture(frigate, texShader, Texture::Name::FRIGATE);
	//frigateGraphics->SetCCW();
	pGameObj = new GameObjectRigid(frigateGraphics);
	Matrix frigatePos(Matrix::TransType::Trans, 0.0f, 7000.0f, 0.0f);
	pGameObj->SetWorld(&frigatePos);
	//pGameObj->SetSelfRotX(0.1f);
	pGameObj->SetName("FRIGATE1");
	//pGameObj->SetScale(0.02f, 0.02f, 0.02f);
	GameObjectMan::Add(pGameObj, GameObjectMan::ObjType::NONANIM);



	//UNUSED_VAR(temp);
}

	//-----------------------------------------------------------------------------
	// Game::Update()
	//      Called once per frame, update data, tranformations, etc
	//      Use this function to control process order
	//      Input, AI, Physics, Animation, and Graphics
	//-----------------------------------------------------------------------------
	void Game::Update(float )
	{


		InputMan::Update();
		// Note: these static should be tied into an animation system
		static Time tCurrent(Time::ZERO);

		// Mark our end time.
		this->intervalTimer.Toc();
		this->intervalTimer.Tic();

		// Update camera - make sure everything is consistent
		Camera *pCam = CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);
		pCam->updateCamera();

		pCam = CameraMan::GetCurrent(Camera::Type::ORTHOGRAPHIC_2D);
		pCam->updateCamera();

		// ------------------------------------------------
		// Animate ME
		// ------------------------------------------------
			//Skeleton s("humanoidheirarchy.spu");

			//ProcessAnimation(tCurrent);
			AnimMan::Update();

			// Game objects
			GameObjectMan::Update(this->globalTimer.Toc());

			//AnimMan::TimeUpdate();
			

	}

//-----------------------------------------------------------------------------
// Game::Draw()
//		This function is called once per frame
//	    Use this for draw graphics to the screen.
//      Only do rendering here
//-----------------------------------------------------------------------------
void Game::Draw()
{
	GameObjectMan::Draw();
}


//-----------------------------------------------------------------------------
// Game::UnLoadContent()
//       unload content (resources loaded above)
//       unload all content that was loaded before the Engine Loop started
//-----------------------------------------------------------------------------
void Game::UnLoadContent()
{
	// Todo fix this - verify removal
}

//------------------------------------------------------------------
// Game::ClearBufferFunc()
// Allows user to change the way the clear buffer function works
//------------------------------------------------------------------
void Game::ClearBufferFunc()
{
	const GLfloat grey[] = { 0.5f, 0.5f, 0.5f, 0.0f };
	const GLfloat one = 1.0f;

	glViewport(0, 0, info.windowWidth, info.windowHeight);
	glClearBufferfv(GL_COLOR, 0, grey);
	glClearBufferfv(GL_DEPTH, 0, &one);
}

// --- End of File --------------------------------------------------

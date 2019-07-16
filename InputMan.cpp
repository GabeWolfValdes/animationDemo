//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>
#include "InputMan.h"
#include "AnimMan.h"
#include "CameraMan.h"
InputMan *InputMan::pInputMan = 0;

void InputMan::Create( )
{
	InputMan::privCreate( );
}

InputMan::InputMan( )
{
	this->pKeyboard = new Keyboard(  );
	assert(this->pKeyboard);
}

void InputMan::privCreate(   )
{
	InputMan::pInputMan = new InputMan(  );
	assert(pInputMan);
}

InputMan::~InputMan()
{
	delete this->pKeyboard;
}

Keyboard *InputMan::GetKeyboard()
{
	InputMan *pMan = InputMan::privInstance();
	return pMan->pKeyboard;
}

Mouse *InputMan::GetMouse()
{
	InputMan *pMan = InputMan::privInstance();
	return pMan->pMouse;
}

InputMan *InputMan::privInstance()
{
	assert( pInputMan );
	return pInputMan;
}
void InputMan::Update() {
	Keyboard *pKey = InputMan::GetKeyboard();

	if (pKey->GetKeyDown(Keyboard::KEY_H))
	{
		AnimMan::SwitchHuman();
	}

	if (pKey->GetKeyDown(Keyboard::KEY_T))
	{
		AnimMan::SwitchTeddy();
	}

	if (pKey->GetKeyState(Keyboard::KEY_W))
	{
		CameraMan::Rotate3dU();
	}

	if (pKey->GetKeyState(Keyboard::KEY_S))
	{
		CameraMan::Rotate3dD();
	}

	if (pKey->GetKeyState(Keyboard::KEY_D))
	{
		CameraMan::Rotate3dR();
	}


	if (pKey->GetKeyState(Keyboard::KEY_A)) 
	{
		CameraMan::Rotate3dL();
	}

	if (pKey->GetKeyState(Keyboard::KEY_Q))
	{
		CameraMan::Move3dF();
	}

	if (pKey->GetKeyState(Keyboard::KEY_E))
	{
		CameraMan::Move3dB();
	}


	if (pKey->GetKeyDown(Keyboard::KEY_1)) {
		CameraMan::SetCurrent(Camera::Name::CAMERA_0, Camera::Type::PERSPECTIVE_3D);
		CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(-500.0f, -500.0f, 500.0f), Vect(2000.0f, 0.0f, 0.0f));
	}
	if (pKey->GetKeyDown(Keyboard::KEY_2)) {
		CameraMan::SetCurrent(Camera::Name::CAMERA_2, Camera::Type::PERSPECTIVE_3D);
		CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 50.0f), Vect(400.0f, 0.0f, 0.0f));

	}
	if (pKey->GetKeyDown(Keyboard::KEY_3)) {
		CameraMan::SetCurrent(Camera::Name::CAMERA_3, Camera::Type::PERSPECTIVE_3D);
		CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 50.0f), Vect(0.0f, 7000.0f, 0.0f));

	}
	if (pKey->GetKeyDown(Keyboard::KEY_B)) {
		AnimMan::StartBlending();
	}
}
// --- End of File --------------------------------------------------
//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//---------------------------------------------------------------------------

#include "Framework.h"

#include "BoundingSphereGameObject.h"
#include "GameObject.h"
#include "CameraMan.h"
#include "GraphicsObject_WireframeConstantColor.h"


BoundingSphereGameObject::BoundingSphereGameObject(GraphicsObject  *pGraphicsObject)
	: GameObject(pGraphicsObject)
{
	this->pRefGameObj = nullptr;
	this->pScale = new Vect();
	this->pTrans = new Vect();
}

BoundingSphereGameObject::~BoundingSphereGameObject()
{
	delete this->pScale;
	delete this->pTrans;
}

void BoundingSphereGameObject::SetBoundingSphereReference(GameObject *pGameObj)
{
	this->pRefGameObj = pGameObj;
}

void BoundingSphereGameObject::privUpdate(Time )
{
	// Goal: update the world matrix

	Matrix Scale(Matrix::Scale, *this->pScale);
	Matrix TransA(Matrix::Trans, *this->pTrans);

	// world matrix
	*this->pWorld = Scale * TransA;


	
	// Flip the color based on camera_0
	Camera *pCam = CameraMan::Find(Camera::Name::CAMERA_0);
	Sphere *pCurrSphere = this->pGraphicsObject->pCurrSphere;

	// Architecture issue... this doesn't feel good.
	GraphicsObject_WireframeConstantColor *pGObj = (GraphicsObject_WireframeConstantColor *)this->pGraphicsObject;
	if (pCam->CullTest(*pCurrSphere) == Camera::CullResult::CULL_OUTSIDE)
	{
		pGObj->pWireColor->set(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		pGObj->pWireColor->set(0.0f, 1.0f, 0.0f, 1.0f);
	}


}

void BoundingSphereGameObject::privUpdateBoundingSphereReference(Time currentTime)
{
	// incase the reference object wasn't updated first
	// update it now.

	this->pRefGameObj->Update(currentTime);

	// set the center and scale of bound box based on ref game object's world coord
	Sphere *pRefCurrSphere = this->pRefGameObj->GetGraphicsObject()->pCurrSphere;
	this->pScale->set(pRefCurrSphere->rad, pRefCurrSphere->rad, pRefCurrSphere->rad);
	this->pTrans->set(pRefCurrSphere->cntr);

	Sphere *pCurrSphere = this->GetGraphicsObject()->pCurrSphere;
	*pCurrSphere = *pRefCurrSphere;
}

void BoundingSphereGameObject::Update(Time currentTime)
{
	// Set Translation & Scale from reference
    this->privUpdateBoundingSphereReference(currentTime);

	// Goal: update the world matrix
	this->privUpdate(currentTime);

	// push to graphics object
	this->pGraphicsObject->SetWorld(*this->pWorld);
}

// End of File


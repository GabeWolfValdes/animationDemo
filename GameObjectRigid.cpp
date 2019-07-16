//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "GameObjectRigid.h"
#include "GraphicsObject.h"

GameObjectRigid::GameObjectRigid(GraphicsObject  *pGraphicsObject)
	: GameObject(pGraphicsObject)
{
	assert(pGraphicsObject != 0);

	this->pScale = new Vect(0.2f, 0.2f, 0.2f);
	assert(this->pScale);

	this->pPos = new Vect(0.0f, 7000.0f, 0.0f);
	assert(this->pPos);

	this->angle = 0.0f;

	


}

GameObjectRigid::~GameObjectRigid()
{
	delete this->pPos;
	delete this->pScale;
}

void GameObjectRigid::privUpdate(Time )
{
	Matrix T = Matrix(Matrix::Trans, *this->pPos);
	Matrix S = Matrix(Matrix::Scale, *this->pScale);

	angle += 0.03f;
	Matrix R = Matrix(Matrix::Rot_Z, angle);

	Matrix M = S * R * T;
	//Matrix M = S *  T;

	*this->pWorld = M;
}

void GameObjectRigid::Update(Time currentTime)
{
	// Goal: update the world matrix
	this->privUpdate(currentTime);

	// update the bounding volume based on world matrix
	//this->baseUpdateBoundingSphere();

	// push to graphics object
	this->pGraphicsObject->SetWorld(*this->pWorld);
}


//--- End of File ------------------------------------------------------------

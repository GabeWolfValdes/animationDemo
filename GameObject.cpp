//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game.h"
#include "MathEngine.h"
#include "GameObject.h"
#include "GraphicsObject.h"
#include "Model.h"
#include "FrameBucket.h"

// YUCK - REFACTOR
//extern Game *pGame;

Matrix *GameObject::GetWorld()
{
	return this->pWorld;
}

GameObject::GameObject(GraphicsObject  *pGraphicsObject)
	: pGraphicsObject(pGraphicsObject)
{
	this->pWorld = new Matrix(Matrix::Identity);
	assert(pWorld);

	assert(pGraphicsObject != 0);

	this->drawEnable = true;

}

GraphicsObject *GameObject::GetGraphicsObject()
{
	return this->pGraphicsObject;
}

GameObject::~GameObject()
{
	delete this->pWorld;
	delete this->pGraphicsObject;
}

void GameObject::baseUpdateBoundingSphere()
{
	// Get the ref Sphere
	Sphere *pRefSphere = pGraphicsObject->GetModel()->pRefSphere;
	assert(pRefSphere);

	// Get the curr Sphere
	Sphere *pCurrSphere = pGraphicsObject->pCurrSphere;
	assert(pCurrSphere);

	// Update... cntr process through the world
	// Assuming a uniform scaling
	Vect A = pRefSphere->cntr;
	Vect radius(1.0f, 0.0f, 0.0f);
	Vect B = A + radius;
	Vect A_out = A * (*this->pWorld);
	Vect B_out = B * (*this->pWorld);

    pCurrSphere->rad = (B_out[x] - A_out[x])*pRefSphere->rad;
	pCurrSphere->cntr = A_out;
}

void GameObject::SetWorld(Matrix *_pWorld)
{
	assert(_pWorld);
	*this->pWorld = *_pWorld;
}

bool GameObject::GetDrawEnable()
{
	return this->drawEnable;
}

void GameObject::SetDrawEnable(bool b)
{
	this->drawEnable = b;
}

void GameObject::Draw()
{
	GraphicsObject *pGraphicsObj = this->GetGraphicsObject();
	assert(pGraphicsObj);

	pGraphicsObj->Render();
}

int GameObject::GetIndex()
{
	return INT_MIN;
}

//--- End of File ------------------------------------------------------------
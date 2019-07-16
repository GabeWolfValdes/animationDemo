#include "GameObject2D.h"


GameObject2D::GameObject2D(GraphicsObject_Sprite  *pGraphicsObject)
	: GameObject(pGraphicsObject)
{
	assert(pGraphicsObject != 0);

	this->angle = 0.0f;
	this->posX = pGraphicsObject->origPosX;
	this->posY = pGraphicsObject->origPosY;
	this->scaleX = 1.0f;
	this->scaleY = 1.0f;
}

GameObject2D::~GameObject2D()
{
}


void GameObject2D::Update(Time currentTime)
{
	UNUSED_VAR(currentTime);

	// Goal: update the world matrix
	Matrix Scale(Matrix::Scale, this->scaleX, this->scaleY, 1.0f);
	Matrix Rot(Matrix::Rot_Z, this->angle);
	Matrix Trans(Matrix::Trans, this->posX, this->posY, 0.0f);

	*this->pWorld = Scale * Rot * Trans;

	// push to graphics object
	this->pGraphicsObject->SetWorld(*this->pWorld);
}



//--- End of File ------------------------------------------------------------

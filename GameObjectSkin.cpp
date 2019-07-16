#include "GameObjectSkin.h"
//#include "GraphicsObject.h"

#include "AnimMan.h"
#include "Animation.h"

//#include "FrameBucket.h"



GameObjectSkin::GameObjectSkin(GraphicsObject * graphicsObject)
	: GameObject(graphicsObject)
{
	assert(graphicsObject != 0);

	this->pDof = new Vect(0.0f, 0.0f, 0.0f);
	assert(pDof);

	this->pUp = new Vect(0.0f, 1.0f, 0.0f);
	assert(pUp);

	this->pScale = new Vect(1.0f, 1.0f, 1.0f);
	assert(pScale);

	this->pPos = new Vect(0.0f, 0.0f, 0.0f);
	assert(pPos);

	this->pLocal = new Matrix(Matrix::Identity);
	assert(pLocal);

	this->drawEnable = true;
}

GameObjectSkin::~GameObjectSkin()
{
	delete this->pDof;
	delete this->pUp;
	delete this->pPos;
	delete this->pScale;
	delete this->pLocal;
}

void GameObjectSkin::Update(Time currentTime)
{
	this->privUpdate(currentTime);

	this->pGraphicsObject->SetWorld(*this->pWorld);
}


skinType GameObjectSkin::GetType()
{
	return this->type;
}

void GameObjectSkin::SetType(skinType t)
{
	this->type = t;
}

void GameObjectSkin::privUpdate(Time currentTime)
{

	* this->pLocal = this->pGraphicsObject->GetWorld();

	GameObject *pBoneParent = (GameObject *)this->GetParent();
	assert(pBoneParent != 0);

	Matrix ParentWorld = *pBoneParent->GetWorld();

	Matrix T = Matrix(Matrix::Trans, *this->pPos);
	Matrix S = Matrix(Matrix::Scale, *this->pScale);

	//angle += 0.03f;
	//Matrix R = Matrix(Matrix::Rot_Z, angle);

	//Matrix M = S * R * T;
	//Matrix M = S * T;

	//*this->pLocal = M;



	// Goal: update the world matrix
	//*this->pWorld = *this->pLocal * ParentWorld;
	// Goal: update the world matrix
	*this->pWorld = *this->pLocal * ParentWorld;

	//send over the matrix to the animation to later be sent to grapphics object
	//if (this->type == skinType::HUMAN) {
	//	AnimMan::GetActiveHuman()->SetWorldArray(indexBoneArray, *this->pWorld);

	//}
	//else {
	//	AnimMan::GetActiveTeddy()->SetWorldArray(indexBoneArray, *this->pWorld);
	//}


}

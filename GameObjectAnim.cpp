//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "GameObjectAnim.h"
#include "GraphicsObject.h"
//#include "FrameBucket.h"
//
#include "AnimMan.h"
#include "Animation.h"

#include "FrameBucket.h"
#include "WorldIndicesTable.h"
#include "ShaderMan.h"
#include "SSBO.h"
//extern Frame_Bucket *pHead;

GameObjectAnim::GameObjectAnim(GraphicsObject  *pGraphicsObject)
	: GameObject(pGraphicsObject)
{
	assert(pGraphicsObject != 0);

	this->pDof = new Vect(0.0f, 0.0f, 0.0f);
	assert(pDof);

	this->pUp = new Vect(0.0f, 1.0f, 0.0f);
	assert(pUp);

	this->pScale = new Vect(1.0f, 1.0f, 1.0f);
	assert(pScale);

	this->pPos = new Vect(0.0f, 0.0f, 0.0f);
	assert(pPos);

	this->indexBoneArray = 0;

	this->pLocal = new Matrix(Matrix::Identity);
	assert(pLocal);

	this->pBoneOrientation = new Matrix(Matrix::Identity);
	assert(pBoneOrientation);

}

GameObjectAnim::~GameObjectAnim()
{
	delete this->pDof;
	delete this->pUp;
	delete this->pPos;
	delete this->pScale;
	delete this->pLocal;
}

void GameObjectAnim::privUpdate(Time )
{

	Matrix temp;
	if (this->type == boneType::HUMAN) {
		//testing
		 //temp = WorldIndexTable::humanoid->GetBoneWorld(Matrix(Matrix::SpecialType::Identity), AnimMan::GetActiveHuman()->pHead, this->indexBoneArray);
		 //temp = WorldIndexTable::humanoid->GetBoneWorld(Matrix(Matrix::SpecialType::Identity), AnimMan::GetBlendHead(), this->indexBoneArray);
	}
	else {
		//temp = WorldIndexTable::teddy->GetBoneWorld(Matrix(Matrix::SpecialType::Identity), AnimMan::GetActiveTeddy()->pHead, this->indexBoneArray);
	}

	//*this->pWorld = temp;


	//send over the matrix to the animation to later be sent to grapphics object
	if (this->type == boneType::HUMAN) {
		//AnimMan::GetActiveHuman()->SetWorldArray(indexBoneArray, *this->pWorld);
		
	}
	else {
		//AnimMan::GetActiveTeddy()->SetWorldArray(indexBoneArray, *this->pWorld);
	}

}

void GameObjectAnim::Update(Time currentTime)
{
	// Goal: update the world matrix
	this->privUpdate(currentTime);

	// update the bounding volume based on world matrix
	this->baseUpdateBoundingSphere();

	// push to graphics object
	Matrix mTmp = *this->pBoneOrientation;

	// push to graphics object
	this->pGraphicsObject->SetWorld(mTmp);
}

int GameObjectAnim::GetIndex()
{
	return this->indexBoneArray;
}

void GameObjectAnim::SetIndex(int val)
{
	this->indexBoneArray = val;
}
void GameObjectAnim::SetType(boneType t)
{
	this->type = t;
}

void GameObjectAnim::SetBoneOrientation(const Matrix &tmp)
{
	*this->pBoneOrientation = tmp;
}

Matrix GameObjectAnim::GetBoneOrientation(void) const
{
	return Matrix(*this->pBoneOrientation);
}


//--- End of File ------------------------------------------------------------

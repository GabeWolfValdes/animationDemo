//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_ANIM_H
#define GAME_OBJECT_ANIM_H

#include "GameObject.h"
#include "MathEngine.h"
#include "GraphicsObject.h"

enum boneType {
	TEDDY,
	HUMAN
};


class GameObjectAnim: public GameObject
{
public:
	GameObjectAnim(GraphicsObject  *graphicsObject);

	// Big four
	GameObjectAnim() = delete;
	GameObjectAnim(const GameObjectAnim &) = delete;
	GameObjectAnim &operator=(GameObjectAnim &) = delete;
	virtual ~GameObjectAnim() override;

	virtual void Update(Time currentTime);
	virtual int GetIndex() override;

	// Accessors
	Matrix GetBoneOrientation(void) const;
	void SetBoneOrientation(const Matrix &);
	void SetIndex(int val);

	void SetType(boneType t);

private:
	void privUpdate(Time currentTime);

public:
	Vect *pScale;
	Vect *pPos;
	Vect *pDof;
	Vect *pUp;

	Matrix      *pLocal;
	Matrix		*pBoneOrientation;
	int         indexBoneArray;
	boneType type;
};


#endif

//--- End of File ------------------------------------------------------------

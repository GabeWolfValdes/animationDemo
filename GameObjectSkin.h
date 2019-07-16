//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_SKIN_H
#define GAME_OBJECT_SKIN_H

#include "GameObject.h"
#include "MathEngine.h"
#include "GraphicsObject.h"

enum skinType {
	TEDDY_SKIN,
	HUMAN_SKIN
};


class GameObjectSkin : public GameObject
{
public:
	GameObjectSkin(GraphicsObject  *graphicsObject);

	// Big four
	GameObjectSkin() = delete;
	GameObjectSkin(const GameObjectSkin &) = delete;
	GameObjectSkin &operator=(GameObjectSkin &) = delete;
	virtual ~GameObjectSkin() override;

	virtual void Update(Time currentTime);
	//virtual int GetIndex() override;

	// Accessors
	//Matrix GetBoneOrientation(void) const;
	//void SetBoneOrientation(const Matrix &);
	//void SetIndex(int val);
	skinType GetType();

	void SetType(skinType t);

private:
	void privUpdate(Time currentTime);

public:
	Vect *pScale;
	Vect *pPos;
	Vect *pDof;
	Vect *pUp;

	Matrix      *pLocal;
	//Matrix		*pBoneOrientation;
	//int      indexBoneArray;
	skinType type;
};


#endif

//--- End of File ------------------------------------------------------------

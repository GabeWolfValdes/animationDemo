//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_RIGID_H
#define GAME_OBJECT_RIGID_H

#include "GameObject.h"
#include "MathEngine.h"
#include "GraphicsObject.h"

// Keenan(33)
class GameObjectRigid : public GameObject
{
public:
	GameObjectRigid(GraphicsObject  *graphicsObject);

	// Big four
	GameObjectRigid() = delete;
	GameObjectRigid(const GameObjectRigid &) = delete;
	GameObjectRigid &operator=(GameObjectRigid &) = delete;
	virtual ~GameObjectRigid() override;

	virtual void Update(Time currentTime);

private:
	void privUpdate(Time currentTime);

public:
	Vect *pScale;
	Vect *pPos;
	float angle;

};


#endif

//--- End of File ------------------------------------------------------------

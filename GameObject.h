//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "MathEngine.h"
//#include "ShaderObject.h"
//#include "Model.h"
#include "GraphicsObject.h"
#include "PCSNode.h"

class GameObject : public PCSNode
{
public:
	GameObject(GraphicsObject  *graphicsObject);

	// Big four
	GameObject() = delete;
	GameObject(const GameObject &) = delete;
	GameObject &operator=(GameObject &) = delete;
	virtual ~GameObject();

	virtual void Draw();
	virtual void Update(Time currentTime) = 0;
	virtual int GetIndex();

	// Accessors
	GraphicsObject *GetGraphicsObject();
	Matrix *GetWorld();
	void SetWorld(Matrix *_pWorld);


	//new PA1
	bool GetDrawEnable();
	void SetDrawEnable(bool);

protected:
	Matrix *pWorld;
	void baseUpdateBoundingSphere();

protected:
	Sphere *pSphere;
	GraphicsObject  *pGraphicsObject;
	bool drawEnable;
	char pad[3];
};


#endif

//--- End of File ------------------------------------------------------------

//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H

#include "RenderMaterial.h"

class Matrix;
class ShaderObject;
class Model;
class Sphere;

class GraphicsObject: private RenderMaterial
{
public:
	GraphicsObject(Model *model, ShaderObject *pShaderObj);

	GraphicsObject() = delete;
	virtual ~GraphicsObject();

	void Render();
	void SetWorld( Matrix &_world);

	Model *GetModel() const;
	Matrix &GetWorld() ;

	Sphere			*pCurrSphere;

protected:
	Model			*pModel;
	Matrix			*pWorld;
	ShaderObject	*pShaderObj;
};

#endif

//--- End of File -------------------------------------------------------------


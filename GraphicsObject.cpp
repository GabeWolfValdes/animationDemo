//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>

#include "GraphicsObject.h"
#include "MathEngine.h"
#include "Model.h"

GraphicsObject::GraphicsObject(Model *_pModel, ShaderObject *_pShaderObj)
	: pModel(_pModel), pShaderObj(_pShaderObj)
{
	assert(_pModel);
	assert(_pShaderObj);
	this->pWorld = new Matrix(Matrix::SpecialType::Identity);
	this->pCurrSphere = new Sphere();
}

GraphicsObject::~GraphicsObject()
{
	delete this->pWorld;
	delete this->pCurrSphere;
}

void GraphicsObject::Render(   )
{
	this->SetState();
	this->SetDataGPU();
	this->Draw();
	this->RestoreState();
}

Model *GraphicsObject::GetModel() const
{
	return this->pModel;
}

Matrix &GraphicsObject::GetWorld() 
{
	return *this->pWorld;
}

void GraphicsObject::SetWorld( Matrix &_world)
{
	*this->pWorld = _world;
}


//--- End of File -------------------------------------------------------------

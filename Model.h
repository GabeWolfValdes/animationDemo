//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MODEL_H
#define MODEL_H

#include "sb7.h"

#include "MathEngine.h"
#include "BoundingSphere.h"

class Model
{
public:
	Model();
	Model(Model &copyModel) = delete;
	Model & operator = (Model &copyModel) = delete;
	virtual ~Model();

	// Data
	int numVerts;
	int numTris;

	GLuint vao;
	GLuint vbo[4];

	// Reference sphere... before any transformations are applied
	Sphere *pRefSphere;

protected:
	// load VAO
	virtual void privCreateVAO(const char * const pModelFileName) = 0;

public:
	Model* next = 0;
	Model* prev = 0;

};


#endif

// --- End of File --------------------------------------------------

//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>
#include <sb7.h>

#include "CubeModel.h"
#include "ModelFileHeader.h"
#include "File.h"
#include "BoundingSphere.h"
#include "Trace.h"

struct Vert_xyzuvn
{
	float x;
	float y;
	float z;
	float u;
	float v;
	float nx;
	float ny;
	float nz;
};

struct Tri_index
{
	unsigned int v0;
	unsigned int v1;
	unsigned int v2;
};


void CubeModel::privCreateVAO(const char * const pModelFileName)
{
	File::Handle fh2;
	File::Error  ferror;

	ferror = File::Open(fh2, pModelFileName, File::Mode::READ);
	assert(ferror == File::Error::SUCCESS);

	// Read the Hdr
	ModelFileHeader modelHdr;
	ferror = File::Read(fh2, &modelHdr, sizeof(ModelFileHeader));
	assert(ferror == File::Error::SUCCESS);

	// Using the hdr, allocate the space for the buffers
	this->numVerts = modelHdr.numVerts;
	this->numTris = modelHdr.numTriList;

	// allocate buffers
	Vert_xyzuvn  *cubeData = new Vert_xyzuvn[(unsigned int)modelHdr.numVerts];
	Tri_index *triList = new Tri_index[(unsigned int)modelHdr.numTriList];

	// Read verts
	ferror = File::Seek(fh2, File::Location::BEGIN, (int)modelHdr.vertBufferOffset);
	assert(ferror == File::Error::SUCCESS);

	ferror = File::Read(fh2, cubeData, sizeof(Vert_xyzuvn) * this->numVerts);
	assert(ferror == File::Error::SUCCESS);

	// Read trilist
	ferror = File::Seek(fh2, File::Location::BEGIN, (int)modelHdr.triListBufferOffset);
	assert(ferror == File::Error::SUCCESS);

	ferror = File::Read(fh2, triList, sizeof(Tri_index) * this->numTris);
	assert(ferror == File::Error::SUCCESS);

	ferror = File::Close(fh2);
	assert(ferror == File::Error::SUCCESS);

	// Create a VAO
	glGenVertexArrays(1, &this->vao);
	assert(this->vao != 0);
	glBindVertexArray(this->vao);

	// Create a VBO
	glGenBuffers(2, &this->vbo[0]);
	assert(this->vbo[0] != 0);

	// Find Bounding Volume - quick hack
	// MOVE this to converter!!!!!
	Vect *pVerts = new Vect[(unsigned int)this->numVerts];
	for (int i = 0; i < this->numVerts; i++)
	{
		pVerts[i].set(cubeData[i].x, cubeData[i].y, cubeData[i].z);
		//Trace::out("%d:  %f %f %f\n", i, pVerts[i][x], pVerts[i][y], pVerts[i][z]);
	}

	assert(this->pRefSphere);
	this->pRefSphere->RitterSphere(pVerts, this->numVerts);
	//Trace::out("Ritter: cntr:%f %f %f  rad: %f ", this->pRefSphere->cntr[x], this->pRefSphere->cntr[y], this->pRefSphere->cntr[z], this->pRefSphere->rad);

	delete[] pVerts;

	// Load the combined data: ---------------------------------------------------------

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);

		// load the data to the GPU
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), cubeData, GL_STATIC_DRAW);

		// Define an array of generic vertex attribute data

			// Vert data is location: 0  (used in vertex shader)
			// todo make a table or enum
			void *offsetVert = (void *)((unsigned int)&cubeData[0].x - (unsigned int)cubeData);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetVert);
			glEnableVertexAttribArray(0);

			// Texture data is location: 1  (used in vertex shader)
			void *offsetTex = (void *)((unsigned int)&cubeData[0].u - (unsigned int)cubeData);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
			glEnableVertexAttribArray(1);

			// normals data in location 2 (used in vertex shader
			void *offsetNorm = (void *)((unsigned int)&cubeData[0].nx - (unsigned int)cubeData);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
			glEnableVertexAttribArray(2);

	// Load the index data: ---------------------------------------------------------

		/* Bind our 2nd VBO as being the active buffer and storing index ) */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);

		/* Copy the index data to our buffer */
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), triList, GL_STATIC_DRAW);

}


CubeModel::CubeModel(const char * const pModelFileName)
	: Model()
{
	assert(pModelFileName);
	//this->vbo = new GLuint[2];
	this->privCreateVAO(pModelFileName);
}

CubeModel::~CubeModel()
{
	// remove anything dynamic here
}

// --- End of File --------------------------------------------------

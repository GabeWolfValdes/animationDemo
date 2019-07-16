//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>

#include "GraphicsObject_TextureLight.h"
#include "Model.h"
#include "Texture.h"
#include "ShaderObject.h"
#include "CameraMan.h"


GraphicsObject_TextureLight::GraphicsObject_TextureLight(Model *_pModel, ShaderObject *_pShaderObj, Texture::Name _name, Vect &LightColor, Vect &LightPos)
	: GraphicsObject(_pModel, _pShaderObj),
	textName(_name)
{
	this->pLightColor = new Vect(LightColor);
	this->pLightPos = new Vect(LightPos);
}

GraphicsObject_TextureLight::~GraphicsObject_TextureLight()
{
	delete this->pLightColor;
	delete this->pLightPos;
}

void GraphicsObject_TextureLight::SetState()
{
	glActiveTexture(GL_TEXTURE0);

	// Bind the texture
	GLuint textureID = Texture::FindID(this->textName);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}

void GraphicsObject_TextureLight::SetDataGPU()
{
	// use this shader
	this->pShaderObj->SetActive();

	// set the vao
	glBindVertexArray(this->GetModel()->vao);

	Camera *pCam = CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);

	Matrix world = this->GetWorld();
	Matrix view = pCam->getViewMatrix();
	Matrix proj = pCam->getProjMatrix();

	glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float *)&proj);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float *)&view);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float *)&world);

	Vect light = *this->pLightPos;
	Vect color = *this->pLightColor;
	glUniform3fv(this->pShaderObj->GetLocation("vLightPos"), 1, (float *)&light);
	glUniform4fv(this->pShaderObj->GetLocation("vColor"), 1, (float *)&color);
}

void GraphicsObject_TextureLight::Draw()
{
	//The draw triangles 
	//glDrawArrays(GL_TRIANGLES, 0, (3 * this->GetModel()->numTris));

	//The starting point of the IBO
	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
}

void GraphicsObject_TextureLight::RestoreState()
{
	// undo the states you corrupted
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}


// --- End of File -----------------------------------------------------------

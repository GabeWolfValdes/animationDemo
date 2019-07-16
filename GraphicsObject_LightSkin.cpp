#include "GraphicsObject_LightSkin.h"
#include <assert.h>

#include "Model.h"
#include "ShaderObject.h"
#include "CameraMan.h"

GraphicsObject_LightSkin::GraphicsObject_LightSkin(Model * model, ShaderObject * pShaderObj, Vect &LightColor, Vect &LightPos)
	: GraphicsObject(model, pShaderObj)
{
	assert(model);
	assert(pShaderObj);
	this->pLightColor = new Vect(LightColor);
	this->pLightPos = new Vect(LightPos);
}

void GraphicsObject_LightSkin::SetState()
{
	glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);

	glFrontFace(GL_CW);

}

void GraphicsObject_LightSkin::SetDataGPU()
{

	this->pShaderObj->SetActive();

	// set the vao
	glBindVertexArray(this->GetModel()->vao);

	// Set state
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glDisable(GL_CULL_FACE);


	//Matrix world(Matrix::SpecialType::Identity);


	Camera *pCam = CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);

	Matrix view = pCam->getViewMatrix();
	Matrix proj = pCam->getProjMatrix();


	glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float *)&proj);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float *)&view);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("bone_world_inv"), this->pAnimation->GetNumBones(), GL_FALSE, (float *)this->pAnimation->worldInvOutBuff->Map(SSBO::Access::READ_ONLY));

	Vect light = *this->pLightPos;
	Vect color = *this->pLightColor;
	glUniform3fv(this->pShaderObj->GetLocation("vLightPos"), 1, (float *)&light);
	glUniform4fv(this->pShaderObj->GetLocation("vColor"), 1, (float *)&color);

	this->pAnimation->worldInvOutBuff->Unmap();
}

void GraphicsObject_LightSkin::Draw()
{
	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
}

void GraphicsObject_LightSkin::RestoreState()
{
	//glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

}

void GraphicsObject_LightSkin::SetAnimation(BlendAnimation * _anim)
{
	this->pAnimation = _anim;
}

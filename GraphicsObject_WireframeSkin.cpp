#include "GraphicsObject_WireframeSkin.h"
#include <assert.h>

#include "Model.h"
#include "ShaderObject.h"
#include "CameraMan.h"

GraphicsObject_WireframeSkin::GraphicsObject_WireframeSkin(Model * model, ShaderObject * pShaderObj, Vect & wireColor)
	: GraphicsObject(model, pShaderObj)
{
	assert(model);
	assert(pShaderObj);
	this->pWireColor = new Vect(wireColor);
}

void GraphicsObject_WireframeSkin::SetState()
{
	glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);

	glFrontFace(GL_CW);

}

void GraphicsObject_WireframeSkin::SetDataGPU()
{

	this->pShaderObj->SetActive();

	// set the vao
	glBindVertexArray(this->GetModel()->vao);

	// Set state
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);


	//Matrix world(Matrix::SpecialType::Identity);


	Camera *pCam = CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);

	Matrix view = pCam->getViewMatrix();
	Matrix proj = pCam->getProjMatrix();


	glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float *)&proj);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float *)&view);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("bone_world_inv"), this->pAnimation->GetNumBones(), GL_FALSE, (float *)this->pAnimation->worldInvOutBuff->Map(SSBO::Access::READ_ONLY));

	glUniform4fv(this->pShaderObj->GetLocation("wire_color"), 1, (float *)this->pWireColor);
	this->pAnimation->worldInvOutBuff->Unmap();
}

void GraphicsObject_WireframeSkin::Draw()
{
	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
}

void GraphicsObject_WireframeSkin::RestoreState()
{
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GraphicsObject_WireframeSkin::SetAnimation(BlendAnimation * _anim)
{
	this->pAnimation = _anim;
}

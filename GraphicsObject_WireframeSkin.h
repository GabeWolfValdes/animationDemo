#ifndef GRAPHICS_OBJECT_WIREFRAME_SKIN_H
#define GRAPHICS_OBJECT_WIREFRAME_SKIN_H

#include "GraphicsObject.h"
//#include "Animation.h"
#include "BlendAnimations.h"
class GraphicsObject_WireframeSkin :public GraphicsObject
{
public:
	GraphicsObject_WireframeSkin(Model *model, ShaderObject *pShaderObj, Vect &wireColor);

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	void SetAnimation(BlendAnimation * _anim);

	//void SetAnimation(Animation * _anim);

public:
	// data:  place uniform instancing here
	Vect *pWireColor;
	BlendAnimation *pAnimation;

	GraphicsObject_WireframeSkin() = delete;

};
#endif

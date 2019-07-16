#ifndef GRAPHICS_OBJECT_LIGHT_SKIN_H
#define GRAPHICS_OBJECT_LIGHT_SKIN_H

#include "GraphicsObject.h"
//#include "Animation.h"
#include "BlendAnimations.h"
class GraphicsObject_LightSkin :public GraphicsObject
{
public:
	//GraphicsObject_LightSkin(Model *model, ShaderObject *pShaderObj, Vect &wireColor);

	GraphicsObject_LightSkin(Model * model, ShaderObject * pShaderObj, Vect & LightColor, Vect & LightPos);

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	void SetAnimation(BlendAnimation * _anim);

	//void SetAnimation(Animation * _anim);

public:
	// data:  place uniform instancing here
	Vect *pLightColor;
	Vect *pLightPos;
	BlendAnimation *pAnimation;

	GraphicsObject_LightSkin() = delete;

};
#endif

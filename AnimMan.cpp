#include "AnimMan.h"
#include "Animation.h"
#include "Skeleton.h"


//might need to change these
#include "ShaderObject.h"
#include "AnimationSkinModel.h"
#include "GameObjectAnim.h"
#include "GameObjectRigid.h"
#include "GameObjectMan.h"
#include "GameObjectSkin.h"

#include "GraphicsObject_LightSkin.h"


void AnimMan::Update()
{

	AnimMan* man = privGetInstance();

	
	//man->GetActiveTeddy()->Update(privGetInstance()->GetTeddySkeleton()->pFirstBone);
	man->blendTeddy->UpdateTime();
	man->blendTeddy->Update();
	
	man->blendHuman->UpdateTime();
	man->blendHuman->Update();

	//man->blendHuman->animationBegin->SetBoneWorldInv();
	//man->blendTeddy->animationBegin->SetBoneWorldInv();

}

BlendAnimation* AnimMan::GetBlend() {

	int count = 0;
	for (int i = 0; i < 10; i ++) {

		count += 1;

		if (i %2 ==0) {

		}


	}


	return privGetInstance()->blendHuman;
}

void AnimMan::TimeUpdate()
{
	privGetInstance()->activeTeddy->UpdateTime();
	privGetInstance()->blendHuman->UpdateTime();
	//privGetInstance()->activeHuman->UpdateTime();
}

Animation* AnimMan::GetActiveTeddy()
{
	return privGetInstance()->activeTeddy;
}

void AnimMan::SwitchTeddy()
{
	if (privGetInstance()->activeTeddy == privGetInstance()->teddy0)
	{
		privGetInstance()->activeTeddy = privGetInstance()->teddy1;
	}
	else
	{
		privGetInstance()->activeTeddy = privGetInstance()->teddy0;

	}

	privGetInstance()->ChangeSkinPointer(skinSwitch::SWITCH_TEDDY);

}

void AnimMan::SwitchHuman() {

	if (privGetInstance()->activeHuman == privGetInstance()->human0)
	{
		privGetInstance()->activeHuman = privGetInstance()->human1;
	}
	else
	{
		privGetInstance()->activeHuman = privGetInstance()->human0;
	}
	privGetInstance()->ChangeSkinPointer(skinSwitch::SWITCH_HUMAN);

}

void AnimMan::SetHumanSkin(char* fileName)
{
	UNUSED_VAR(fileName);
	AnimMan* manager = privGetInstance();

	ShaderObject *pShaderWire = new ShaderObject(ShaderObject::Name::SKIN, "skinLightRender");
	//ShaderObject *pShaderWire = new ShaderObject(ShaderObject::Name::COLOR_SINGLE, "colorSingleRender");
	AnimationSkinModel* pSkin = new AnimationSkinModel(fileName);
	//ModelMan::Add(pSkin);
	Vect Yellow(1.0f, 1.0f, 0.0f, 1.0f);
	Vect Pos(5.0f, 1.0f, 0.0f, 1.0f);
	GraphicsObject_LightSkin *pWireframe = new GraphicsObject_LightSkin(pSkin, pShaderWire, Yellow, Pos);
	//GraphicsObject_WireframeConstantColor *test = new GraphicsObject_WireframeConstantColor(pSkin, pShaderWire, Yellow);


	GameObjectSkin *skinAnim = new GameObjectSkin(pWireframe);
	//GameObjectSkin *skinAnim = new GameObjectSkin(test);
	//GameObjectRigid *skinAnim = new GameObjectRigid(test);
	//skinAnim->SetWorld(AnimMan::GetHumanSkeleton()->pFirstBone->GetWorld());
	//GameObject* rt = (GameObject*)manager->GetHumanSkeleton()->pFirstBone;

	skinAnim->SetName("HumanSkin");
	//skinAnim->SetWorld(rt->GetWorld());

	//GameObjectMan::Add(skinAnim, rt);
	//
	//GameObjectMan::Add(skinAnim, GameObjectMan::GetRoot());
	GameObjectMan::Add(skinAnim, GameObjectMan::ObjType::SKIN);

	pWireframe->SetAnimation(manager->blendHuman);
	manager->humanSkin = pWireframe;
	manager->ChangeSkinPointer(skinSwitch::SWITCH_HUMAN);
}


void AnimMan::SetTeddySkin(char* fileName)
{
	UNUSED_VAR(fileName);
	AnimMan* manager = privGetInstance();

	ShaderObject *pShaderWire = new ShaderObject(ShaderObject::Name::SKIN, "skinLightRender");
	//ShaderObject *pShaderWire = new ShaderObject(ShaderObject::Name::COLOR_SINGLE, "colorSingleRender");
	AnimationSkinModel* pSkin = new AnimationSkinModel(fileName);
	//ModelMan::Add(pSkin);
	Vect Yellow(1.0f, 1.0f, 0.0f, 1.0f);
	Vect Pos(5.0f, 1.0f, 0.0f, 1.0f);
	GraphicsObject_LightSkin *pWireframe = new GraphicsObject_LightSkin(pSkin, pShaderWire, Yellow, Pos);
	//GraphicsObject_WireframeConstantColor *test = new GraphicsObject_WireframeConstantColor(pSkin, pShaderWire, Yellow);


	GameObjectSkin *skinAnim = new GameObjectSkin(pWireframe);
	//GameObjectSkin *skinAnim = new GameObjectSkin(test);
	//GameObjectRigid *skinAnim = new GameObjectRigid(test);
	//skinAnim->SetWorld(AnimMan::GetHumanSkeleton()->pFirstBone->GetWorld());
	//GameObject* rt = (GameObject*)manager->GetHumanSkeleton()->pFirstBone;

	skinAnim->SetName("TeddySkin");
	//skinAnim->SetWorld(rt->GetWorld());

	//GameObjectMan::Add(skinAnim, rt);
	//
	//GameObjectMan::Add(skinAnim, GameObjectMan::GetRoot());
	GameObjectMan::Add(skinAnim, GameObjectMan::ObjType::SKIN);

	pWireframe->SetAnimation(manager->blendTeddy);
	manager->teddySkin = pWireframe;
	manager->ChangeSkinPointer(skinSwitch::SWITCH_TEDDY);
}


void AnimMan::StartBlending()
{

	AnimMan* manager = privGetInstance();
	//manager->blending = true;
	manager->blendHuman->StartBlend(0.5f* Time(Time::ONE_SECOND));
}

//
//void AnimMan::StopBlending()
//{
//	AnimMan* manager = AnimMan::privGetInstance();
//	manager->blending = false;
//}

FrameBucket * AnimMan::GetBlendHead()
{
	AnimMan * man = AnimMan::privGetInstance();
	return man->blendHuman->pHeadBlended;
}



void AnimMan::ChangeSkinPointer(skinSwitch s)
{

	AnimMan* manager = privGetInstance();

	if (s == skinSwitch::SWITCH_HUMAN) {
		manager->humanSkin->SetAnimation(manager->blendHuman);
	}
	else {
		manager->teddySkin->SetAnimation(manager->blendTeddy);
	}
	//add teddy skin here
}

GraphicsObject * AnimMan::GetGraphicObj()
{
	return privGetInstance()->humanSkin;
}


Skeleton * AnimMan::GetHumanSkeleton()
{
	return privGetInstance()->humanSkeleton;
}

Skeleton * AnimMan::GetTeddySkeleton()
{
	return privGetInstance()->teddySkeleton;
}
Animation * AnimMan::GetActiveHuman()
{
	return privGetInstance()->blendHuman->animationBegin;
}


void AnimMan::Initialize() {

	AnimMan* manager = privGetInstance();

	manager->teddySkeleton = new Skeleton("teddyheirarchy.spu", skelType::TEDDYSKEL);
	manager->humanSkeleton = new Skeleton("humanoid2heirarchy.spu", skelType::HUMANSKEL);

	//placeholders
	manager->teddy0 = new Animation("teddyAnim0.spu", animType::TEDDYANIM);
	manager->human0 = new Animation("humanoid2Anim3.spu", animType::HUMANANIM);

	manager->teddy1 = new Animation("teddyAnim1.spu", animType::TEDDYANIM);
	manager->human1 = new Animation("humanoid2Anim2.spu", animType::HUMANANIM);



	//this->teddy1 = new Animation("teddy_animation1.spu");
	manager->blendTeddy = new BlendAnimation(manager->teddy0, manager->teddy1, blendType::TEDDYBLEND);
	//manager->activeHuman = manager->human0;

	manager->blendHuman = new BlendAnimation(manager->human0, manager->human1, blendType::HUMANBLEND);

	manager->activeHuman = manager->blendHuman->animationBegin;
	manager->activeTeddy = manager->blendTeddy->animationBegin;


	manager->SetHumanSkin("humanoid2");

	manager->SetTeddySkin("teddy");

}

AnimMan::AnimMan()
{
	//this->teddySkeleton = new Skeleton("teddy_hierarchy.spu");
	//this->humanSkeleton = new Skeleton("human_hierarchy.spu");

	////this->teddy0 = new Animation("teddy_animation0.spu");
	////this->teddy1 = new Animation("teddy_animation1.spu");
	//this->activeTeddy = this->teddy0;


}

AnimMan * AnimMan::privGetInstance()
{
	static AnimMan am;
	return &am;
}
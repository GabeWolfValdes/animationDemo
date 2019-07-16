#ifndef ANIM_MAN_H
#define ANIM_MAN_H

#include "GraphicsObject_WireframeSkin.h"
#include "GraphicsObject_LightSkin.h"

#include "BlendAnimations.h"
class Animation;
class Skeleton;
//class Blendanimations;

class AnimMan
{

	enum skinSwitch {
		SWITCH_TEDDY,
		SWITCH_HUMAN
	};

public:
	static void Update();
	static void TimeUpdate();
	static Animation* GetActiveTeddy();
	static Skeleton * GetTeddySkeleton();
	static Animation* GetActiveHuman();
	static BlendAnimation* GetBlend();
	static Skeleton * GetHumanSkeleton();

	static void Initialize();

	static void SwitchTeddy();

	static void SwitchHuman();

	static void SetHumanSkin(char* fileName);
	static void SetTeddySkin(char* fileName);


	static void StartBlending();
	static void StopBlending();
	static FrameBucket* GetBlendHead();

	static void ChangeSkinPointer(skinSwitch s);
	static GraphicsObject* GetGraphicObj();

private:
	AnimMan();

	static AnimMan *privGetInstance();

	// data
	Skeleton* teddySkeleton;
	Skeleton* humanSkeleton;

	Animation* teddy0;
	Animation* teddy1;
	Animation* human0;
	Animation* human1;


	Animation* activeTeddy;
	Animation* activeHuman;

	GraphicsObject_LightSkin* humanSkin;
	GraphicsObject_LightSkin* teddySkin;

	BlendAnimation* blendHuman;
	BlendAnimation* blendTeddy;

	bool blending = false;
	bool pad[3];
};


#endif
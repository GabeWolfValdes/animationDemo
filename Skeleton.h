//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SKELETON_H
#define SKELETON_H

//#include "Timer.h"
////#include "GameObjectAnim.h"
//#include "Animation.h"
//#include "GameObjectMan.h"
//#include "PyramidModel.h"
//#include "GraphicsObject_TextureLight.h"
//#include "GameObjectAnim.h"
//#include "FrameBucket.h"
//#include "ShaderObject.h"
//#include "PCSTreeForwardIterator.h"
//#include "PCSTreeReverseIterator.h"
//#include "GameObjectRigid.h"
//#include "eat.h"
//#include "ChunkHeader.h"
//#include "Framework.h"

#define BONE_WIDTH 8.0f
class GameObjectAnim;
class Animation;
class PyramidModel;

#define NUM_TEDDY_BONES 20
#define NUM_HUMAN_BONES 80


enum skelType {
	TEDDYSKEL,
	HUMANSKEL
};


class Skeleton{
	public:
	
		Skeleton() = delete;
		Skeleton(const Skeleton&) = delete;
		Skeleton &operator=(Skeleton &) = delete;
		~Skeleton() = default;


		Skeleton(char* spuFile, skelType type);

		void SetAnimationHeirarchyFromFileT(char* spuFile);

		//in game loop it goes 
		void ProcessAnimation(Time tCurr);
		void SetAnimationHeirarchyFromFileH(char* spuFile);

		//then
		void Update(Time tCurr);
		//finally
		//SetAnimationPose(pFirstBone, tCurrent);

		void FindMaxTime(Time &tMax);

		GameObjectAnim* GetFirstBone();

private:
	void privUpdate(Time tCurr);


//data 
public:
	GameObjectAnim *pFirstBone;
	Animation* pCurrAnim;
	Skeleton* pNextSkeleton;
	Skeleton* pPrevSkeleton;
	PyramidModel *pPyramidModel = 0;

};

#endif
//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIMATION_H
#define ANIMATION_H

#include "Timer.h"
//#include "GameObjectAnim.h"
#include "FrameBucket.h"
#include "SSBO.h"
#define NUM_TEDDY_BONES 20
#define NUM_HUMAN_BONES 80

class GameObjectAnim;

enum animType {
	TEDDYANIM,
	HUMANANIM
};


class Animation {
public:

	Animation() = delete;
	Animation(const Animation&) = delete;
	Animation &operator=(Animation &) = delete;
	~Animation() = default;


	//Animation(char* animationSpuFile);


	void SetAnimationPose(GameObjectAnim *root, Time tCurr);

	void ProcessAnimation(Time tCurr);
	void SetAnimationDataFromFileH(char * spuFile);
	void SetAnimationDataFromFileT(char * spuFile);
	//void SetAnimationDataFromFileH();
	//void SetAnimationDataFromFileT();

	void FindMaxTime(Time &tMax);
	void setBonePose(GameObjectAnim *node);
	void SetSpeed(float speed);


	void Update(GameObjectAnim *firstBone);
	void UpdateTime();
	Animation(char * animationSpuFile, animType type);

	int GetNumBones();

	Matrix* GetWorldArray();
	Matrix* GetInvArray();
	void SetWorldArray(int index, Matrix& _m);

	void SetBoneWorldInv();
	Matrix* GetWorldInv();

private:
	void SetSkin(char *fileName);


	//data 
public:
	//GameObjectAnim *pFirstBone;

	//gabe new


	Time currTime;
	Time deltaTime;
	Time maxTime;

	Matrix *boneWorldArray;
	Matrix *boneInvArray;
	Matrix *boneWorldInv;

	FrameBucket *pHead = 0;
	Animation* pNextAnim;
	Animation* pPrevAnim;
	int numSkelBones = 0;

	SSBO* boneA;
	SSBO* boneB;
	SSBO* boneOut;
	SSBO* matrixResult;

	SSBO* heirarchyBuff;
	SSBO* worldInBuff;
	SSBO* worldOutBuff;
	SSBO* inverseBoneBuff;

	int* worldIndexTable;

	//int pad;
};
#endif
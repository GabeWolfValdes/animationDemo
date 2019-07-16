#ifndef BLEND_ANIMATION_H
#define BLEND_ANIMATION_H

#include "Timer.h"
//#include "GameObjectAnim.h"
#include "Animation.h"
#include "FrameBucket.h"
#include "SSBO.h"
#define NUM_TEDDY_BONES 20
#define NUM_HUMAN_BONES 80

class GameObjectAnim;


enum blendType {
	TEDDYBLEND,
	HUMANBLEND
};


class BlendAnimation {
public:

	BlendAnimation() = delete;
	BlendAnimation(const BlendAnimation&) = delete;
	BlendAnimation &operator=(BlendAnimation &) = delete;
	~BlendAnimation() = default;


	//Animation(char* animationSpuFile);


	//void SetAnimationPose(GameObjectAnim *root, Time tCurr);
	BlendAnimation(Animation* animA, Animation* animB, blendType _type);

	//void ProcessAnimation(Time tCurr);
	//void SetAnimationDataFromFileH(char * spuFile);
	//void SetAnimationDataFromFileT(char * spuFile);
	//void SetAnimationDataFromFileH();
	//void SetAnimationDataFromFileT();

	void FindMaxTime(Time &tMax);
	//void setBonePose(GameObjectAnim *node);
	void SetSpeed(float speed);


	//void Update(GameObjectAnim *firstBone);
	void Update();
	void UpdateTime();

	int GetNumBones();

	Matrix* GetWorldArray();
	Matrix* GetInvArray();
	Matrix * GetWorldInverse();
	void SetWorldArray(int index, Matrix& _m);

	//change bool to true
	void StartBlend(Time _maxBlendTime);

	//change bool to false, switch begin and end, change next time to 0 again 
	void StopBlend();

	//data 
public:
	//GameObjectAnim *pFirstBone;


	Time blendTotalTime;
	Time elapsedBlendTime;
	Time nextBlendTime;
	Time deltaTime;
	//gabe new
	Matrix *boneWorldArray;
	Matrix *boneInvArray;

	Matrix *boneWorldInverse;

	Animation* animationBegin;
	Animation* animationEnd;

	FrameBucket *pHeadBlended;
	//Animation* pNextAnim;
	//Animation* pPrevAnim;

	SSBO* boneA;
	SSBO* boneB;
	SSBO* boneOut;
	SSBO* matrixResult;

	SSBO* heirarchyBuff;
	SSBO* worldInBuff;
	SSBO* worldOutBuff;
	SSBO* worldInvOutBuff;

	//Time maxTime;
	int* worldIndexTable;
	float sTblend = 0;

	int numSkelBones = 0;

	bool isBlending = false;
	bool pad[3];
	//int pad;
};
#endif
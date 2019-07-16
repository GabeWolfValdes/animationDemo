#include "BlendAnimations.h"
#include "VectApp.h"
#include "QuatApp.h"
#include "eat.h"
#include "ChunkHeader.h"
#include "PCSTreeForwardIterator.h"
#include "GameObjectAnim.h"
#include "GameObjectMan.h"
#include "AnimMan.h"

#include "WorldIndicesTable.h"
#include "ShaderMan.h"

struct boneData {
	float tx;
	float ty;
	float tz;
	float qx;
	float qy;
	float qz;
	float sx;
	float sy;
	float sz;
};


BlendAnimation::BlendAnimation(Animation * animA, Animation * animB, blendType _type)
{
	this->boneA = new SSBO();
	this->boneB = new SSBO();
	this->boneOut = new SSBO();
	this->matrixResult = new SSBO();

	this->heirarchyBuff = new SSBO();
	this->worldInBuff = new SSBO();
	this->worldOutBuff = new SSBO();
	this->worldInvOutBuff = new SSBO();


	this->animationBegin = animA;
	this->animationEnd = animB;



	if (_type == blendType::HUMANBLEND) {
		this->numSkelBones = NUM_HUMAN_BONES;
		this->boneWorldArray = new Matrix[NUM_HUMAN_BONES];
		this->boneInvArray = new Matrix[NUM_HUMAN_BONES];
		this->boneWorldInverse = new Matrix[NUM_HUMAN_BONES];
		this->worldIndexTable = WorldIndexTable::humanoid->table;
		this->heirarchyBuff->Set((unsigned int)17 * 80, sizeof(int), this->worldIndexTable);
	}
	else {
		this->numSkelBones = NUM_TEDDY_BONES;
		this->boneWorldArray = new Matrix[NUM_TEDDY_BONES];
		this->boneInvArray = new Matrix[NUM_TEDDY_BONES];
		this->boneWorldInverse = new Matrix[NUM_TEDDY_BONES];

		//this->SetAnimationDataFromFileT(animationSpuFile);
		this->worldIndexTable = WorldIndexTable::teddy->table;
		this->heirarchyBuff->Set((unsigned int)9 * 20, sizeof(int), this->worldIndexTable);
	}

	this->boneOut->Set((unsigned int)this->numSkelBones + 1, sizeof(Bone), 0);
	this->matrixResult->Set((unsigned int)this->numSkelBones, sizeof(Matrix), 0);
	this->worldOutBuff->Set((unsigned int)this->numSkelBones, sizeof(Matrix), 0);
	this->worldInvOutBuff->Set((unsigned int)this->numSkelBones, sizeof(Matrix), 0);

	//this->currTime = Time(Time::ZERO);
	this->deltaTime = 0.3f* Time(Time::NTSC_30_FRAME);
	//this->FindMaxTime(this->maxTime);

	this->pHeadBlended = animA->pHead;
}

void BlendAnimation::FindMaxTime(Time & tMax)
{
	//may need to change this
	FrameBucket *pTmp = this->animationBegin->pHead->next;

	while (pTmp->next != 0)
	{
		pTmp = pTmp->next;
	}

	tMax = pTmp->KeyTime;
}

void BlendAnimation::SetSpeed(float speed)
{
	this->deltaTime = speed * Time(Time::NTSC_30_FRAME);
}

void BlendAnimation::Update()
{
	//proccess each animation individually first to get the local transforms ok if end results in 0
	this->animationBegin->ProcessAnimation(animationBegin->currTime);
	this->animationEnd->ProcessAnimation(animationEnd->currTime);

	// find correct ts
	float tS = 0;
	if (this->isBlending) {
		tS = (this->elapsedBlendTime) / (this->blendTotalTime);
	}
	
	//use the same shader from the local bone transforms used on each anim and use that to blend the resultant bones
	ShaderObject* pComputeShader = ShaderMan::Find(ShaderObject::Name::WORLD_MATRIX_COMPUTE);
	pComputeShader->SetActive();
	
	//set the correct ts based on the blend time given in
	glUniform1f(pComputeShader->GetLocation("tS"), tS);

	//bind the result of the local bone from both anims
	this->animationBegin->boneOut->Bind(0);
	this->animationEnd->boneOut->Bind(1);

	//bind bone result and matrix result buffers for later use
	this->boneOut->Bind(2);
	this->matrixResult->Bind(3);

	//run the shader
	pComputeShader->Dispatch((unsigned int)this->numSkelBones+1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//get the bone inverse world compute shader
	pComputeShader = ShaderMan::Find(ShaderObject::Name::BONE_WORLD_COMPUTE_TEST);
	pComputeShader->SetActive();


	//bind result from the blend
	this->boneOut->Bind(3);
	
	//set the depth depending on model
	if (this->numSkelBones > 20) {
		glUniform1i(pComputeShader->GetLocation("heirarchyDepth"), WorldIndexTable::humanoid->tableWidth);
	}
	else {
		glUniform1i(pComputeShader->GetLocation("heirarchyDepth"), WorldIndexTable::teddy->tableWidth);
	}

	glUniform1i(pComputeShader->GetLocation("boneCount"), this->numSkelBones);

	//bind the heirarchy and inverse stuff
	this->animationBegin->heirarchyBuff->Bind(2);
	this->animationBegin->inverseBoneBuff->Bind(1);

	//bind world inverse matrix to result buff
	this->worldInvOutBuff->Bind(4);

	//run the shader 
	pComputeShader->Dispatch((unsigned int)this->numSkelBones, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

}

void BlendAnimation::UpdateTime()
{
	this->animationBegin->UpdateTime();


	if (this->isBlending) {
		this->animationEnd->UpdateTime();
		this->nextBlendTime += this->deltaTime;
		this->elapsedBlendTime += this->deltaTime;

		if (this->elapsedBlendTime >= this->blendTotalTime) {
			this->StopBlend();
		}

	}

}

int BlendAnimation::GetNumBones()
{
	return this->numSkelBones;
}

Matrix * BlendAnimation::GetWorldArray()
{
	return this->boneWorldArray;
}

Matrix * BlendAnimation::GetInvArray()
{
	return this->boneInvArray;
}

Matrix * BlendAnimation::GetWorldInverse() {
	return this->boneWorldInverse;
}

void BlendAnimation::SetWorldArray(int index, Matrix & _m)
{
	this->boneWorldArray[index].set(_m);
}


void BlendAnimation::StartBlend(Time _maxBlendTime)
{
	this->isBlending = true;
	this->blendTotalTime = _maxBlendTime;
	this->nextBlendTime = Time(Time::ZERO);

}

void BlendAnimation::StopBlend()
{
	this->isBlending = false;
	this->blendTotalTime = Time(Time::ZERO);
	this->nextBlendTime = Time(Time::ZERO);
	this->elapsedBlendTime = Time(Time::ZERO);

	Animation* tmp = this->animationBegin;
	this->animationBegin = this->animationEnd;
	this->animationEnd = tmp;

	this->animationEnd->currTime = Time(Time::ZERO);
	this->pHeadBlended = animationBegin->pHead;
}

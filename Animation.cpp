#include "Framework.h"
#include "Animation.h"
#include "VectApp.h"
#include "QuatApp.h"
#include "FrameBucket.h"
#include "eat.h"
#include "ChunkHeader.h"
#include "PCSTreeForwardIterator.h"
#include "GameObjectAnim.h"
#include "GameObjectMan.h"

#include "WorldIndicesTable.h"
#include "ShaderMan.h"
//#include "SSBO.h"

//#define UNUSED_VAR(x) ((void *)x)
#define BONE_WIDTH 10.0f


//class GameObject;
//class GameObjecAnim;

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



void Animation::FindMaxTime(Time &tMax)
{
	FrameBucket *pTmp = this->pHead->next;

	while (pTmp->next != 0)
	{
		pTmp = pTmp->next;
	}

	tMax = pTmp->KeyTime;
}

void Animation::setBonePose(GameObjectAnim * node)
{
	GameObjectAnim *childNode = (GameObjectAnim *)node;
	GameObjectAnim *parentNode = (GameObjectAnim *)node->GetParent();

	if (node->GetParent() == 0)
		return;


	//add compute shader here
	//add compute shader here
	//add compute shader here

	//make sure getting same values as the previous way of life



	if (parentNode != 0 && childNode != 0)
	{
		// starting point
		Vect start(0.0f, 0.0f, 0.0f);

		//  At this point, Find the two bones initial positions in world space
		//  Now get the length and directions

		Vect ptA = start * *parentNode->GetWorld();
		Vect ptB = start * *childNode->GetWorld();

		// direction between the anchor points of the respective bones
		Vect dir = (ptA - ptB);

		// length of the bone 0
		float mag = dir.mag();

		Matrix S(Matrix::Scale, BONE_WIDTH, BONE_WIDTH, mag);
		Quat Q(Quat::RotOrient, dir.getNorm(), Vect(0.0f, 1.0f, 0.0f));
		Matrix T(Matrix::Trans, ptB);

		Matrix BoneOrient = S * Q * T;

		//GameObjectAnim * temp = node;
		//temp->SetBoneOrientation(WorldIndexTable::humanoid->GetBoneWorld(Matrix(Matrix::SpecialType::Identity), this->pHead, node->indexBoneArray));

		childNode->SetBoneOrientation(BoneOrient);
	}





}

void Animation::Update(GameObjectAnim * firstBone)
{
	this->ProcessAnimation(this->currTime);
	
	//only matters for the skeleton to render
	//this->SetAnimationPose(firstBone,this->currTime);
	UNUSED_VAR(firstBone);

}

void Animation::UpdateTime()
{
	this->currTime += this->deltaTime;


	// protection for time values for looping
	if (this->currTime < Time(Time::ZERO))
	{
		this->currTime = this->maxTime;
	}
	else if (this->currTime > this->maxTime)
	{
		this->currTime = Time(Time::ZERO);
	}
	else
	{
		// do nothing
	}
}

Animation::Animation(char * animationSpuFile, animType type)
{
	//this->boneA = new SSBO();
	//this->boneB = new SSBO();
	this->boneOut = new SSBO();
	this->matrixResult = new SSBO();

	this->heirarchyBuff = new SSBO();
	this->worldInBuff = new SSBO();
	this->worldOutBuff = new SSBO();
	this->inverseBoneBuff = new SSBO();

	if (type == animType::HUMANANIM) {
		this->numSkelBones = NUM_HUMAN_BONES;
		this->boneWorldArray = new Matrix[NUM_HUMAN_BONES];
		//this->boneWorldArray[NUM_HUMAN_BONES] = Matrix(Matrix::SpecialType::Identity);
		this->boneInvArray = new Matrix[NUM_HUMAN_BONES];
		this->boneWorldInv = new Matrix[NUM_HUMAN_BONES];
		this->SetAnimationDataFromFileH(animationSpuFile);
		this->worldIndexTable = WorldIndexTable::humanoid->table;
	}
	else {
		this->numSkelBones = NUM_TEDDY_BONES;
		this->boneWorldArray = new Matrix[NUM_TEDDY_BONES];
		this->boneInvArray = new Matrix[NUM_TEDDY_BONES];
		this->boneWorldInv = new Matrix[NUM_TEDDY_BONES];
		this->SetAnimationDataFromFileT(animationSpuFile);
		this->worldIndexTable = WorldIndexTable::teddy->table;
		this->heirarchyBuff->Set((unsigned int)9 * 20, sizeof(int), this->worldIndexTable);
		//this->heirarchyBuff->Bind(2);
	}

	//set ssbos
	this->heirarchyBuff->Set((unsigned int)17 * 80, sizeof(int), this->worldIndexTable);
	this->inverseBoneBuff->Set((unsigned int)this->numSkelBones, sizeof(Matrix), this->boneInvArray);
	this->worldOutBuff->Set((unsigned int)this->numSkelBones, sizeof(Matrix), 0);
	this->boneOut->Set((unsigned int)this->numSkelBones, sizeof(Bone), 0);
	this->matrixResult->Set((unsigned int)this->numSkelBones, sizeof(Matrix), 0);

	//this->worldInBuff->Set();

	this->currTime = Time(Time::ZERO);
	this->deltaTime = 0.3f* Time(Time::NTSC_30_FRAME);
	this->FindMaxTime(this->maxTime);


	
}

void Animation::SetSpeed(float speed) {
	this->deltaTime = speed * Time(Time::NTSC_30_FRAME);
}



int Animation::GetNumBones()
{
	return this->numSkelBones;
}

Matrix * Animation::GetWorldArray()
{
	return this->boneWorldArray;
}

Matrix * Animation::GetInvArray()
{
	return this->boneInvArray;
}

void Animation::SetWorldArray(int index, Matrix & _m)
{

	//temp fix for gpu
	this->boneWorldArray[index].set( _m);

	UNUSED_VAR(index);
	UNUSED_VAR(_m);

	//do compute shader for setting world in here


}

void Animation::SetBoneWorldInv()
{
	//add compute shader
	ShaderObject* pComputeShader = ShaderMan::Find(ShaderObject::Name::BONE_WORLD_COMPUTE_TEST);
	pComputeShader->SetActive();

	this->pHead->frameSSBO->Bind(3);

	if (this->numSkelBones > 20) {
		glUniform1i(pComputeShader->GetLocation("heirarchyDepth"), WorldIndexTable::humanoid->tableWidth);
	}
	else {
		glUniform1i(pComputeShader->GetLocation("heirarchyDepth"), WorldIndexTable::teddy->tableWidth);
	}
	glUniform1i(pComputeShader->GetLocation("boneCount"), this->numSkelBones);


	//bind heirarchy table
	this->heirarchyBuff->Bind(2);

	//bind inverse bone table
	this->inverseBoneBuff->Bind(1);

	//bind matrix world output from shader 
	this->worldOutBuff->Bind(4);

	//send out the shader patrols
	pComputeShader->Dispatch((unsigned int)this->numSkelBones, 1, 1);
	this->worldOutBuff->Map(SSBO::Access::READ_WRITE);

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	this->worldOutBuff->Unmap();
}

Matrix * Animation::GetWorldInv()
{
	return this->boneWorldInv;
}


void Animation::SetAnimationPose(GameObjectAnim * root, Time tCurr)
{

	PCSTreeForwardIterator pIter(root);
	PCSNode *pNode = pIter.First();
	GameObjectAnim *pGameObj = 0;
	int i = 0;
	// walks the anim node does the pose for everything that
	while (pNode != 0 && i < this->numSkelBones)
	{
		assert(pNode);
		// Update the game object
		pGameObj = (GameObjectAnim *)pNode;
		setBonePose(pGameObj);

		pNode = pIter.Next();
		i++;
	}
}

void Animation::ProcessAnimation(Time tCurr)
{

	FrameBucket *pTmp = this->pHead->next;
	//pTmp = this->pHead->next;

	// Find which key frames
	while (tCurr >= pTmp->KeyTime  && pTmp->next != 0)
	{
		pTmp = pTmp->next;
	}

	//get frames to use
	FrameBucket *frameA = pTmp->prev;
	FrameBucket *frameB = pTmp;

	// find the "S" of the time
	float tS = (tCurr - frameA->KeyTime) / (frameB->KeyTime - frameA->KeyTime);


	//get compute shader
	ShaderObject* pComputeShader = ShaderMan::Find(ShaderObject::Name::WORLD_MATRIX_COMPUTE);
	pComputeShader->SetActive();

	glUniform1f(pComputeShader->GetLocation("tS"), tS);
	 
	// bone arrays to send in, get from frambuckets owned ssbos
	frameA->frameSSBO->Bind(0);
	frameB->frameSSBO->Bind(1);

	//bind matrix and bone results
	this->boneOut->Bind(2);
	this->matrixResult->Bind(3);

	pComputeShader->Dispatch((unsigned int)this->numSkelBones, 1, 1);

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void Animation::SetAnimationDataFromFileH(char * spuFile) {

	//read in from spu
	bool status;
	unsigned char *bBuff;
	unsigned int chunkSize;


	char spuName[50];
	strcpy_s(spuName, spuFile);
	//strcat_s(spuName, ".spu");

	char chunkName[50];
	strcpy_s(chunkName, "humanoid2Anim");

	// Find chunk and return it
	status = eat(spuName, ChunkType::ANIM_TYPE, chunkName, bBuff, chunkSize);
	assert(status);



	unsigned int numFrames = chunkSize / (sizeof(boneData) * NUM_HUMAN_BONES);
	unsigned int numBones = chunkSize / sizeof(boneData);
	boneData *  bones = new boneData[numBones];

	for (unsigned int i = 0; i < numBones; i++) {
		bones[i] = *(boneData*)bBuff;
		bBuff += sizeof(boneData);
	}


	//PA1
	this->pHead = new FrameBucket(Time(Time::ZERO), NUM_HUMAN_BONES + 1);


	this->pHead->pBone[NUM_HUMAN_BONES].T = Vect(0.0f, 0.0f, 0.0f,1.0f);
	this->pHead->pBone[NUM_HUMAN_BONES].Q = Quat(Quat::Identity);
	this->pHead->pBone[NUM_HUMAN_BONES].S = Vect(1.0f, 1.0f, 1.0f);
	this->pHead->SetSSBO((unsigned int)this->numSkelBones + 1, sizeof(Bone), pHead->pBone);

	//PA1
	FrameBucket *currFrame;
	FrameBucket *prevFrame = this->pHead;
	unsigned int boneIndex;

	for (int i = 0; i < (int)numFrames; i++) { //(int)numFrames; i++ ) {

		currFrame = new FrameBucket(i * Time(Time::NTSC_30_FRAME), NUM_HUMAN_BONES+1);
		currFrame->next = 0;
		currFrame->prev = prevFrame;
		prevFrame->next = currFrame;

		currFrame->pBone[NUM_HUMAN_BONES].T = Vect(0.0f, 0.0f, 0.0f);
		currFrame->pBone[NUM_HUMAN_BONES].Q = Quat(Quat::Rot_XYZ, 0.0f, 0.0f, 0.0f);
		currFrame->pBone[NUM_HUMAN_BONES].S = Vect(1.0f, 1.0f, 1.0f);

		for (unsigned int j = 0; j < NUM_HUMAN_BONES; j++) {
			boneIndex = j + (i * NUM_HUMAN_BONES);
			if (j == 0) {
				currFrame->pBone[0].T = Vect(0.0f, 0.0f, 0.0f);
				currFrame->pBone[0].Q = Quat(Quat::Rot_XYZ, 0.0f, 0.0f, 0.0f);
				currFrame->pBone[0].S = Vect(1.0f, 1.0f, 1.0f);
			}

			currFrame->pBone[j].T = Vect(bones[boneIndex].tx, bones[boneIndex].ty, bones[boneIndex].tz);
			currFrame->pBone[j].Q = Quat(Quat::Rot_XYZ, bones[boneIndex].qx, bones[boneIndex].qy, bones[boneIndex].qz);
			currFrame->pBone[j].S = Vect(bones[boneIndex].sx, bones[boneIndex].sy, bones[boneIndex].sz);
		}

		//set and bind SSBO's here
		currFrame->SetSSBO((unsigned int)this->numSkelBones + 1, sizeof(Bone), currFrame->pBone);


		prevFrame = currFrame;

	}


	//
	unsigned char* invBuff;
	char spuInv[50];
	//strcpy_s(spuInv, spuFile);
	strcpy_s(spuInv, "humanoid2skinData.spu");

	char chunkInv[50];
	strcpy_s(chunkInv, "humanoid2boneInv");

	// Find chunk and return it
	status = eat(spuInv, ChunkType::SKIN_TYPE, chunkInv, invBuff, chunkSize);
	assert(status);

	//unsigned int dataSize = chunkSize / (sizeof(Matrix) * NUM_HUMAN_BONES);
	unsigned int numMatrices = chunkSize / sizeof(Matrix);
	this->boneInvArray = new Matrix[numMatrices];

	for (unsigned int i = 0; i < numMatrices; i++) {
		this->boneInvArray[i] = *(Matrix*)invBuff;
		invBuff += sizeof(Matrix);
	}

	//this->SetSkin("humanoid2");
}

void Animation::SetAnimationDataFromFileT(char * spuFile) {

	//read in from spu
	bool status;
	unsigned char *bBuff;
	unsigned int chunkSize;


	char spuName[50];
	strcpy_s(spuName, spuFile);
	//strcat_s(spuName, ".spu");

	char chunkName[50];
	strcpy_s(chunkName, "teddyAnim");

	// Find chunk and return it
	status = eat(spuName, ChunkType::ANIM_TYPE, chunkName, bBuff, chunkSize);
	assert(status);



	unsigned int numFrames = chunkSize / (sizeof(boneData) * NUM_TEDDY_BONES);
	unsigned int numBones = chunkSize / sizeof(boneData);
	boneData *  bones = new boneData[numBones];

	for (unsigned int i = 0; i < numBones; i++) {
		bones[i] = *(boneData*)bBuff;
		bBuff += sizeof(boneData);
	}


	//PA1
	this->pHead = new FrameBucket(Time(Time::ZERO), NUM_TEDDY_BONES +1);
	//this->pHead = new Frame_Bucket();
	this->pHead->prev = 0;
	this->pHead->next = 0;


	this->pHead->pBone[NUM_TEDDY_BONES].T = Vect(0.0f, 0.0f, 0.0f);
	this->pHead->pBone[NUM_TEDDY_BONES].Q = Quat(Quat::Rot_XYZ, 0.0f, 0.0f, 0.0f);
	this->pHead->pBone[NUM_TEDDY_BONES].S = Vect(1.0f, 1.0f, 1.0f);


	this->pHead->SetSSBO((unsigned int)this->numSkelBones + 1, sizeof(Bone), pHead->pBone);

	//this->pHead->KeyTime = Time(Time::ZERO);
	//this->pHead->pBone = new Bone[NUM_TEDDY_BONES];

	//PA1
	FrameBucket *currFrame;// = new Frame_Bucket();
	//this->pHead->next = currFrame;
	FrameBucket *prevFrame = this->pHead;
	//Bone* tmpBone;
	unsigned int boneIndex;

	for (int i = 0; i < (int)numFrames; i++) { //(int)numFrames; i++ ) {

		//PA1
		currFrame = new FrameBucket(i * Time(Time::NTSC_30_FRAME), NUM_TEDDY_BONES+1);
		currFrame->next = 0;
		currFrame->prev = prevFrame;
		prevFrame->next = currFrame;


		currFrame->pBone[NUM_TEDDY_BONES].T = Vect(0.0f, 0.0f, 0.0f);
		currFrame->pBone[NUM_TEDDY_BONES].Q = Quat(Quat::Rot_XYZ, 0.0f, 0.0f, 0.0f);
		currFrame->pBone[NUM_TEDDY_BONES].S = Vect(1.0f, 1.0f, 1.0f);

		//currFrame->KeyTime = i * Time(Time::NTSC_30_FRAME);
		//currFrame->pBone = new Bone[NUM_TEDDY_BONES];

		for (unsigned int j = 0; j < NUM_TEDDY_BONES; j++) {
			boneIndex = j + (i * NUM_TEDDY_BONES);
			if (j == 0) {
				currFrame->pBone[0].T = Vect(bones[boneIndex].tx, bones[boneIndex].ty, bones[boneIndex].tz);
				currFrame->pBone[0].Q = Quat(Quat::Rot_XYZ, bones[boneIndex].qx, bones[boneIndex].qy, bones[boneIndex].qz);
				currFrame->pBone[0].S = Vect(1.0f, 1.0f, 1.0f);
			}

			else if (j == 1) {
				currFrame->pBone[1].T = Vect(0.0f, 0.0f, 0.0f);
				currFrame->pBone[1].Q = Quat(Quat::Rot_XYZ, 0.0f, 0.0f, 0.0f);
				currFrame->pBone[1].S = Vect(1.0f, 1.0f, 1.0f);
			}


			else {
				currFrame->pBone[j].T = Vect(bones[boneIndex].tx, bones[boneIndex].ty, bones[boneIndex].tz);
				currFrame->pBone[j].Q = Quat(Quat::Rot_XYZ, bones[boneIndex].qx, bones[boneIndex].qy, bones[boneIndex].qz);
				currFrame->pBone[j].S = Vect(bones[boneIndex].sx, bones[boneIndex].sy, bones[boneIndex].sz);
			}


		}
		currFrame->SetSSBO((unsigned int)this->numSkelBones + 1, sizeof(Bone), currFrame->pBone);

		prevFrame = currFrame;

	}

	unsigned char* invBuff;
	char spuInv[50];
	//strcpy_s(spuInv, spuFile);
	strcpy_s(spuInv, "teddyskinData.spu");

	char chunkInv[50];
	strcpy_s(chunkInv, "teddyboneInv");

	// Find chunk and return it
	status = eat(spuInv, ChunkType::SKIN_TYPE, chunkInv, invBuff, chunkSize);
	assert(status);

	//unsigned int dataSize = chunkSize / (sizeof(Matrix) * NUM_HUMAN_BONES);
	unsigned int numMatrices = chunkSize / sizeof(Matrix);
	this->boneInvArray = new Matrix[numMatrices];

	for (unsigned int i = 0; i < numMatrices; i++) {
		this->boneInvArray[i] = *(Matrix*)invBuff;
		invBuff += sizeof(Matrix);
	}

}
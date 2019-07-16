#include "Framework.h"
#include "FrameBucket.h"

// operators
FrameBucket::FrameBucket(Time time, unsigned int numBones)
{
	this->next = nullptr;
	this->prev = nullptr;
	KeyTime = time;

	this->pBone = new Bone[numBones];
	assert(this->pBone);

	//this->pad[0] = 0;
	//this->pad[1] = 0;
	//this->pad[2] = 0;
	//this->pad[3] = 0;
	this->frameSSBO = new SSBO();
}

FrameBucket::~FrameBucket()
{
	// delete all of the buckets
	delete[] this->pBone;

}

void FrameBucket::SetSSBO(unsigned int elementCount, unsigned int size, void *inData) {
	this->frameSSBO->Set(elementCount,size, inData);
}
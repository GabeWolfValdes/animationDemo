#ifndef FRAME_BUCKET
#define FRAME_BUCKET

#include "Timer.h"
#include "Vect.h"
#include "Quat.h"
#include "SSBO.h"

struct Bone : public Align16
{
	Vect  T;
	Quat  Q;
	Vect  S;
};

struct FrameBucket
{
public:
	// operators
	FrameBucket(Time t, unsigned int numBones);
	FrameBucket() = delete;
	~FrameBucket();

	void SetSSBO(unsigned int elementCount, unsigned int size, void *inData);

	// data:
	FrameBucket *next;
	FrameBucket *prev;
	Time		  KeyTime;
	Bone		  *pBone;
	SSBO*		 frameSSBO;
	//char		  pad[4];


};


#endif
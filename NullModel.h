//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef NULL_MODEL_H
#define NULL_MODEL_H

#include "Model.h"

class NullModel : public Model
{
public:
	NullModel(const char * const modelFileName);
	~NullModel();

	// tells the compiler do not create or allow it to be used, c++11
	NullModel(const NullModel &) = delete;
	NullModel& operator=(const NullModel & other) = delete;

private:
	virtual void privCreateVAO(const char * const modelFileName) override ;


};

#endif

//--- End of File ------------------------------------------------------------

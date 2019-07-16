#ifndef PYRAMID_MODEL_H
#define PYRAMID_MODEL_H

#include "Model.h"

class PyramidModel : public Model
{
public:
	PyramidModel( const char * const modelFileName,bool multPi );
	virtual ~PyramidModel();

	// tells the compiler do not create or allow it to be used, c++11
	PyramidModel(const PyramidModel &) = delete;
	PyramidModel& operator=(const PyramidModel & other) = delete;

	//This is all some janky PA1 stuff
	void setPiMult2(bool on);

private:
	void privCreateVAO( const char * const modelFileName  ) override;

	//This is all some janky PA1 stuff
	bool piMult2 =true;
	bool pad[3];
};

#endif
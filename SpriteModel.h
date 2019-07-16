#ifndef SPRITE_MODEL_H
#define SPRITE_MODEL_H

#include "Model.h"

class SpriteModel : public Model
{
public:
	SpriteModel(const char* const modelFileName);
	~SpriteModel();

	SpriteModel(const SpriteModel &) = delete;
	SpriteModel& operator = (const SpriteModel&) = delete;

private:
	void privCreateVAO(const char *const modelFileName);

};



#endif
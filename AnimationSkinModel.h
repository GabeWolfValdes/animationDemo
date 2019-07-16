#ifndef HSKIN_MODEL_H
#define HSKIN_MODEL_H

#include "Model.h"
#include "Texture.h"

#include "sb7.h"

#include "MathEngine.h"
#include "BoundingSphere.h"

class AnimationSkinModel : public Model
{
public:
	AnimationSkinModel() = delete;
	AnimationSkinModel(const AnimationSkinModel &) = delete;
	AnimationSkinModel &operator=(const AnimationSkinModel &) = delete;
	//void CreateWriteData();
	AnimationSkinModel(const char * const modelFileName);
	virtual ~AnimationSkinModel();

	////void CreateVAO(const char * const modelFileName) override;
	//TextureName getTexEnum();
	//char* getTexName();

private:
	virtual void privCreateVAO(const char * const modelFileName);
	char* textureName;
	//TextureName textureEnum;
	//void ReadFromFile(const char * const modelFileName);
	int numBoneWeights;
	int numBoneIndices;

	//GLuint vao;
	//GLuint vbo[4];

	// Reference sphere... before any transformations are applied
	//Sphere *pRefSphere;
};

#endif
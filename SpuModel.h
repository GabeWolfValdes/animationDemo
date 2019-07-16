#ifndef SPU_MODEL_H
#define SPU_MODEL_H

#include "Model.h"
#include "Texture.h"

class SpuModel : public Model
{
public:
	SpuModel() = delete;
	SpuModel(const SpuModel &) = delete;
	SpuModel &operator=(const SpuModel &) = delete;
	//void CreateWriteData();
	SpuModel(const char * const modelFileName);
	~SpuModel();

	//void CreateVAO(const char * const modelFileName) override;
	Texture::Name getTexEnum();
	char* getTexName();

private:
	virtual void privCreateVAO(const char * const modelFileName) override;
	char* textureName;
	Texture::Name textureEnum;
	//void ReadFromFile(const char * const modelFileName);

};

#endif
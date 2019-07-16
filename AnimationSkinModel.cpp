#include "AnimationSkinModel.h"

#include <assert.h>
#include <sb7.h>

#include "ModelFileHeader.h"
#include "File.h"
#include "eat.h"


#pragma warning(push)
#pragma warning(disable: 5045)
//should probably find alternative
//#pragma warning(push)
//#pragma warning(disable: 4774)
//#include <string>

struct Vert_xyzuvn
{
	float x;
	float y;
	float z;
	float u;
	float v;
	float nx;
	float ny;
	float nz;
};

struct Tri_index
{
	unsigned int v0;
	unsigned int v1;
	unsigned int v2;
};


struct BoneWeight
{
	float A_weight;
	float B_weight;
	float C_weight;
	float D_weight;
};

struct BoneIndex
{
	int index_A;
	int index_B;
	int index_C;
	int index_D;
};



#define WRITE_TO_FILE 1

AnimationSkinModel::AnimationSkinModel(const char * const modelFileName)
	:Model()
{
	this->privCreateVAO(modelFileName);
}

AnimationSkinModel::~AnimationSkinModel()
{
}

void AnimationSkinModel::privCreateVAO(const char * const modelFileName)
{
	//write to file 
	//see if you can try and move this to a seperate function

	char azul[50];
	strcpy_s(azul, modelFileName);
	strcat_s(azul, ".azul");

#if WRITE_TO_FILE

	bool status;
	unsigned char *vertBuff;
	unsigned char * triBuff;
	//unsigned char* uvBuff;
	//unsigned char* normBuff;
	unsigned char * tgaBuff;
	unsigned char* weightBuff;
	unsigned char* indexBuff;


	unsigned int chunkSize;


	char spuName[50];
	strcpy_s(spuName, modelFileName);
	strcat_s(spuName, "skinData.spu");

	char vboName[50];
	strcpy_s(vboName, modelFileName);
	strcat_s(vboName, "skinVert");



	// Find chunk and return it
	status = eat(spuName, ChunkType::VERTS_TYPE, vboName, vertBuff, chunkSize);
	assert(status);
	//status = eat("frigate.spu", ChunkType::UV_TYPE, "space_frigate", uvBuff, chunkSize);
	//assert(status);	
	//status = eat("frigate.spu", ChunkType::NORMS_TYPE, "space_frigate", normBuff, chunkSize);
	//assert(status);



	 // use eat here

	unsigned int tmpvertNum = chunkSize / sizeof(Vert_xyzuvn);
	this->numVerts = (int)tmpvertNum;
	Vert_xyzuvn *  spuVerts = new Vert_xyzuvn[(unsigned int)this->numVerts];

	for (unsigned int i = 0; i < tmpvertNum; i++) {
		spuVerts[i] = *(Vert_xyzuvn*)vertBuff;
		vertBuff += sizeof(Vert_xyzuvn);
	}


	char triName[50];
	strcpy_s(triName, modelFileName);
	strcat_s(triName, "skinTri");

	status = eat(spuName, ChunkType::VERTS_TYPE, triName, triBuff, chunkSize);
	assert(status);
	unsigned int triNumTmp = chunkSize / sizeof(Tri_index);
	this->numTris = (int)triNumTmp; // use eat here
	//unsigned int triTmp = (unsigned int)this->numTris;
	Tri_index * triList = new Tri_index[triNumTmp];


	for (unsigned int j = 0; j < triNumTmp; j++) {
		triList[j] = *(Tri_index*)triBuff;
		triBuff += sizeof(Tri_index);
	}

	// find tga in spu file and write to a tga if it exists
	File::Handle fh;
	File::Error error;



	//error = File::Open(fh,tgaName,File::Mode::READ_WRITE);
	//assert(error == File::Error::SUCCESS);


	//if theres a texture model then read it to tga
	int texCount = 0;
	//std::string texName = modelFileName;
	//texName += "_tga0";

	//std::string tmpTex = texName;
	//char c = texCount;
	//tmpTex += c;
	char texName[50];
	strcpy_s(texName, modelFileName);
	strcat_s(texName, "_tga0");
	//char texCntstr[3] = { '0' + texCount, '\0' };
	//strcat_s(texName, texCntstr);
	//status = eat(spuName, ChunkType::TEXTURE_TYPE, texName, tgaBuff, chunkSize);



	//error = File::Open(fh, tgaName, File::Mode::READ_WRITE);
	//assert(error == File::Error::SUCCESS);
	//File::Write(fh, tgaBuff, chunkSize);
	//assert(error == File::Error::SUCCESS);
	//this->textureName = new char[OBJECT_NAME_SIZE];
	//strncpy_s(this->textureName, OBJECT_NAME_SIZE, tgaName, _TRUNCATE);

	//texCount++;
	char tgaName[50];
	char tempName[50];
	//strcpy_s(tgaName, texName);
	//strcat_s(tgaName, ".tga");

	while (eat(spuName, ChunkType::TEXTURE_TYPE, texName, tgaBuff, chunkSize)) {

		//add to texture manager here
		//TextureManager::addTexture(tgaName,TextureName::MODEL);
		strcpy_s(texName, modelFileName);

		switch (texCount) {
		case 0:
			strcpy_s(tempName, texName);
			strcat_s(tempName, "_tga1");
			strcat_s(texName, "_tga0");
			break;
		case 1:
			strcpy_s(tempName, texName);
			strcat_s(tempName, "_tga2");
			strcat_s(texName, "_tga1");
			break;
		case 2:
			strcpy_s(tempName, texName);
			strcat_s(tempName, "_tga3");
			strcat_s(texName, "_tga2");
			break;
		case 3:
			strcpy_s(tempName, texName);
			strcat_s(tempName, "_tga4");
			strcat_s(texName, "_tga3");
			break;
		case 4:
			strcpy_s(tempName, texName);
			strcat_s(tempName, "_tga5");
			strcat_s(texName, "_tga4");
			break;
		case 5:
			strcpy_s(tempName, texName);
			strcat_s(tempName, "_tga6");
			strcat_s(texName, "_tga5");
			break;
		default:
			break;
		}

		strcpy_s(tgaName, texName);
		strcat_s(tgaName, ".tga");

		error = File::Open(fh, tgaName, File::Mode::READ_WRITE);
		assert(error == File::Error::SUCCESS);
		File::Write(fh, tgaBuff, chunkSize);
		assert(error == File::Error::SUCCESS);
		this->textureName = new char[OBJECT_NAME_SIZE];
		strncpy_s(this->textureName, OBJECT_NAME_SIZE, tgaName, _TRUNCATE);

		//this->textureEnum = TextureName::MODEL;

		strcpy_s(texName, tempName);
		//c = texCount;
		//tmpTex += c;
		//status = eat(spuName, ChunkType::TEXTURE_TYPE, texName, tgaBuff, chunkSize);
		texCount++;
		File::Close(fh);
		assert(error == File::Error::SUCCESS);

	}


	//read bone weights and bone index 
	//BoneWeight* bWeight;
	//BoneIndex* bIndex;

	char weightName[50];
	strcpy_s(weightName, modelFileName);
	strcat_s(weightName, "SkinWeight");

	status = eat(spuName, ChunkType::SKIN_TYPE, weightName, weightBuff, chunkSize);
	assert(status);
	unsigned int numWeights = chunkSize / sizeof(BoneWeight);
	this->numBoneWeights = (int)numWeights; // use eat here
	//unsigned int triTmp = (unsigned int)this->numTris;
	BoneWeight * weightList = new BoneWeight[(unsigned int)numWeights];


	for (unsigned int j = 0; j < numWeights; j++) {
		weightList[j] = *(BoneWeight*)weightBuff;
		weightBuff += sizeof(BoneWeight);
	}



	//bone weight indexes 
	char indexName[50];
	strcpy_s(indexName, modelFileName);
	strcat_s(indexName, "SkinInd");

	status = eat(spuName, ChunkType::SKIN_TYPE, indexName, indexBuff, chunkSize);
	assert(status);
	unsigned int numIndexes = chunkSize / sizeof(BoneIndex);
	this->numBoneIndices = (int)numIndexes; // use eat here
	//unsigned int triTmp = (unsigned int)this->numTris;
	BoneIndex * indexList = new BoneIndex[(unsigned int)numIndexes];
	

	for (unsigned int j = 0; j < numIndexes; j++) {
		indexList[j] = *(BoneIndex*)indexBuff;
		indexBuff += sizeof(BoneIndex);
	}

	ModelFileHeader modelHdr;
	strncpy_s(modelHdr.objName, OBJECT_NAME_SIZE, modelFileName, _TRUNCATE);

	//vertex buff
	modelHdr.numVerts = this->numVerts;
	modelHdr.vertBufferOffset = 0;

	//tri list
	modelHdr.numTriList = this->numTris;
	modelHdr.triListBufferOffset = 0;




	//open file
	error = File::Open(fh, azul, File::Mode::READ_WRITE);
	assert(error == File::Error::SUCCESS);
	//write hdr
	error = File::Write(fh, &modelHdr, sizeof(ModelFileHeader));
	assert(error == File::Error::SUCCESS);

	//write vertex data
	error = File::Tell(fh, modelHdr.vertBufferOffset);
	assert(error == File::Error::SUCCESS);

	error = File::Write(fh, spuVerts, sizeof(Vert_xyzuvn)* this->numVerts);
	assert(error == File::Error::SUCCESS);

	//write triangle data
	error = File::Tell(fh, modelHdr.triListBufferOffset);
	assert(error == File::Error::SUCCESS);

	error = File::Write(fh, triList, sizeof(Tri_index)* this->numTris);
	assert(error == File::Error::SUCCESS);

	//flush dat ish
	error = File::Flush(fh);
	assert(error == File::Error::SUCCESS);

	//go back and write what the offset was to the hdr file
	error = File::Seek(fh, File::Location::BEGIN, 0);
	assert(error == File::Error::SUCCESS);

	error = File::Write(fh, &modelHdr, sizeof(ModelFileHeader));
	assert(error == File::Error::SUCCESS);

	error = File::Flush(fh);
	assert(error == File::Error::SUCCESS);

	//verify that its written correctly
	ModelFileHeader hdr2;

	error = File::Seek(fh, File::Location::BEGIN, 0);
	assert(error == File::Error::SUCCESS);

	error = File::Read(fh, &hdr2, sizeof(ModelFileHeader));
	assert(error == File::Error::SUCCESS);

	//close
	error = File::Close(fh);
	assert(error == File::Error::SUCCESS);

#else

	//read from file 
	//try and rework this so it looks more elegant
	//ReadFromFile(modelFileName);
	File::Handle fh2;
	File::Error msg;

	msg = File::Open(fh2, azul, File::Mode::READ);
	assert(msg == File::Error::SUCCESS);

	//read the header data
	ModelFileHeader modelHdr;
	msg = File::Read(fh2, &modelHdr, sizeof(ModelFileHeader));
	assert(msg == File::Error::SUCCESS);

	//set read data to this obj data
	this->numVerts = modelHdr.numVerts;
	this->numTris = modelHdr.numTriList;

	//allocate data b4 writing to it
	Vert_xyzuvn *cubeData = new Vert_xyzuvn[(unsigned int)modelHdr.numVerts];
	Tri_index *triList = new Tri_index[(unsigned int)modelHdr.numTriList];

	//move to beginning of vert data then read into here
	msg = File::Seek(fh2, File::Location::BEGIN, (int)modelHdr.vertBufferOffset);
	assert(msg == File::Error::SUCCESS);
	msg = File::Read(fh2, cubeData, sizeof(Vert_xyzuvn)* this->numVerts);
	assert(msg == File::Error::SUCCESS);

	//move to beginning of tri data then read into here
	msg = File::Seek(fh2, File::Location::BEGIN, (int)modelHdr.triListBufferOffset);
	assert(msg == File::Error::SUCCESS);
	msg = File::Read(fh2, triList, sizeof(Tri_index)* this->numTris);
	assert(msg == File::Error::SUCCESS);

	msg = File::Close(fh2);
	assert(msg == File::Error::SUCCESS);
#endif


	// Create a VAO
	glGenVertexArrays(1, &this->vao);
	assert(this->vao != 0);
	glBindVertexArray(this->vao);

	// Create a VBO
	glGenBuffers(4, &this->vbo[0]);
	assert(this->vbo[0] != 0);


	//move to converter
	//Vect *pVerts = new Vect[(unsigned int)this->numVerts];
	//for (int i = 0; i < this->numVerts; i++)
	//{
	//	pVerts[i].set(spuVerts[i].x, spuVerts[i].y, spuVerts[i].z);
	//	//Trace::out("%d:  %f %f %f\n", i, pVerts[i][x], pVerts[i][y], pVerts[i][z]);
	//}
	//this->pRefSphere->RitterSphere(pVerts, this->numVerts);

	//delete[] pVerts;



	// Load the combined data: ---------------------------------------------------------

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);

	// load the data to the GPU
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), spuVerts, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), pVerts, GL_STATIC_DRAW);

	// Define an array of generic vertex attribute data

		// Vert data is location: 0  (used in vertex shader)
		// todo make a table or enum
	void *offsetVert = (void *)((unsigned int)&spuVerts[0].x - (unsigned int)spuVerts);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetVert);
	glEnableVertexAttribArray(0);

	// Texture data is location: 1  (used in vertex shader)
	void *offsetTex = (void *)((unsigned int)&spuVerts[0].u - (unsigned int)spuVerts);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
	glEnableVertexAttribArray(1);

	// normals data in location 2 (used in vertex shader
	void *offsetNorm = (void *)((unsigned int)&spuVerts[0].nx - (unsigned int)spuVerts);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
	glEnableVertexAttribArray(2);

	// Load the index data: ---------------------------------------------------------

		/* Bind our 2nd VBO as being the active buffer and storing index ) */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);

	/* Copy the index data to our buffer */
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), triList, GL_STATIC_DRAW);

	//add inverse matrix, weight data, and index data

		//Gabe
	//bone weights 
	//could be 0 instead of 4 or 5
	glBindBuffer(GL_ARRAY_BUFFER,this->vbo[2]);
	glBufferData(GL_ARRAY_BUFFER,(GLsizeiptr)(sizeof(BoneWeight)*this->numBoneWeights),weightList,GL_STATIC_DRAW);
	glVertexAttribPointer(3,4,GL_FLOAT,GL_FALSE,sizeof(BoneWeight),0);
	glEnableVertexAttribArray(3);




	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(BoneIndex)*this->numBoneIndices), indexList, GL_STATIC_DRAW);
	glVertexAttribIPointer(4, 4, GL_INT,  sizeof(BoneIndex), 0);
	glEnableVertexAttribArray(4);

}


#pragma warning(pop)

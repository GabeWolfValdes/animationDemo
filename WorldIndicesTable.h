#ifndef _WORLD_INDICIES_TABLE_
#define _WORLD_INDICIES_TABLE_

#include "MathApp.h"
#include "FrameBucket.h"

class WorldIndexTable
{
public:
	static WorldIndexTable *humanoid;
	static WorldIndexTable *teddy;

	int tableWidth;
	int *table;

	WorldIndexTable() : tableWidth(0), table(nullptr) {}
	WorldIndexTable(int tableWidthIn, int *tableIn) : 
		tableWidth(tableWidthIn), table(tableIn) {}
	Matrix GetBoneWorld(const Matrix &rootMat, FrameBucket *frame, int boneIndex);
};


#endif // !_WORLD_INDICIES_TABLE_

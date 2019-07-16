//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
// Optimized C++
//----------------------------------------------------------------------------- 

#include <stdio.h>
#include <assert.h>

#include "eat.h"
#include "ChunkHeader.h"
#include "PackageHeader.h"

#include "File.h"

#define UNUSED_VAR(x) ((void *)x)

//---------------------------------------------------------------------------
// Leave this signature - do your EAT magic here
// Feel free to add files and methods to this project
//---------------------------------------------------------------------------

bool eat(const char * const inFileName,
		 ChunkType type,
		 const char * const chunkName,
		 unsigned char *&chunkBuff,
		 unsigned int &chunkSize)
{
	File::Handle fh(nullptr);
	File::Error msg;
	DWORD fSize(0);
	PackageHeader* package;// = new PackageHeader;
	ChunkHeader* chunk = new ChunkHeader;
	bool found = false;
	unsigned int offset =0;

	msg = File::Open(fh,inFileName,File::Mode::READ);
	assert(msg == File::Error::SUCCESS);

	msg = File::Seek(fh, File::Location::END, 0);
	assert(msg == File::Error::SUCCESS);

	msg = File::Tell(fh, fSize);
	assert(msg == File::Error::SUCCESS);

	char* buffer = new char[(unsigned int)fSize];

	msg = File::Seek(fh, File::Location::BEGIN, 0);
	assert(msg == File::Error::SUCCESS);

	msg = File::Read(fh, buffer, fSize);
	assert(msg == File::Error::SUCCESS);

	//msg = File::Seek(fh, File::Location::BEGIN, 0);
	//assert(msg == File::Error::SUCCESS);

	package = (PackageHeader*)buffer;
	//chunk = (ChunkHeader*) (package + sizeof(PackageHeader));
	File::Seek(fh, File::Location::BEGIN, sizeof(PackageHeader));
	assert(msg == File::Error::SUCCESS);




	while (!found && (offset < package->totalSize)) {

		File::Read(fh, chunk,sizeof(ChunkHeader));
		assert(msg == File::Error::SUCCESS);
		
		if (chunk->type == type) {
			if (strncmp((char*)chunk->chunkName, (char*)chunkName, chunk->CHUNK_NAME_SIZE)==0) {

				chunkBuff = new unsigned char[chunk->chunkSize];
				File::Read(fh,chunkBuff,chunk->chunkSize);
				assert(msg == File::Error::SUCCESS);

				chunkSize = chunk->chunkSize;
				found = true;
			}
		}
		//get to end of chunk
		File::Seek(fh, File::Location::CURRENT, (int)chunk->chunkSize);
		offset += chunk->chunkSize;

	}

	//free up resources
	File::Close(fh);
	free(package);
	delete chunk;
	//delete[] buffer;


	return found;
}

// End of File

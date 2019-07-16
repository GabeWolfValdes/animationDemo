//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "sb7.h"

#include "TextureNodeLink.h"

class Texture
{
public:
	static const unsigned int ASSET_NAME_SIZE = 64;

	enum class Name
	{
		STONES,
		RED_BRICK,
		DUCKWEED,
		ROCKS,
		INVADERS,
		FRIGATE,
		NOT_INITIALIZED
	};

public:	
	Texture(const Texture &) = delete;
	Texture & operator = (const Texture &) = delete;
	~Texture() = default;

	static void Add( const char * const _assetName, const Texture::Name _name);
	static void DeleteAllTextures();
	static TextureNodeLink* Find( const Texture::Name _name);
	static GLuint FindID(const Texture::Name _name);


private:  
	// methods
	Texture();
	static Texture *privGetInstance();

	// helper private methods
	void privLoadTexture( const char * const _assetName, GLuint *&textureID, int &width, int &height );
	bool privLoadTGATexture(const char *szFileName, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode, int &width, int &height);
	void privAddToFront(TextureNodeLink *node, TextureNodeLink *&head);
	GLbyte *gltReadTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);

private: 

	TextureNodeLink *active;
};

#endif

// --- End of File --------------------------------------------------

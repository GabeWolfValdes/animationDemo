//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include <sb7.h>

class ShaderObject
{
public:
	enum Name
	{
		NULL_SHADER,
		COLOR_LIGHT,
		COLOR_SINGLE,
		TEXTURE_SIMPLE,
		TEXTURE_POINT_LIGHT,
		CONST_COLOR,
		SPRITE,
		SKIN,
		SKINLIGHT,
		TEST_COMPUTE,
		WORLD_MATRIX_COMPUTE,
		BONE_WORLD_COMPUTE,
		BONE_WORLD_COMPUTE_TEST,
		ANIMATION_COMPUTE
	};

	enum Type {
		VERTEX,
		COMPUTE
	};


public:
	ShaderObject(ShaderObject::Name shaderName, const char * const pShaderBaseFileName);

	ShaderObject(ShaderObject::Name shaderName, const char * const pShaderBaseFileName, GLenum shader_type);

	ShaderObject() = delete;
	ShaderObject(const ShaderObject &) = delete;
	ShaderObject &operator = (ShaderObject &) = delete;
	virtual ~ShaderObject() = default;

	void SetActive();
	GLint GetLocation(const char * pUniformName);
	void Dispatch(unsigned int groupsize_x, unsigned int groupsize_y, unsigned int groupsize_z);

private:
	bool privCreateShader(GLuint &programObject, const char * const pShaderBaseFileName);
	bool privCreateShader(GLuint & inProgramObject, const char * const pShaderBaseFileName, GLenum _shaderType);
	bool privLoadNCompile(GLuint &shaderObject, const char * const pShaderFileName, GLenum shader_type);


public:
	Name	name;
	GLuint	programObject;
	Type	type = VERTEX;

	ShaderObject* prev;
	ShaderObject* next;



};


#endif

// --- End of File --------------------------------------------------

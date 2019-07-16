#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "ShaderObject.h"

// Singleton
class ShaderMan
{
public:
	static void Add(ShaderObject *pShader);
	static void Create();
	static void Destroy();
	static ShaderObject * Find(ShaderObject::Name reqName);

private:  // methods

	static ShaderMan *privGetInstance();
	ShaderMan();

	void privAddToFront(ShaderObject *node, ShaderObject *&head);
	void privRemove(ShaderObject *node, ShaderObject *&head);


private:  // add

	ShaderObject *active;

};

#endif
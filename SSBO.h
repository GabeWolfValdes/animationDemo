#ifndef SSBO_H
#define SSBO_H


#include <sb7.h>

class SSBO
{
public:
	enum class Access
	{
		READ_ONLY = 0x88B8,
		WRITE_ONLY = 0x88B9,
		READ_WRITE = 0x88BA
	};


public:
	SSBO();
	SSBO(const SSBO &) = delete;
	const SSBO & operator = (const SSBO &) = delete;
	~SSBO();

	void Set(unsigned int elementCount, unsigned int size, void *inData);

	unsigned int GetLocation() const;
	void Bind(unsigned bindLocation);
	void *Map(SSBO::Access accessType);
	void Unmap();
	void* GetBuffer();

private:
	GLuint ssbo;
	unsigned int elementCount;
	unsigned int elementSize;
	void * pBuffer;
};






#endif // !SSBO_H

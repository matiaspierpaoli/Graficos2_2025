#include "VertexBufferElement.h"
#include <glew/include/GL/glew.h>
#include <assert.h>

unsigned int VertexBufferElement::GetSizeOfType(unsigned int type)
{
	switch (type)
	{
	case GL_FLOAT:			return 4;
	case GL_UNSIGNED_INT:	return 4;
	case GL_UNSIGNED_BYTE:	return 1;
	}
	assert(false);
	return 0;
}
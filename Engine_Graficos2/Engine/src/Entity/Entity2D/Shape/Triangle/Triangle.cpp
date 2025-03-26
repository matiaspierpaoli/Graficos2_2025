#include "Triangle.h"
#include "RendererSingleton.h"

Triangle::Triangle()
{
	float positions[4][2] = {
		/*-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f, 0.5f*/

		-0.5f, -0.5f,
		 0.0f, 0.5f,
		 0.5f, -0.5f
	};

	unsigned int tempIndices[3] = {
		0, 1, 2		
	};

	*vBuffer = RendererSingleton::GetRenderer()->GetNewVertexBuffer(positions, 6 * (sizeof(float) + sizeof(float) * 4));
	*iBuffer = RendererSingleton::GetRenderer()->GetNewIndexBuffer(tempIndices, 3);

}

Triangle::~Triangle()
{

}

void Triangle::Draw()
{
	RendererSingleton::GetRenderer()->Draw(*vBuffer, *iBuffer, modelId);
}

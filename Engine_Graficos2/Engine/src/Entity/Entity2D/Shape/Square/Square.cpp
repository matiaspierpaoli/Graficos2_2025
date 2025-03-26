#include "Square.h"
#include "RendererSingleton.h"

Square::Square(float vertexCol[4][4])
{
	float vertexPos[4][2] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f,
	};

	unsigned int tempIndices[6] = {
		0, 1, 2,
		2, 3, 0,
	};

	float tempVertices[4][6];
	for (unsigned short i = 0; i < 4; i++)
	{
		for (unsigned short j = 0; j < 2; j++)
		{
			tempVertices[i][j] = vertexPos[i][j];
		}
		for (unsigned short j = 2; j < 6; j++)
		{
			tempVertices[i][j] = vertexCol[i][j - 2];
		}
	}

	*vBuffer = RendererSingleton::GetRenderer()->GetNewVertexBuffer(tempVertices, 4 * (sizeof(float) * 2 + sizeof(float) * 4)  );
	*iBuffer = RendererSingleton::GetRenderer()->GetNewIndexBuffer(tempIndices, 6);

}

Square::~Square()
{

}

void Square::Draw()
{
	RendererSingleton::GetRenderer()->Draw(*vBuffer, *iBuffer, modelId);
}

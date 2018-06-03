#pragma once
#include "DirectXDevice.h"
#include "Vertex.h"

class VBO
{
public:
	VBO();
	~VBO();

	void Create(DirectXDevice* device, Vertex vertices[], int numVerticies);

	void Draw(DirectXDevice* device);

	void Release();

private:
	/// The number of vertices in the vbo.
	ID3D11Buffer* mpVBO;
	int miNumVertices;
};


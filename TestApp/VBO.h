#pragma once
#include "DirectXDevice.h"
#include "Vertex.h"
#include <vector>

class VBO
{
public:
	VBO();
	~VBO();

	void Create(DirectXDevice* device, Vertex vertices[], int numVerticies);
	void Create(DirectXDevice* device, std::vector<Vertex> vertices);

	void Draw(DirectXDevice* device);
	void SetVBO(DirectXDevice* device);

	void Release();

private:
	/// The number of vertices in the vbo.
	ID3D11Buffer* mpVBO;
	int miNumVertices;
};


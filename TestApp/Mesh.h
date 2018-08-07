#pragma once
#include "VBO.h"
#include "IndexBuffer.h"
#include "Vertex.h"
#include "TextureDetails.h"
#include "DirectXDevice.h"

#include <vector>

class Mesh
{
public:
	Mesh();
	Mesh(std::vector<Vertex> vertices);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indicies, std::vector<TextureDetail> textureDetails);
	~Mesh();

	VBO* GetVBO() const { return mpVbo; }
	int NumVertices() const { return (int)mVertices.size(); }
	Vertex GetVertex(int i) const { return mVertices[i]; }
	Vertex& GetVertexRef(int i) { return mVertices[i]; }

	VBO* CreateVBO(DirectXDevice* device);
	bool AddVertex(Vertex v);

	void SetupMesh(DirectXDevice* device);
	void Draw(DirectXDevice* device);

	bool Clear();
	bool DeleteVertex(int i);

	void Reset();
	void Release();

private:
	/// State of the vbo.
	bool mLocked;
	/// The meshes vbo.
	VBO* mpVbo;
	IndexBuffer* mpIndexBuffer;
	/// The meshes vertices.
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	std::vector<TextureDetail> mTextureDetails;
};


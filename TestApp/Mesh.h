#pragma once
#include "VBO.h"
#include "Vertex.h"
#include "DirectXDevice.h"

#include <vector>

class Mesh
{
public:
	Mesh();
	~Mesh();

	VBO* GetVBO() const { return mpVbo; }
	int NumVertices() const { return (int)mVertices.size(); }
	Vertex GetVertex(int i) const { return mVertices[i]; }
	Vertex& GetVertexRef(int i) { return mVertices[i]; }

	VBO* CreateVBO(DirectXDevice* device);
	bool AddVertex(Vertex v);
	bool Clear();
	bool DeleteVertex(int i);

	void Reset();
	void Release();

private:
	/// State of the vbo.
	bool mLocked;
	/// The meshes vbo.
	VBO* mpVbo;
	/// The meshes vertices.
	std::vector<Vertex> mVertices;
};


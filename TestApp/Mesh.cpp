#include "Mesh.h"
#include "Log.h"
#include <glm\common.hpp>

Mesh::Mesh()
	: mLocked(false),
	mpVbo(NULL),
	mpIndexBuffer(NULL)

{
}

Mesh::Mesh(std::vector<Vertex> vertices) :
	Mesh()
{
	mVertices = vertices;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) :
	Mesh()
{
	mVertices = vertices;
	mIndices = indices;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indicies, std::vector<TextureDetail> textureDetails) :
	Mesh()
{
	mVertices = vertices;
	mIndices = indicies;
	mTextureDetails = textureDetails;
}


Mesh::~Mesh()
{
	delete mpVbo;
	mpVbo = NULL;
}

/**
*  @brief Adds a vertex to the list of vertices.
*
*  @param v The vertex to add
*  @return bool indicating success.
*/
bool Mesh::AddVertex(Vertex v)
{
	if (!mLocked)
	{
		mVertices.push_back(v);
		return true;
	}
	else
	{
		return false;
	}
}

/**
*  @brief Deletes a vertex from the list of vertices
*
*  Deletes the vertex at index i
*  @param i The index of the vertex to delete.
*  @return bool indicating success.
*/
bool Mesh::DeleteVertex(int i)
{
	if (!mLocked)
	{
		mVertices.erase(mVertices.begin() + i);
		return true;
	}
	else
	{
		return false;
	}
}

/**
*  @brief Clears the list of vertices.
*
*  Clears the list of vertices.
*
*  @return bool indicating success.
*/
bool Mesh::Clear()
{
	if (!mLocked)
	{
		mVertices.clear();
		return true;
	}
	return false;
}

/**
*  @brief Deletes the current vbo so that another one can be made.
*/
void Mesh::Reset()
{
	delete mpVbo;
	mpVbo = NULL;
	mLocked = false;
}

/**
*  @brief Creates the VBO.
*
*  Creates the VBO using a GraphicsDevice, _locked is set to true.
*  @returns The created vbo.
*/
VBO* Mesh::CreateVBO(DirectXDevice* device)
{
	mLocked = true;

	if (mpVbo == nullptr)
		mpVbo = new VBO();

	mpVbo->Create(device, mVertices.data(), NumVertices());

	return mpVbo;
}

/**
*  @brief Safely releases all the resources being used by the mesh.
*/
void Mesh::Release()
{
	if (mpVbo)
	{
		mpVbo->Release();
	}
	mLocked = false;
	Clear();
}

void Mesh::SetupMesh(DirectXDevice* device)
{
	if (mpVbo)
	{
		mpVbo->Release();
		delete mpVbo;
		mpVbo = nullptr;
	}
	if (mpIndexBuffer)
	{
		mpIndexBuffer->Release();
		delete mpIndexBuffer;
		mpIndexBuffer = nullptr;
	}
	mLocked = false;
	
	if (mVertices.size() > 0)
	{
		mpVbo = new VBO();
		mpVbo->Create(device, mVertices);
	}

	if (mIndices.size() > 0)
	{
		mpIndexBuffer = new IndexBuffer();
		mpIndexBuffer->Create(device, mIndices);
	}
}

void Mesh::Draw(DirectXDevice* device)
{
	if (!mpVbo) return;

	mpVbo->SetVBO(device);


	// select primitive type
	device->GetContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (mTextureDetails.size() > 0 && mTextureDetails[0].mTexture)
	{
		device->GetContext()->PSSetShaderResources(0, 1, mTextureDetails[0].mTexture->GetAddressOfShaderResourceView());
	}

	if (mpIndexBuffer && mIndices.size() > 0)
	{
		mpIndexBuffer->SetIndexBuffer(device);
		device->GetContext()->DrawIndexed(mIndices.size(), 0, 0);
	}
	else
	{
		// draw the vertex buffer to the back buffer
		device->GetContext()->Draw(mVertices.size(), 0);
	}
}

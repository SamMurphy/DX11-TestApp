#include "Mesh.h"
#include "Log.h"
#include <glm\common.hpp>

Mesh::Mesh()
	: mLocked(false),
	mpVbo(NULL)

{
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

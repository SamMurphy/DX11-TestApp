#pragma once
#include "VBO.h"
#include "Log.h"


VBO::VBO()
{
}


VBO::~VBO()
{
}


void VBO::Create(DirectXDevice* device, Vertex vertices[], int numVertices)
{
	miNumVertices = numVertices;

	if (miNumVertices <= 0) LOG_ERROR << "No vertices for vbo creation";

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * numVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// Create the buffer
	HRESULT result;
	result = device->GetDevice()->CreateBuffer(&bd, NULL, &mpVBO);
	if (result != S_OK)
	{
		LOG_ERROR << "Failed to create directX vbo";
	}

	// Copy vertices into buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	result = device->GetContext()->Map(mpVBO, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	if (result != S_OK)
	{
		LOG_ERROR << "Failed to create directX vbo";
	}
	memcpy(ms.pData, vertices, sizeof(Vertex) * numVertices);
	device->GetContext()->Unmap(mpVBO, NULL);
}

void VBO::Create(DirectXDevice * device, std::vector<Vertex> vertices)
{
	miNumVertices = vertices.size();

	if (miNumVertices <= 0) LOG_ERROR << "No vertices for vbo creation";

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * miNumVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// Create the buffer
	HRESULT result;
	result = device->GetDevice()->CreateBuffer(&bd, NULL, &mpVBO);
	if (result != S_OK)
	{
		LOG_ERROR << "Failed to create directX vbo";
	}

	// Copy vertices into buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	result = device->GetContext()->Map(mpVBO, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	if (result != S_OK)
	{
		LOG_ERROR << "Failed to create directX vbo";
	}
	memcpy(ms.pData, &vertices[0], sizeof(Vertex) * miNumVertices);
	device->GetContext()->Unmap(mpVBO, NULL);
}

void VBO::Draw(DirectXDevice* device)
{
	// select the buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	device->GetContext()->IASetVertexBuffers(0, 1, &mpVBO, &stride, &offset);

	// select primitive type
	device->GetContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw the vertex buffer to the back buffer
	device->GetContext()->Draw(miNumVertices, 0);
}

void VBO::SetVBO(DirectXDevice * device)
{
	// select the buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	device->GetContext()->IASetVertexBuffers(0, 1, &mpVBO, &stride, &offset);
}

void VBO::Release()
{
	if (mpVBO)
		mpVBO->Release();
}
#pragma once
#include <vector>
#include "DirectXDevice.h"

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	void Create(DirectXDevice* device, std::vector<unsigned int> indicies);
	void Release();

	void SetIndexBuffer(DirectXDevice* device);

private:
	ID3D11Buffer* mpIndexBuffer;
	int mNumberOfIndices;
};


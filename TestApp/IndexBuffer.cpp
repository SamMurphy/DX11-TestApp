#include "IndexBuffer.h"
#include "Log.h"


IndexBuffer::IndexBuffer() :
	mpIndexBuffer(nullptr)
{
}


IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::Create(DirectXDevice * device, std::vector<unsigned int> indicies)
{
	mNumberOfIndices = indicies.size();

	// Fill in a buffer description.
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(unsigned int) * mNumberOfIndices;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	
	// Define the resource data.
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &indicies[0];
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	// Create the buffer with the device.
	HRESULT hr = device->GetDevice()->CreateBuffer(&bufferDesc, &initData, &mpIndexBuffer);
	
	if (FAILED(hr))
	{
		LOG_ERROR << "Failed to create Index buffer";
	}

	// Set the buffer.
	device->GetContext()->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void IndexBuffer::Release()
{
	if (mpIndexBuffer)
	{
		mpIndexBuffer->Release();
		mpIndexBuffer = nullptr;
	}
}

void IndexBuffer::SetIndexBuffer(DirectXDevice * device)
{
	// Set the buffer.
	device->GetContext()->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

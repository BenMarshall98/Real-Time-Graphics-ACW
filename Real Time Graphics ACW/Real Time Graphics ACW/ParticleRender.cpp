#include "ParticleRender.h"
#include "DX11Render.h"
#include "ParticleManager.h"

bool ParticleRender::loadParticles()
{
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Dx11Render::instance()->getDevice(device);


	//None changing data for Vertex Buffers;
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof bufferDesc);

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof initData);

	//Create Position Buffer
	std::vector<DirectX::XMFLOAT2> vertices;
	vertices.emplace_back(DirectX::XMFLOAT2(-1.0f, -1.0f));
	vertices.emplace_back(DirectX::XMFLOAT2(1.0f, -1.0f));
	vertices.emplace_back(DirectX::XMFLOAT2(1.0f, 1.0f));
	vertices.emplace_back(DirectX::XMFLOAT2(-1.0f, 1.0f));

	const auto vertexSize = vertices.size();
	
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT2) * vertexSize;
	initData.pSysMem = vertices.data();

	auto result = device->CreateBuffer(&bufferDesc, &initData, &mPositionBuffer);

	if (FAILED(result))
	{
		//TODO: Log error
		return false;
	}

	//Create TexCoord Buffer
	std::vector<DirectX::XMFLOAT2> texCoords;
	texCoords.emplace_back(DirectX::XMFLOAT2(0.0f, 0.0f));
	texCoords.emplace_back(DirectX::XMFLOAT2(1.0f, 0.0f));
	texCoords.emplace_back(DirectX::XMFLOAT2(1.0f, 1.0f));
	texCoords.emplace_back(DirectX::XMFLOAT2(0.0f, 1.0f));

	const auto texSize = texCoords.size();

	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT2) * texSize;
	initData.pSysMem = texCoords.data();

	result = device->CreateBuffer(&bufferDesc, &initData, &mTexCoordBuffer);

	if (FAILED(result))
	{
		//TODO: Log error
		return false;
	}

	//None changing data for Vertex Buffers;
	ZeroMemory(&bufferDesc, sizeof bufferDesc);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//Create Particle Position Buffer
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT3) * ParticleManager::max_particles;

	result = device->CreateBuffer(&bufferDesc, nullptr, &mParticlePositionBuffer);

	if (FAILED(result))
	{
		//TODO: Log error
		return false;
	}

	//Create Particle Time Buffer
	bufferDesc.ByteWidth = sizeof(float) * ParticleManager::max_particles;

	result = device->CreateBuffer(&bufferDesc, nullptr, &mParticleTimeBuffer);

	if (FAILED(result))
	{
		//TODO: Log error
		return false;
	}

	//Create Index Buffer
	std::vector<WORD> index;
	index.emplace_back(2);
	index.emplace_back(1);
	index.emplace_back(0);
	index.emplace_back(0);
	index.emplace_back(3);
	index.emplace_back(2);

	mIndicesSize = index.size();
	
	ZeroMemory(&bufferDesc, sizeof bufferDesc);

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	bufferDesc.ByteWidth = sizeof(WORD) * mIndicesSize;
	
	ZeroMemory(&initData, sizeof initData);
	initData.pSysMem = index.data();

	result = device->CreateBuffer(&bufferDesc, &initData, &mIndicesBuffer);

	if (FAILED(result))
	{
		//TODO: Log error
		return false;
	}

	return true;
}

void ParticleRender::render(const std::vector<DirectX::XMFLOAT3>& pParticlePositions, const std::vector<float>& pParticleTimes) const
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Dx11Render::instance()->getDeviceContext(deviceContext);

	D3D11_MAPPED_SUBRESOURCE mappedData;

	//Map Particle Positions
	ZeroMemory(&mappedData, sizeof mappedData);

	auto result = deviceContext->Map(mParticlePositionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	
	memcpy(mappedData.pData, pParticlePositions.data(), sizeof(DirectX::XMFLOAT3) * pParticlePositions.size());

	deviceContext->Unmap(mParticlePositionBuffer.Get(), 0);

	//Map Particle Time
	ZeroMemory(&mappedData, sizeof mappedData);

	result = deviceContext->Map(mParticleTimeBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

	memcpy(mappedData.pData, pParticleTimes.data(), sizeof(float) * pParticleTimes.size());

	deviceContext->Unmap(mParticleTimeBuffer.Get(), 0);

	//Get current model data

	//TODO: Check data output

	std::vector<ID3D11Buffer *> bufferArray;
	bufferArray.resize(5);
	std::vector<UINT> strideArray;
	strideArray.resize(5);
	std::vector<UINT> offsetArray;
	offsetArray.resize(5);

	ID3D11Buffer * indexBuffer;
	DXGI_FORMAT format;
	UINT offset;

	D3D11_PRIMITIVE_TOPOLOGY topology;

	deviceContext->IAGetVertexBuffers(0, 5, bufferArray.data(), strideArray.data(), offsetArray.data());
	deviceContext->IAGetIndexBuffer(&indexBuffer, &format, &offset);
	deviceContext->IAGetPrimitiveTopology(&topology);

	//Draw particles

	const auto numberBuffers = 4u;
	
	std::vector<ID3D11Buffer *> particleBufferArray;
	particleBufferArray.emplace_back(mPositionBuffer.Get());
	particleBufferArray.emplace_back(mTexCoordBuffer.Get());
	particleBufferArray.emplace_back(mParticlePositionBuffer.Get());
	particleBufferArray.emplace_back(mParticleTimeBuffer.Get());

	std::vector<UINT> particleStrideArray;
	particleStrideArray.emplace_back(sizeof(DirectX::XMFLOAT2));
	particleStrideArray.emplace_back(sizeof(DirectX::XMFLOAT2));
	particleStrideArray.emplace_back(sizeof(DirectX::XMFLOAT3));
	particleStrideArray.emplace_back(sizeof(float));

	std::vector<UINT> particleOffsetArray;
	particleOffsetArray.emplace_back(0);
	particleOffsetArray.emplace_back(0);
	particleOffsetArray.emplace_back(0);
	particleOffsetArray.emplace_back(0);
	
	deviceContext->IASetVertexBuffers(0, numberBuffers, particleBufferArray.data(), particleStrideArray.data(), particleOffsetArray.data());
	deviceContext->IASetIndexBuffer(mIndicesBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->DrawIndexedInstanced(mIndicesSize, pParticlePositions.size(), 0, 0, 0);

	//Set previous model data

	deviceContext->IASetVertexBuffers(0, 5, bufferArray.data(), strideArray.data(), offsetArray.data());
	deviceContext->IASetIndexBuffer(indexBuffer, format, offset);
	deviceContext->IASetPrimitiveTopology(topology);
}
#include "Model.h"
#include "DX11Render.h"

bool Model::loadModel(const std::vector<DirectX::XMFLOAT3> & pPositions, const std::vector<DirectX::XMFLOAT3> & pNormals,
	const std::vector<DirectX::XMFLOAT2> & pTexCoords, const std::vector<DirectX::XMFLOAT3> & pTangents,
	const std::vector<DirectX::XMFLOAT3> & pBiTangents, const std::vector<WORD> & pIndices)
{
	mIndicesSize = pIndices.size();
	
	auto device = Dx11Render::instance()->getDevice();

	//None changing data for buffers;
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));

	//Create Position Buffer
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT3) * pPositions.size();
	initData.pSysMem = pPositions.data();

	auto result = device->CreateBuffer(&bufferDesc, &initData, &mPositionBuffer);

	if (FAILED(result))
	{
		//TODO: Log error
		return false;
	}

	//Create Normal Buffer
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT3) * pNormals.size();
	initData.pSysMem = pNormals.data();

	result = device->CreateBuffer(&bufferDesc, &initData, &mNormalBuffer);

	if (FAILED(result))
	{
		//TODO: Log error
		return false;
	}

	//Create TexCoord Buffer
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT2) * pTexCoords.size();
	initData.pSysMem = pTexCoords.data();

	result = device->CreateBuffer(&bufferDesc, &initData, &mTexCoordBuffer);

	if (FAILED(result))
	{
		//TODO: Log error
		return false;
	}

	//Create Tangent Buffer
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT3) * pTangents.size();
	initData.pSysMem = pTangents.data();

	result = device->CreateBuffer(&bufferDesc, &initData, &mTangentBuffer);

	if (FAILED(result))
	{
		//TODO: Log error
		return false;
	}

	//Create BiTangent Buffer
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT3) * pBiTangents.size();
	initData.pSysMem = pBiTangents.data();

	result = device->CreateBuffer(&bufferDesc, &initData, &mBiTangentBuffer);

	if (FAILED(result))
	{
		//TODO: Log error
		return false;
	}

	//Create Index Buffer
	bufferDesc.ByteWidth = sizeof(WORD) * pIndices.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = pIndices.data();

	result = device->CreateBuffer(&bufferDesc, &initData, &mIndicesBuffer);

	if (FAILED(result))
	{
		//TODO: Log error
		return false;
	}

	return true;
}

void Model::render()
{
	auto deviceContext = Dx11Render::instance()->getDeviceContext();

	static Model * lastModel = nullptr;

	const auto numberBuffers = 5;

	if (this != lastModel)
	{
		ID3D11Buffer * bufferArray[numberBuffers] =
		{
			mPositionBuffer.Get(), mNormalBuffer.Get(), mTexCoordBuffer.Get(), mTangentBuffer.Get(), mBiTangentBuffer.Get()
		};

		UINT strideArray[numberBuffers] =
		{
			sizeof(DirectX::XMFLOAT3), sizeof(DirectX::XMFLOAT3), sizeof(DirectX::XMFLOAT2), sizeof(DirectX::XMFLOAT3), sizeof(DirectX::XMFLOAT3)
		};

		UINT offsetArray[numberBuffers] =
		{
			0, 0, 0, 0, 0
		};

		deviceContext->IASetVertexBuffers(0, numberBuffers, bufferArray, strideArray, offsetArray);
		deviceContext->IASetIndexBuffer(mIndicesBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		lastModel = this;
	}

	deviceContext->DrawIndexed(mIndicesSize, 0, 0);
}

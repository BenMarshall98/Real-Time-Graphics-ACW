#include "Model.h"
#include "DX11Render.h"

Model::Model(const std::vector<DirectX::XMFLOAT3> & pPositions, const std::vector<DirectX::XMFLOAT3> & pNormals,
	const std::vector<DirectX::XMFLOAT2> & pTexCoords, const std::vector<DirectX::XMFLOAT3> & pTangents,
	const std::vector<DirectX::XMFLOAT3> & pBiTangents, const std::vector<WORD> & pIndices) : mIndicesSize(pIndices.size())//mPositions(pPositions), mNormals(pNormals), mTexCoords(pTexCoords), mIndices(pIndices)
{
	ID3D11Device * device = DX11Render::Instance()->GetDevice();

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

	device->CreateBuffer(&bufferDesc, &initData, &mPositionBuffer);

	//Create Normal Buffer
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT3) * pNormals.size();
	initData.pSysMem = pNormals.data();

	device->CreateBuffer(&bufferDesc, &initData, &mNormalBuffer);

	//Create TexCoord Buffer
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT2) * pTexCoords.size();
	initData.pSysMem = pTexCoords.data();

	device->CreateBuffer(&bufferDesc, &initData, &mTexCoordBuffer);

	//Create Tangent Buffer
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT3) * pTangents.size();
	initData.pSysMem = pTangents.data();

	device->CreateBuffer(&bufferDesc, &initData, &mTangentBuffer);

	//Create BiTangent Buffer
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT3) * pBiTangents.size();
	initData.pSysMem = pBiTangents.data();

	device->CreateBuffer(&bufferDesc, &initData, &mBiTangentBuffer);

	//Create Index Buffer
	bufferDesc.ByteWidth = sizeof(WORD) * pIndices.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = pIndices.data();

	device->CreateBuffer(&bufferDesc, &initData, &mIndicesBuffer);
}

Model::~Model()
{
	mPositionBuffer->Release();
	mNormalBuffer->Release();
	mTexCoordBuffer->Release();
	mTangentBuffer->Release();
	mBiTangentBuffer->Release();
	mIndicesBuffer->Release();
}

void Model::Render()
{
	ID3D11DeviceContext * deviceContext = DX11Render::Instance()->GetDeviceContext();

	static Model * lastModel = nullptr;

	if (this != lastModel)
	{
		ID3D11Buffer * bufferArray[2] =
		{
			mPositionBuffer, mNormalBuffer//, texCoordBuffer
		};

		UINT strideArray[2] =
		{
			sizeof(DirectX::XMFLOAT3), sizeof(DirectX::XMFLOAT3)//, sizeof(DirectX::XMFLOAT2)
		};

		UINT offsetArray[2] =
		{
			0, 0//, 0
		};

		deviceContext->IASetVertexBuffers(0, 2, bufferArray, strideArray, offsetArray);
		deviceContext->IASetIndexBuffer(mIndicesBuffer, DXGI_FORMAT_R16_UINT, 0);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		lastModel = this;
	}

	deviceContext->DrawIndexed(mIndicesSize, 0, 0);
}

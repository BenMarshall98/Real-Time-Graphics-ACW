#include "Model.h"
#include "DX11Render.h"

Model::Model(std::vector<DirectX::XMFLOAT3> pPositions, std::vector<DirectX::XMFLOAT3> pNormals,
	std::vector<DirectX::XMFLOAT2> pTexCoords, std::vector<unsigned int> pIndices) : mPositions(pPositions), mNormals(pNormals), mTexCoords(pTexCoords), mIndices(pIndices)
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
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT3) * mPositions.size();
	initData.pSysMem = mPositions.data();

	device->CreateBuffer(&bufferDesc, &initData, &positionBuffer);

	//Create Normal Buffer
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT3) * mNormals.size();
	initData.pSysMem = mNormals.data();

	device->CreateBuffer(&bufferDesc, &initData, &normalBuffer);

	////Create TexCoord Buffer
	//bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT2) * mTexCoords.size();
	//initData.pSysMem = mTexCoords.data();

	//device->CreateBuffer(&bufferDesc, &initData, &texCoordBuffer);

	//Create Index Buffer
	bufferDesc.ByteWidth = sizeof(unsigned int) * mIndices.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = mIndices.data();

	device->CreateBuffer(&bufferDesc, &initData, &indicesBuffer);
}

Model::~Model()
{
	positionBuffer->Release();
	normalBuffer->Release();
	texCoordBuffer->Release();
	indicesBuffer->Release();
}

void Model::Render()
{
	ID3D11DeviceContext * deviceContext = DX11Render::Instance()->GetDeviceContext();

	static Model * lastModel = nullptr;

	if (this != lastModel)
	{
		ID3D11Buffer * bufferArray[2] =
		{
			positionBuffer, normalBuffer//, texCoordBuffer
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
		deviceContext->IASetIndexBuffer(indicesBuffer, DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		lastModel = this;
	}

	deviceContext->DrawIndexed(mIndices.size(), 0, 0);
}

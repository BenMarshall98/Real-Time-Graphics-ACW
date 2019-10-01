#pragma once

#include <DirectXMath.h>
#include <vector>
#include <d3d11.h>

class Model
{
private:
	std::vector<DirectX::XMFLOAT3> mPositions;
	std::vector<DirectX::XMFLOAT3> mNormals;
	std::vector<DirectX::XMFLOAT2> mTexCoords;
	std::vector<unsigned int> mIndices;

	ID3D11Buffer * positionBuffer;
	ID3D11Buffer * normalBuffer;
	ID3D11Buffer * texCoordBuffer;
	ID3D11Buffer * indicesBuffer;

public:
	Model(std::vector<DirectX::XMFLOAT3> pPositions, std::vector<DirectX::XMFLOAT3> pNormals,
		std::vector<DirectX::XMFLOAT2> pTexCoords, std::vector<unsigned int> pIndices);
	~Model();

	void Render();
};


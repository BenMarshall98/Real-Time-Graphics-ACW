#pragma once

#include <DirectXMath.h>
#include <vector>
#include <d3d11.h>

class Model
{
	unsigned int mIndicesSize;

	ID3D11Buffer * mPositionBuffer;
	ID3D11Buffer * mNormalBuffer;
	ID3D11Buffer * mTexCoordBuffer;
	ID3D11Buffer * mTangentBuffer;
	ID3D11Buffer * mBiTangentBuffer;
	ID3D11Buffer * mIndicesBuffer;

public:
	Model(const std::vector<DirectX::XMFLOAT3> & pPositions, const std::vector<DirectX::XMFLOAT3> & pNormals,
		const std::vector<DirectX::XMFLOAT2> & pTexCoords, const std::vector<DirectX::XMFLOAT3> & pTangents,
		const std::vector<DirectX::XMFLOAT3> & pBiTangents, const std::vector<WORD> & pIndices);
	~Model();

	void Render();
};


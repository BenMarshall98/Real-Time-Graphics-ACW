#pragma once

#include <DirectXMath.h>
#include <vector>
#include <d3d11.h>

class Model
{
	unsigned int mIndicesSize = 0u;

	ID3D11Buffer * mPositionBuffer = nullptr;
	ID3D11Buffer * mNormalBuffer = nullptr;
	ID3D11Buffer * mTexCoordBuffer = nullptr;
	ID3D11Buffer * mTangentBuffer = nullptr;
	ID3D11Buffer * mBiTangentBuffer = nullptr;
	ID3D11Buffer * mIndicesBuffer = nullptr;

public:
	Model() = default;
	bool loadModel(const std::vector<DirectX::XMFLOAT3> & pPositions, const std::vector<DirectX::XMFLOAT3> & pNormals,
		const std::vector<DirectX::XMFLOAT2> & pTexCoords, const std::vector<DirectX::XMFLOAT3> & pTangents,
		const std::vector<DirectX::XMFLOAT3> & pBiTangents, const std::vector<WORD> & pIndices);
	~Model();

	Model(const Model&) = delete;
	Model(Model &&) = delete;
	Model & operator= (const Model &) = delete;
	Model & operator= (Model &&) = delete;

	void render();
};


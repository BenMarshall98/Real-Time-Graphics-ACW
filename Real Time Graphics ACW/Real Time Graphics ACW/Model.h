#pragma once

#include <DirectXMath.h>
#include <vector>
#include <d3d11.h>
#include <wrl/client.h>

class Model
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> mPositionBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mNormalBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mTexCoordBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mTangentBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mBiTangentBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mIndicesBuffer = nullptr;
	
	unsigned int mIndicesSize = 0u;

public:
	Model() = default;
	~Model() = default;

	Model(const Model&) = delete;
	Model(Model &&) = delete;
	Model & operator= (const Model &) = delete;
	Model & operator= (Model &&) = delete;

	bool loadModel(const std::vector<DirectX::XMFLOAT3> & pPositions, const std::vector<DirectX::XMFLOAT3> & pNormals,
		const std::vector<DirectX::XMFLOAT2> & pTexCoords, const std::vector<DirectX::XMFLOAT3> & pTangents,
		const std::vector<DirectX::XMFLOAT3> & pBiTangents, const std::vector<WORD> & pIndices);

	void render(bool pTessellated = false);
};


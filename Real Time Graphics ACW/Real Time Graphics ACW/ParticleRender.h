#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <vector>

class ParticleRender
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> mPositionBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mTexCoordBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mParticlePositionBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mParticleTimeBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mIndicesBuffer = nullptr;

	unsigned int mIndicesSize = 0u;
	
public:
	ParticleRender() = default;
	~ParticleRender() = default;

	ParticleRender(const ParticleRender&) = delete;
	ParticleRender(ParticleRender &&) = delete;
	ParticleRender & operator= (const ParticleRender &) = delete;
	ParticleRender & operator= (ParticleRender &&) = delete;

	bool loadParticles();
	void render(const std::vector<DirectX::XMFLOAT3> & pParticlePositions, const std::vector<float> & pParticleTimes) const;
};


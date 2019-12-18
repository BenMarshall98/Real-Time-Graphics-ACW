#include "ParticleManager.h"
#include "ResourceManager.h"

ParticleManager * ParticleManager::mInstance = nullptr;

ParticleManager::ParticleManager()
{
	mParticleRender = std::make_unique<ParticleRender>();
	
	if (!mParticleRender->loadParticles())
	{
		mParticleRender.reset();
	}

	mParticleShader = ResourceManager::instance()->loadParticleShader("ParticleVertexShader.hlsl", "ParticleFragmentShader.hlsl");
}

void ParticleManager::addParticles(const std::vector<Particle>& pParticles)
{
	mParticles.insert(mParticles.end(), pParticles.begin(), pParticles.end());
}

void ParticleManager::update(float pDt)
{
	for (auto & particle : mParticles)
	{
		particle.calculatePhysics(pDt);
	}
}

void ParticleManager::render()
{
	const std::vector<DirectX::XMFLOAT3> mPositions =
	{
		DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT3(4.0f, 4.0f, 4.0f),
		DirectX::XMFLOAT3(-3.0f, 2.0f, 4.0f),
		DirectX::XMFLOAT3(2.0f, 4.5f, 2.5f)
	};

	const std::vector<float> mTimes =
	{
		0.0f, 0.0f, 0.0f, 0.0f
	};

	mParticleShader->useShader();
	mParticleRender->render(mPositions, mTimes);
}
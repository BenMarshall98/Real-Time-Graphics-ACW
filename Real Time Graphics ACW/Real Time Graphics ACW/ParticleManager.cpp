#include "ParticleManager.h"
#include "ResourceManager.h"
#include "Shape.h"

ParticleManager * ParticleManager::mInstance = nullptr;

ParticleManager::ParticleManager()
{
	mParticleRenders.emplace_back(std::make_unique<ParticleRender>());
	
	if (!mParticleRenders[0]->loadParticles())
	{
		mParticleRenders.erase(mParticleRenders.begin() + 0);
	}

	ResourceManager::instance()->loadParticleShader(mParticleShader, "ParticleVertexShader.hlsl", "ParticleFragmentShader.hlsl");
}

void ParticleManager::addParticles(std::vector<Particle>& pParticles)
{
	for (auto& particle : pParticles)
	{
		particle.setTime(1.0f);
	}
	
	mParticles.insert(mParticles.end(), pParticles.begin(), pParticles.end());

	if (mParticles.size() > max_particles * mParticleRenders.size())
	{
		mParticleRenders.emplace_back(std::make_unique<ParticleRender>());

		if (!mParticleRenders[mParticleRenders.size() - 1]->loadParticles())
		{
			mParticleRenders.pop_back();
		}
	}
}

void ParticleManager::update(const float pDt)
{
	for (auto & particle : mParticles)
	{
		particle.calculatePhysics(pDt);
	}

	std::vector<std::shared_ptr<Shape>> staticObjects;
	std::vector<std::shared_ptr<Shape>> dynamicObjects;

	
}

void ParticleManager::render()
{
	mParticleShader->useShader();

	std::vector<DirectX::XMFLOAT3> positions;
	std::vector<float> times;

	positions.reserve(1000);
	times.reserve(1000);

	for (auto i = 0u; i < mParticleRenders.size(); i ++)
	{
		positions.clear();
		times.clear();
		
		for (auto j = i * max_particles; j < (i + 1) * max_particles && j < mParticles.size(); j++)
		{
			DirectX::XMFLOAT3 currentPosition;
			mParticles[j].getCurrentPosition(currentPosition);
			
			positions.push_back(currentPosition);
			times.push_back(mParticles[j].getTime());
		}

		mParticleRenders[i]->render(positions, times);
	}
}
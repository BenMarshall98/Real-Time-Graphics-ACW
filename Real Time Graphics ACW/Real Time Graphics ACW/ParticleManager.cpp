#include "ParticleManager.h"

ParticleManager * ParticleManager::mInstance = nullptr;

ParticleManager::ParticleManager()
{
	mParticleRender = std::make_unique<ParticleRender>();
	
	if (mParticleRender->loadParticles())
	{
		mParticleRender.reset();
	}
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
	
}
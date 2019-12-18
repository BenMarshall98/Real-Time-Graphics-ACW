#include "ParticleManager.h"

ParticleManager * ParticleManager::mInstance = nullptr;

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

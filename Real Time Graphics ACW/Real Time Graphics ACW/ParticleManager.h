#pragma once

#include "Particle.h"
#include <vector>

class ParticleManager final
{
	static ParticleManager * mInstance;

	std::vector<Particle> mParticles;

	ParticleManager() = default;
	
public:
	~ParticleManager() = default;
	ParticleManager(const ParticleManager&) = delete;
	ParticleManager(ParticleManager &&) = delete;
	ParticleManager & operator= (const ParticleManager &) = delete;
	ParticleManager & operator= (ParticleManager &&) = delete;

	static ParticleManager * instance()
	{
		if (!mInstance)
		{
			mInstance = new ParticleManager();
		}
		return mInstance;
	}

	void addParticles(const std::vector<Particle> & pParticles);
	void update(float pDt);
};


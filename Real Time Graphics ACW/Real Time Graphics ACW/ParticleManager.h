#pragma once

#include "Particle.h"
#include <vector>
#include "ParticleRender.h"
#include <memory>

class ParticleManager final
{
	static ParticleManager * mInstance;
	
	std::vector<Particle> mParticles;
	std::unique_ptr<ParticleRender> mParticleRender;

	ParticleManager();
	
public:
	static const unsigned int max_particles = 1000;
	
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
	void render();
};


#pragma once

#include "Particle.h"
#include <vector>
#include "ParticleRender.h"
#include <memory>
#include "Shader.h"
#include "Texture.h"

struct Collision
{
	std::shared_ptr<Particle> mParticle;
	DirectX::XMFLOAT3 mNormal;
	float mTime;
};

class ParticleManager final
{
	static ParticleManager * mInstance;
	
	std::vector<std::shared_ptr<Particle>> mParticles;
	std::vector<std::unique_ptr<ParticleRender>> mParticleRenders;
	std::shared_ptr<Shader> mParticleShader;
	std::vector<Collision> mCollisions;
	std::shared_ptr<Texture> mTexture;

	ParticleManager();
	
public:
	static const unsigned int max_particles = 2000;
	
	~ParticleManager()
	{
		mInstance = nullptr;
	}
	
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

	void addParticles(std::vector<std::shared_ptr<Particle>> & pParticles);
	void update(float pDt);
	void render();

	void addCollision(const Collision & pCollision)
	{
		mCollisions.push_back(pCollision);
	}
};


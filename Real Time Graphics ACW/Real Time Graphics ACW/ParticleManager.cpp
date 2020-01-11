#include "ParticleManager.h"
#include "ResourceManager.h"
#include "Shape.h"
#include "ObjectManager.h"
#include <algorithm>
#include "DX11Render.h"

ParticleManager * ParticleManager::mInstance = nullptr;

ParticleManager::ParticleManager()
{
	mParticleRenders.emplace_back(std::make_unique<ParticleRender>());
	
	if (!mParticleRenders[0]->loadParticles())
	{
		mParticleRenders.erase(mParticleRenders.begin() + 0);
	}

	ResourceManager::instance()->loadParticleShader(mParticleShader, "ParticleVertexShader.hlsl", "ParticleFragmentShader.hlsl");
	ResourceManager::instance()->loadTexture(mTexture, "noise.dds");

	mTexture->useFragment(25);
}

void ParticleManager::addParticles(std::vector<std::shared_ptr<Particle>>& pParticles)
{
	for (const auto& particle : pParticles)
	{
		particle->setTime(1.0f);
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
	mCollisions.clear();
	mCollisions.shrink_to_fit();

	for (const auto & particle : mParticles)
	{
		particle->update();
		particle->calculatePhysics(pDt);
	}

	std::vector<std::shared_ptr<Shape>> objects;

	ObjectManager::instance()->getAllShapes(objects);

	for (auto i = 0u; i < objects.size(); i++)
	{
		for (const auto & particle : mParticles)
		{
			objects[i]->collideWith(particle);
		}
	}

	for (auto i = 0u; i < mCollisions.size(); i++)
	{
		auto tempStart = DirectX::XMFLOAT3();
		mCollisions[i].mParticle->getPreviousPosition(tempStart);

		auto tempEnd = DirectX::XMFLOAT3();
		mCollisions[i].mParticle->getCurrentPosition(tempEnd);

		const auto particleStart = DirectX::XMLoadFloat3(&tempStart);
		const auto particleEnd = DirectX::XMLoadFloat3(&tempEnd);

		const auto particleDirection = DirectX::XMVectorSubtract(particleEnd, particleStart);

		const auto normal = DirectX::XMLoadFloat3(&mCollisions[i].mNormal);

		const auto newDirection = DirectX::XMVectorSubtract(particleDirection, DirectX::XMVectorScale(normal, (1.0f + 0.8f) * DirectX::XMVectorGetX(DirectX::XMVector3Dot(particleDirection, normal))));

		const auto newPosition = DirectX::XMVectorAdd(DirectX::XMVectorLerp(particleStart, particleEnd, mCollisions[i].mTime), DirectX::XMVectorScale(newDirection, 1.0f - mCollisions[i].mTime));

		const auto speed = DirectX::XMVectorGetX(DirectX::XMVector3Length(newDirection));

		if (speed < 0.01f)
		{
			const auto it = std::find(mParticles.begin(), mParticles.end(), mCollisions[i].mParticle);

			if (it != mParticles.end())
			{
				mParticles.erase(it);
			}
		}

		auto newVelocity = DirectX::XMFLOAT3();
		auto newPos = DirectX::XMFLOAT3();

		DirectX::XMStoreFloat3(&newVelocity, newDirection);
		DirectX::XMStoreFloat3(&newPos, newPosition);

		mCollisions[i].mParticle->setVelocity(newVelocity);
		mCollisions[i].mParticle->setPosition(newPos);
	}
}

void ParticleManager::render()
{
	
	mParticleShader->useShader();

	std::vector<DirectX::XMFLOAT3> positions;
	std::vector<float> times;

	positions.reserve(1000);
	times.reserve(1000);

	Dx11Render::instance()->enableBlend();

	for (auto i = 0u; i < mParticleRenders.size(); i ++)
	{
		positions.clear();
		times.clear();
		
		for (auto j = i * max_particles; j < (i + 1) * max_particles && j < mParticles.size(); j++)
		{
			DirectX::XMFLOAT3 currentPosition;
			mParticles[j]->getCurrentPosition(currentPosition);
			
			positions.push_back(currentPosition);
			times.push_back(mParticles[j]->getTime());
		}

		mParticleRenders[i]->render(positions, times);
	}

	Dx11Render::instance()->disableBlend();
}
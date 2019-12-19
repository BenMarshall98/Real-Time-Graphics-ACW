#include "Sphere.h"
#include "ResourceManager.h"
#include <corecrt_math_defines.h>
#include "ParticleManager.h"

Sphere::Sphere(std::unique_ptr<TexturePack> pTexturePack, std::unique_ptr<Material> pMaterial) :
	Shape(ResourceManager::instance()->loadModel("sphere.obj"), std::move(pTexturePack), std::move(pMaterial))
{
}

Sphere::Sphere() :
	Shape(ResourceManager::instance()->loadModel("sphere.obj"), nullptr, nullptr)
{
	
}

void Sphere::explode()
{
	std::vector<DirectX::XMFLOAT3> positions;

	const auto segments = 30;
	const auto angle = static_cast<float>(M_PI) / segments;

	positions.emplace_back(DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f));
	positions.emplace_back(DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f));

	for (auto i = 1; i < segments; i++)
	{
		for (auto j = 0; j < segments * 2; j++)
		{
			const auto x = cos(j * angle) * sin(i * angle);
			const auto y = sin(j * angle) * sin(i * angle);
			const auto z = cos(i * angle);

			positions.emplace_back(x, y, z);
		}
	}

	const auto matrix = XMLoadFloat4x4(&mCurrentMatrix);
	const auto center = XMVector3Transform(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), matrix);
	
	std::vector<Particle> particles;
	particles.reserve(positions.size());

	for (int i = 0; i < positions.size(); i++)
	{
		auto tempPosition = XMLoadFloat3(&positions[i]);
		tempPosition = XMVector3Transform(tempPosition, matrix);

		const auto tempVelocity = DirectX::XMVectorScale(DirectX::XMVectorSubtract(tempPosition, center), 10.0f);

		auto position = DirectX::XMFLOAT3();
		auto velocity = DirectX::XMFLOAT3();

		XMStoreFloat3(&position, tempPosition);
		XMStoreFloat3(&velocity, tempVelocity);
		
		particles.emplace_back(
			position, velocity
		);
	}

	ParticleManager::instance()->addParticles(particles);
}

void Sphere::collideWith(Particle pParticle)
{
	//TODO: Implement	
}
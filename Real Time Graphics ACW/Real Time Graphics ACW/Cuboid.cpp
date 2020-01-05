#include "Cuboid.h"
#include "ResourceManager.h"
#include "ParticleManager.h"

Cuboid::Cuboid(std::unique_ptr<TexturePack> pTexturePack, std::unique_ptr<Material> pMaterial) :
	Shape(ResourceManager::instance()->loadModel("cube.obj"), std::move(pTexturePack), std::move(pMaterial))
{
}

Cuboid::Cuboid() :
	Shape(ResourceManager::instance()->loadModel("cube.obj"), nullptr, nullptr)
{
}

void Cuboid::explode()
{
	const auto matrix = DirectX::XMLoadFloat4x4(&mCurrentMatrix);

	auto center = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	auto xDir = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	auto yDir = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	auto zDir = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);

	center = DirectX::XMVector3Transform(center, matrix);
	xDir = DirectX::XMVectorSubtract(DirectX::XMVector3Transform(xDir, matrix), center);
	yDir = DirectX::XMVectorSubtract(DirectX::XMVector3Transform(yDir, matrix), center);
	zDir = DirectX::XMVectorSubtract(DirectX::XMVector3Transform(zDir, matrix), center);

	std::vector<DirectX::XMFLOAT3> positions;

	std::vector<DirectX::XMFLOAT3> directions =
	{
		DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),
		DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)
	};

	for (auto i = 0; i < directions.size(); i++)
	{
		for (auto j = -10; j < 10; j++)
		{
			for (auto k = -10; k < 10; k++)
			{
				auto position = DirectX::XMVectorAdd(DirectX::XMVectorScale(xDir, directions[i].x),
					DirectX::XMVectorAdd(DirectX::XMVectorScale(yDir, directions[i].y),
						DirectX::XMVectorAdd(DirectX::XMVectorScale(zDir, directions[i].z), center)));

				bool jUsed = false;

				if (directions[i].x == 0.0f)
				{
					position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(xDir, j * 0.1f));
					jUsed = true;
				}

				if (directions[i].y == 0.0f)
				{
					if (jUsed)
					{
						position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(yDir, k * 0.1f));
					}
					else
					{
						position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(yDir, j * 0.1f));
					}
				}

				if (directions[i].z == 0.0f)
				{
					position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(zDir, k * 0.1f));
				}

				auto tempPosition = DirectX::XMFLOAT3();

				DirectX::XMStoreFloat3(&tempPosition, position);

				positions.push_back(tempPosition);
			}
		}
	}

	std::vector<Particle> particles;
	particles.reserve(positions.size());

	for (int i = 0; i < positions.size(); i++)
	{
		auto tempPosition = XMLoadFloat3(&positions[i]);
		auto length = DirectX::XMVectorGetX(DirectX::XMVector3Length(tempPosition));

		const auto tempVelocity = DirectX::XMVectorScale(DirectX::XMVectorSubtract(tempPosition, center), 10.0f / length);

		auto velocity = DirectX::XMFLOAT3();

		XMStoreFloat3(&velocity, tempVelocity);

		particles.emplace_back(positions[i], velocity);
	}

	ParticleManager::instance()->addParticles(particles);
}

void Cuboid::collideWith(Particle pParticle)
{
	//TODO: Implement
}
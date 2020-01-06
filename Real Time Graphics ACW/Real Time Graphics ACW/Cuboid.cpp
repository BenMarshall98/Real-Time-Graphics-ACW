#include "Cuboid.h"
#include "ResourceManager.h"
#include "ParticleManager.h"
#include <cmath>
#include <algorithm>

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
	const auto matrix = DirectX::XMLoadFloat4x4(&mCurrentMatrix);

	auto center = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	auto xDir = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	auto yDir = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	auto zDir = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);

	center = DirectX::XMVector3Transform(center, matrix);
	const auto xLength = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMVector3Transform(xDir, matrix), center)));
	const auto yLength = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMVector3Transform(yDir, matrix), center)));
	const auto zLength = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMVector3Transform(zDir, matrix), center)));

	const auto quat0 = DirectX::XMQuaternionRotationMatrix(DirectX::XMLoadFloat4x4(&mPreviousMatrix));
	const auto quat1 = DirectX::XMQuaternionRotationMatrix(DirectX::XMLoadFloat4x4(&mCurrentMatrix));
	const auto cubePos1 = center;

	center = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	const auto preMatrix = DirectX::XMLoadFloat4x4(&mPreviousMatrix);

	const auto cubePos0 = DirectX::XMVector3Transform(center, preMatrix);

	auto tempPos = pParticle.getPreviousPosition();
	const auto partPos0 = DirectX::XMLoadFloat3(&tempPos);

	tempPos = pParticle.getCurrentPosition();
	const auto partPos1 = DirectX::XMLoadFloat3(&tempPos);

	auto currentTime = 0.0f;
	auto timeStart = 0.0f;
	auto timeEnd = 1.0f;
	auto firstTime = true;
	auto counter = 0;

	while (true)
	{
		if (counter > 50)
		{
			return; //Collision not found in time
		}

		if (firstTime)
		{
			//Time = 0
			{
				const auto normal = DirectX::XMVector3Transform(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), DirectX::XMMatrixRotationQuaternion(quat0));
				const auto tangent = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), DirectX::XMMatrixRotationQuaternion(quat0));
				const auto biTangent = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), DirectX::XMMatrixRotationQuaternion(quat0));

				const auto tempPos = cubePos0;

				const auto tempParticlePos = partPos0;

				DirectX::XMVECTOR collisionPos;

				if (detectCollision(normal, tangent, biTangent, DirectX::XMFLOAT3(xLength, yLength, zLength), tempPos, tempParticlePos, collisionPos))
				{
					//TODO: Collision 
				}
			}

			//Time = 1
			{
				const auto normal = DirectX::XMVector3Transform(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), DirectX::XMMatrixRotationQuaternion(quat1));
				const auto tangent = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), DirectX::XMMatrixRotationQuaternion(quat1));
				const auto biTangent = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), DirectX::XMMatrixRotationQuaternion(quat1));

				const auto tempPos = cubePos1;

				const auto tempParticlePos = partPos1;

				DirectX::XMVECTOR collisionPos;

				if (!detectCollision(normal, tangent, biTangent, DirectX::XMFLOAT3(xLength, yLength, zLength), tempPos, tempParticlePos, collisionPos))
				{
					return;
				}

				currentTime = timeStart + 0.5f * (timeEnd - timeStart);
			}
		}
		else
		{
			const auto quat = DirectX::XMQuaternionSlerp(quat0, quat1, currentTime);
			
			const auto normal = DirectX::XMVector3Transform(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), DirectX::XMMatrixRotationQuaternion(quat));
			const auto tangent = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), DirectX::XMMatrixRotationQuaternion(quat));
			const auto biTangent = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), DirectX::XMMatrixRotationQuaternion(quat));

			const auto tempPos = DirectX::XMVectorLerp(cubePos0, cubePos1, currentTime);

			const auto tempParticlePos = DirectX::XMVectorLerp(partPos0, partPos1, currentTime);

			DirectX::XMVECTOR collisionPos;

			if (detectCollision(normal, tangent, biTangent, DirectX::XMFLOAT3(xLength, yLength, zLength), tempPos, tempParticlePos, collisionPos))
			{
				timeEnd = currentTime;
				currentTime = timeStart + 0.5f * (timeEnd - timeStart);
			}

			const auto dist = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(collisionPos, tempParticlePos)));

			if (dist < 0.05f)
			{
				//TODO: Collision
			}

			timeStart = currentTime;
			currentTime = timeStart + 0.5f * (timeEnd - timeStart);
		}

		counter++;
	}
}

bool Cuboid::detectCollision(const DirectX::XMVECTOR & pNormal, const DirectX::XMVECTOR & pTangent, const DirectX::XMVECTOR & pBiTangent, const DirectX::XMFLOAT3 & pLengths,
	const DirectX::XMVECTOR & pCubePos, const DirectX::XMVECTOR & pPartPos, DirectX::XMVECTOR & pCollisionPos)
{
	const auto d = DirectX::XMVectorSubtract(pPartPos, pCubePos);

	pCollisionPos = pCubePos;

	bool collision = true;

	{
		float dist = DirectX::XMVectorGetX(DirectX::XMVector3Dot(d, pNormal));

		if (dist > pLengths.x)
		{
			collision = false;
			dist = pLengths.x;
		}
		if (dist < -pLengths.x)
		{
			collision = false;
			dist = -pLengths.x;
		}

		pCollisionPos = DirectX::XMVectorAdd(pCollisionPos, DirectX::XMVectorScale(pNormal, dist));
	}

	{
		float dist = DirectX::XMVectorGetX(DirectX::XMVector3Dot(d, pTangent));

		if (dist > pLengths.y)
		{
			collision = false;
			dist = pLengths.y;
		}
		if (dist < -pLengths.y)
		{
			collision = false;
			dist = -pLengths.y;
		}

		pCollisionPos = DirectX::XMVectorAdd(pCollisionPos, DirectX::XMVectorScale(pTangent, dist));
	}

	{
		float dist = DirectX::XMVectorGetX(DirectX::XMVector3Dot(d, pBiTangent));

		if (dist > pLengths.z)
		{
			collision = false;
			dist = pLengths.z;
		}

		if (dist < -pLengths.z)
		{
			collision = false;
			dist = -pLengths.z;
		}

		pCollisionPos = DirectX::XMVectorAdd(pCollisionPos, DirectX::XMVectorScale(pBiTangent, dist));
	}

	return collision;
}

//TODO: Source: Simulation Coursework

void Cuboid::calculateNormal(const DirectX::XMVECTOR & pNormal, const DirectX::XMVECTOR & pTangent, const DirectX::XMVECTOR & pBiTangent, const DirectX::XMFLOAT3 & pLengths,
	const DirectX::XMVECTOR & pCubePos, const DirectX::XMVECTOR & pCollisionPos, DirectX::XMVECTOR & pCollisionNormal)
{
	const auto d = DirectX::XMVectorSubtract(pCollisionPos, pCubePos);

	const auto xDist = std::max(-pLengths.x, std::min(DirectX::XMVectorGetX(DirectX::XMVector3Dot(d, pNormal)), pLengths.x));
	const auto yDist = std::max(-pLengths.y, std::min(DirectX::XMVectorGetX(DirectX::XMVector3Dot(d, pTangent)), pLengths.y));
	const auto zDist = std::max(-pLengths.z, std::min(DirectX::XMVectorGetX(DirectX::XMVector3Dot(d, pBiTangent)), pLengths.z));
	const auto EPSILON = 0.0005f;

	if (xDist < pLengths.x + EPSILON && xDist > pLengths.x - EPSILON &&
		yDist < pLengths.y + EPSILON && yDist > pLengths.y - EPSILON &&
		zDist < pLengths.z + EPSILON && zDist > pLengths.z - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorAdd(DirectX::XMVectorAdd(pNormal, pTangent), pBiTangent);
	}
	else if (xDist < pLengths.x + EPSILON && xDist > pLengths.x - EPSILON &&
		yDist < pLengths.y + EPSILON && yDist > pLengths.y - EPSILON &&
		zDist < -pLengths.z + EPSILON && zDist > -pLengths.z - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorSubtract(DirectX::XMVectorAdd(pNormal, pTangent), pBiTangent);
	}
	else if (xDist < pLengths.x + EPSILON && xDist > pLengths.x - EPSILON &&
		yDist < -pLengths.y + EPSILON && yDist > -pLengths.y - EPSILON &&
		zDist < pLengths.z + EPSILON && zDist > pLengths.z - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorAdd(DirectX::XMVectorSubtract(pNormal, pTangent), pBiTangent);
	}
	else if (xDist < pLengths.x + EPSILON && xDist > pLengths.x - EPSILON &&
		yDist < -pLengths.y + EPSILON && yDist > -pLengths.y - EPSILON &&
		zDist < -pLengths.z + EPSILON && zDist > -pLengths.z - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorSubtract(DirectX::XMVectorSubtract(pNormal, pTangent), pBiTangent);
	}
	else if (xDist < -pLengths.x + EPSILON && xDist > -pLengths.x - EPSILON &&
		yDist < pLengths.y + EPSILON && yDist > pLengths.y - EPSILON &&
		zDist < pLengths.z + EPSILON && zDist > pLengths.z - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorAdd(DirectX::XMVectorAdd(DirectX::XMVectorScale(pNormal, -1.0f), pTangent), pBiTangent);
	}
	else if (xDist < -pLengths.x + EPSILON && xDist > -pLengths.x - EPSILON &&
		yDist < pLengths.y + EPSILON && yDist > pLengths.y - EPSILON &&
		zDist < -pLengths.z + EPSILON && zDist > -pLengths.z - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorSubtract(DirectX::XMVectorAdd(DirectX::XMVectorScale(pNormal, -1.0f), pTangent), pBiTangent);
	}
	else if (xDist < -pLengths.x + EPSILON && xDist > -pLengths.x - EPSILON &&
		yDist < -pLengths.y + EPSILON && yDist > -pLengths.y - EPSILON &&
		zDist < pLengths.z + EPSILON && zDist > pLengths.z - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorAdd(DirectX::XMVectorSubtract(DirectX::XMVectorScale(pNormal, -1.0f), pTangent), pBiTangent);
	}
	else if (xDist < -pLengths.x + EPSILON && xDist > -pLengths.x - EPSILON &&
		yDist < -pLengths.y + EPSILON && yDist > -pLengths.y - EPSILON &&
		zDist < -pLengths.z + EPSILON && zDist > -pLengths.z - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorSubtract(DirectX::XMVectorSubtract(DirectX::XMVectorScale(pNormal, -1.0f), pTangent), pBiTangent);
	}
	else if (xDist < pLengths.x + EPSILON && xDist > pLengths.x - EPSILON &&
		yDist < pLengths.y + EPSILON && yDist > pLengths.y - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorAdd(pNormal, pTangent);
	}
	else if (xDist < pLengths.x + EPSILON && xDist > pLengths.x - EPSILON &&
		yDist < -pLengths.y + EPSILON && yDist > -pLengths.y - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorSubtract(pNormal, pTangent);
	}
	else if (xDist < pLengths.x + EPSILON && xDist > pLengths.x - EPSILON &&
		zDist < pLengths.z + EPSILON && zDist > pLengths.z - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorAdd(pNormal, pBiTangent);
	}
	else if (xDist < pLengths.x + EPSILON && xDist > pLengths.x - EPSILON &&
		zDist < -pLengths.z + EPSILON && zDist > -pLengths.z - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorSubtract(pNormal, pBiTangent);
	}
	else if (xDist < -pLengths.x + EPSILON && xDist > -pLengths.x - EPSILON &&
		yDist < pLengths.y + EPSILON && yDist > pLengths.y - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorAdd(DirectX::XMVectorScale(pNormal, -1.0f), pTangent);
	}
	else if (xDist < -pLengths.x + EPSILON && xDist > -pLengths.x - EPSILON &&
		yDist < -pLengths.y + EPSILON && yDist > -pLengths.y - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorSubtract(DirectX::XMVectorScale(pNormal, -1.0f), pTangent);
	}
	else if (xDist < -pLengths.x + EPSILON && xDist > -pLengths.x - EPSILON &&
		zDist < pLengths.z + EPSILON && zDist > pLengths.z - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorAdd(DirectX::XMVectorScale(pNormal, -1.0f), pBiTangent);
	}
	else if (xDist < -pLengths.x + EPSILON && xDist > -pLengths.x - EPSILON &&
		zDist < -pLengths.z + EPSILON && zDist > -pLengths.z - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorSubtract(DirectX::XMVectorScale(pNormal, -1.0f), pBiTangent);
	}
	else if (yDist < pLengths.y + EPSILON && yDist > pLengths.y - EPSILON &&
		zDist < pLengths.z + EPSILON && zDist > pLengths.z - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorAdd(pTangent, pBiTangent);
	}
	else if (yDist < pLengths.y + EPSILON && yDist > pLengths.y - EPSILON &&
		zDist < -pLengths.z + EPSILON && zDist > -pLengths.z - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorSubtract(pTangent, pBiTangent);
	}
	else if (yDist < -pLengths.y + EPSILON && yDist > -pLengths.y - EPSILON &&
		zDist < pLengths.z + EPSILON && zDist > pLengths.z - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorAdd(DirectX::XMVectorScale(pTangent, -1.0f), pBiTangent);
	}
	else if (yDist < -pLengths.y + EPSILON && yDist > -pLengths.y - EPSILON &&
		zDist < -pLengths.z + EPSILON && zDist > -pLengths.z - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorSubtract(DirectX::XMVectorScale(pTangent, -1.0f), pBiTangent);
	}
	else if (xDist < pLengths.x + EPSILON && xDist > pLengths.x - EPSILON)
	{
		pCollisionNormal = pNormal;
	}
	else if (xDist < -pLengths.x + EPSILON && xDist > -pLengths.x - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorScale(pNormal, -1.0f);
	}
	else if (yDist < pLengths.y + EPSILON && yDist > pLengths.y - EPSILON)
	{
		pCollisionNormal = pTangent;
	}
	else if (yDist < -pLengths.y + EPSILON && yDist > -pLengths.y - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorScale(pTangent, -1.0f);
	}
	else if (zDist < pLengths.z + EPSILON && zDist > pLengths.z - EPSILON)
	{
		pCollisionNormal = pBiTangent;
	}
	else if (zDist < -pLengths.z + EPSILON && zDist > -pLengths.z - EPSILON)
	{
		pCollisionNormal = DirectX::XMVectorScale(pBiTangent, -1.0f);
	}

	pCollisionNormal = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
}
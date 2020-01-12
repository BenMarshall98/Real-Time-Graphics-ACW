#include "Sphere.h"
#include "ResourceManager.h"
#include <corecrt_math_defines.h>
#include "ParticleManager.h"

Sphere::Sphere(std::unique_ptr<TexturePack> & pTexturePack, std::unique_ptr<Material> & pMaterial) :
	Shape(pTexturePack, pMaterial, Type::EXPLODE)
{
	std::shared_ptr<Model> model;
	getModel(model);
	ResourceManager::instance()->loadModel(model, "sphere.obj");
	setModel(model);
}

Sphere::Sphere() :
	Shape()
{
	setType(Type::EXPLODE);
	std::shared_ptr<Model> model;
	getModel(model);
	ResourceManager::instance()->loadModel(model, "sphere.obj");
	setModel(model);
}

Sphere::~Sphere() = default;

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

	auto currentMatrix = DirectX::XMFLOAT4X4();
	getCurrentMatrix(currentMatrix);
	
	const auto matrix = XMLoadFloat4x4(&currentMatrix);
	const auto center = XMVector3Transform(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), matrix);
	
	std::vector<std::shared_ptr<Particle>> particles;
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

		const auto particle = std::make_shared<Particle>(position, velocity);
		
		particles.emplace_back(
			particle
		);
	}

	ParticleManager::instance()->addParticles(particles);
}

//Based on Christer Ericson's Real Time Collision Detection

void Sphere::collideWith(const std::shared_ptr<Particle> & pParticle)
{
	//Assume sphere moves in a straight line between time 0 and 1
	auto previousMatrix = DirectX::XMFLOAT4X4();
	getPreviousMatrix(previousMatrix);
	auto currentMatrix = DirectX::XMFLOAT4X4();
	getCurrentMatrix(currentMatrix);
	
	const auto startMatrix = DirectX::XMLoadFloat4x4(&previousMatrix);
	const auto endMatrix = DirectX::XMLoadFloat4x4(&currentMatrix);

	auto sphereStart = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	auto sphereEnd = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	auto sphereNormal = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	sphereStart = DirectX::XMVector3Transform(sphereStart, startMatrix);
	sphereEnd = DirectX::XMVector3Transform(sphereEnd, endMatrix);
	sphereNormal = DirectX::XMVectorSubtract(DirectX::XMVector3Transform(sphereNormal, startMatrix), sphereStart);

	const auto radius = DirectX::XMVectorGetX(DirectX::XMVector3Length(sphereNormal));

	auto tempStart = DirectX::XMFLOAT3();
	pParticle->getPreviousPosition(tempStart);
	
	auto tempEnd = DirectX::XMFLOAT3();
	pParticle->getCurrentPosition(tempEnd);

	const auto particleStart = DirectX::XMLoadFloat3(&tempStart);
	const auto particleEnd = DirectX::XMLoadFloat3(&tempEnd);

	auto particleDirection = DirectX::XMVectorSubtract(particleEnd, particleStart);
	particleDirection = DirectX::XMVectorSubtract(particleDirection, DirectX::XMVectorSubtract(sphereEnd, sphereStart));

	const auto particleDirectionLength = DirectX::XMVectorGetX(DirectX::XMVector3Length(particleDirection));

	particleDirection = DirectX::XMVector3Normalize(particleDirection);

	const auto particleSphere = DirectX::XMVectorSubtract(particleStart, sphereStart);

	const auto b = DirectX::XMVectorGetX(DirectX::XMVector3Dot(particleSphere, particleDirection));
	const auto c = DirectX::XMVectorGetX(DirectX::XMVector3Dot(particleSphere, particleSphere)) - radius * radius;

	if (c > 0.0f && b > 0.0f)
	{
		return;
	}

	const auto discr = b * b - c;

	if (discr < 0.0f)
	{
		return;
	}

	const auto time = -b - sqrt(discr);

	if (time < 0.0f || time > particleDirectionLength)
	{
		return;
	}

	const auto tempPosition = DirectX::XMVectorAdd(particleStart, DirectX::XMVectorScale(particleDirection, time));
	const auto tempSpherePosition = DirectX::XMVectorLerp(sphereStart, sphereEnd, time / particleDirectionLength);

	const auto tempNormal = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(tempPosition, tempSpherePosition));

	Collision col;
	col.mParticle = pParticle;
	DirectX::XMStoreFloat3(&col.mNormal, tempNormal);
	col.mTime = time;

	ParticleManager::instance()->addCollision(col);
}
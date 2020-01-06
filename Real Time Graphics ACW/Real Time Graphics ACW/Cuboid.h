#pragma once

#include "Shape.h"
#include "IExplode.h"

class Cuboid final : public Shape, public IExplode
{
	void calculateNormal(const DirectX::XMVECTOR & pNormal, const DirectX::XMVECTOR & pTangent, const DirectX::XMVECTOR & pBiTangent, const DirectX::XMFLOAT3 & pLengths,
		const DirectX::XMVECTOR & pCubePos, const DirectX::XMVECTOR & pCollisionPos, DirectX::XMVECTOR & pCollisionNormal);

	bool detectCollision(const DirectX::XMVECTOR & pNormal, const DirectX::XMVECTOR & pTangent, const DirectX::XMVECTOR & pBiTangent, const DirectX::XMFLOAT3 & pLengths,
		const DirectX::XMVECTOR & pCubePos, const DirectX::XMVECTOR & pPartPos, DirectX::XMVECTOR & pCollisionPos);

public:
	Cuboid(std::unique_ptr<TexturePack> pTexturePack, std::unique_ptr<Material> pMaterial);
	Cuboid();
	~Cuboid() override = default;

	Cuboid(const Cuboid&) = delete;
	Cuboid(Cuboid &&) = delete;
	Cuboid & operator= (const Cuboid &) = delete;
	Cuboid & operator= (Cuboid &&) = delete;

	void explode() override;
	void collideWith(Particle pParticle) override;
};
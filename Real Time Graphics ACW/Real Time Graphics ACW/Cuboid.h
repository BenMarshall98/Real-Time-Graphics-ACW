#pragma once

#include "Shape.h"

class Cuboid final : public Shape
{
	void calculateNormal(const DirectX::XMVECTOR & pNormal, const DirectX::XMVECTOR & pTangent, const DirectX::XMVECTOR & pBiTangent, const DirectX::XMFLOAT3 & pLengths,
		const DirectX::XMVECTOR & pCubePos, const DirectX::XMVECTOR & pCollisionPos, DirectX::XMVECTOR & pCollisionNormal) const;

	bool detectCollision(const DirectX::XMVECTOR & pNormal, const DirectX::XMVECTOR & pTangent, const DirectX::XMVECTOR & pBiTangent, const DirectX::XMFLOAT3 & pLengths,
		const DirectX::XMVECTOR & pCubePos, const DirectX::XMVECTOR & pPartPos, DirectX::XMVECTOR & pCollisionPos) const;

public:
	Cuboid(std::unique_ptr<TexturePack> & pTexturePack, std::unique_ptr<Material> & pMaterial);
	Cuboid();
	~Cuboid();

	Cuboid(const Cuboid&) = delete;
	Cuboid(Cuboid &&) = delete;
	Cuboid & operator= (const Cuboid &) = delete;
	Cuboid & operator= (Cuboid &&) = delete;

	void explode() override;
	void collideWith(const std::shared_ptr<Particle> & pParticle) override;
};
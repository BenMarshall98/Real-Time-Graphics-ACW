#pragma once

#include "Shape.h"
#include "IExplode.h"

class Cuboid final : public Shape, IExplode
{
public:
	Cuboid(std::unique_ptr<TexturePack> pTexturePack, std::unique_ptr<Material> pMaterial);
	Cuboid() = default;
	~Cuboid() = default;

	void explode() override;
	void collideWith(Particle pParticle) override;
};
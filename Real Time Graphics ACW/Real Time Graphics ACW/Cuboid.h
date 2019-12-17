#pragma once

#include "Shape.h"
#include "IExplode.h"

class Cuboid final : public Shape, public IExplode
{
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
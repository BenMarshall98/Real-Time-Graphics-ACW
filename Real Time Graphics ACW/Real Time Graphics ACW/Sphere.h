#pragma once

#include "Shape.h"
#include "IExplode.h"

class Sphere final : public Shape, IExplode
{
public:
	Sphere(std::unique_ptr<TexturePack> pTexturePack, std::unique_ptr<Material> pMaterial);
	Sphere() = default;
	~Sphere() = default;

	Sphere(const Sphere&) = delete;
	Sphere(Sphere &&) = delete;
	Sphere & operator= (const Sphere &) = delete;
	Sphere & operator= (Sphere &&) = delete;

	void explode() override;
	void collideWith(Particle pParticle) override;
};


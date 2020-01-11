#pragma once

#include "Shape.h"

class Sphere final : public Shape
{
public:
	Sphere(std::unique_ptr<TexturePack> & pTexturePack, std::unique_ptr<Material> & pMaterial);
	Sphere();
	~Sphere();

	Sphere(const Sphere&) = delete;
	Sphere(Sphere &&) = delete;
	Sphere & operator= (const Sphere &) = delete;
	Sphere & operator= (Sphere &&) = delete;

	void explode() override;
	void collideWith(const std::shared_ptr<Particle> & pParticle) override;
};


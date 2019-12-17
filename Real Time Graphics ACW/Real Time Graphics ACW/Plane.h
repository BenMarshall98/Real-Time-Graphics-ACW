#pragma once

#include "Shape.h"

class Plane final : public Shape
{
public:
	Plane(std::unique_ptr<TexturePack> pTexturePack, std::unique_ptr<Material> pMaterial);
	Plane();
	~Plane() = default;

	Plane(const Plane&) = delete;
	Plane(Plane &&) = delete;
	Plane & operator= (const Plane &) = delete;
	Plane & operator= (Plane &&) = delete;

	void collideWith(Particle pParticle) override;
};
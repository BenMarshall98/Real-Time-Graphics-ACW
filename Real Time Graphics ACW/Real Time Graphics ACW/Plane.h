#pragma once

#include "Shape.h"

class Plane final : public Shape
{
public:
	Plane(std::unique_ptr<TexturePack> pTexturePack, std::unique_ptr<Material> pMaterial);
	Plane() = default;
	~Plane() = default;

	void collideWith(Particle pParticle) override;
};
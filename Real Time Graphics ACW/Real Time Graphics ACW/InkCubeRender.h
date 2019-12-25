#pragma once

#include "InkShapeRender.h"

class InkCubeRender final : public InkShapeRender
{
public:
	InkCubeRender();
	~InkCubeRender() = default;

	void update() override;
	void use() override;
};
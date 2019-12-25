#pragma once

#include "InkShapeRender.h"

class InkPlaneRender final : public InkShapeRender
{
	
public:
	InkPlaneRender();
	~InkPlaneRender() = default;

	void update() override;
	void use() override;
};


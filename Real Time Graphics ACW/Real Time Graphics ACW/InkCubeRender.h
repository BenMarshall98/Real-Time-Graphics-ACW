#pragma once

#include "InkShapeRender.h"

struct InkCubeBuffer
{
	DirectX::XMFLOAT4X4 mInkCubeView[6];
	DirectX::XMFLOAT4X4 mInkCubeProjection;
};

class InkCubeRender final : public InkShapeRender
{
public:
	InkCubeRender();
	~InkCubeRender() = default;

	void update(DirectX::XMFLOAT4X4 pMatrix) override;
	void use() override;
};
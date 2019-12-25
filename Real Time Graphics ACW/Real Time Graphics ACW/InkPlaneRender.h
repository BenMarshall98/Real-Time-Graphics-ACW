#pragma once

#include "InkShapeRender.h"

struct InkPlaneBuffer
{
	DirectX::XMFLOAT4X4 mInkPlaneView;
	DirectX::XMFLOAT4X4 mInkPlaneProjection;
};

class InkPlaneRender final : public InkShapeRender
{
	
public:
	InkPlaneRender();
	~InkPlaneRender() = default;

	void update(DirectX::XMFLOAT4X4 pMatrix) override;
	void use() override;
};


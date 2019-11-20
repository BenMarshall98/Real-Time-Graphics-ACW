#pragma once

#include "SceneGraphNode.h"

class RotationNode final : public SceneGraphNode
{
public:
	explicit RotationNode(const DirectX::XMFLOAT3 & pAxis, const float & pAngle);
	RotationNode() = default;
	~RotationNode() = default;

	void read(std::istream& pIn) override;
};


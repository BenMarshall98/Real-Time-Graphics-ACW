#pragma once

#include "SceneGraphNode.h"

class RotationNode final : public SceneGraphNode
{
public:
	explicit RotationNode(const DirectX::XMFLOAT3 & pAxis, const float & pAngle);
	RotationNode() = default;
	~RotationNode() = default;
	RotationNode(const RotationNode&) = delete;
	RotationNode(RotationNode &&) = delete;
	RotationNode & operator= (const RotationNode &) = delete;
	RotationNode & operator= (RotationNode &&) = delete;

	void read(std::istream& pIn) override;
};


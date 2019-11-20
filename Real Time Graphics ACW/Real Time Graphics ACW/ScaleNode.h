#pragma once

#include "SceneGraphNode.h"

class ScaleNode final : public SceneGraphNode
{
public:
	explicit ScaleNode(const float & pX, const float & pY, const float & pZ);
	ScaleNode() = default;
	~ScaleNode() = default;

	void read(std::istream& pIn) override;
};
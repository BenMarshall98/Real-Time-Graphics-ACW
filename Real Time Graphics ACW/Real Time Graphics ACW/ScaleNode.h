#pragma once

#include "SceneGraphNode.h"

class ScaleNode final : public SceneGraphNode
{
public:
	explicit ScaleNode(const float & pX, const float & pY, const float & pZ);
	ScaleNode() = default;
	~ScaleNode();

	ScaleNode(const ScaleNode&) = delete;
	ScaleNode(ScaleNode &&) = delete;
	ScaleNode & operator= (const ScaleNode &) = delete;
	ScaleNode & operator= (ScaleNode &&) = delete;

	void read(std::istream& pIn) override;
};
#pragma once

#include "SceneGraphNode.h"

class IdentityNode final : public SceneGraphNode
{
public:
	IdentityNode() = default;

	void read(std::istream & pIn) override;
};


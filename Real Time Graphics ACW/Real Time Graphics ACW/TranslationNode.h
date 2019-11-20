#pragma once

#include "SceneGraphNode.h"

class TranslationNode final : public SceneGraphNode
{
public:
	explicit TranslationNode(const float & pX, const float & pY, const float & pZ);
	TranslationNode() = default;
	~TranslationNode() = default;

	void read(std::istream& pIn) override;
};


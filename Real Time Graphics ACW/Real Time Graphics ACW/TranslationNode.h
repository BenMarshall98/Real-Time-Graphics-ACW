#pragma once

#include "SceneGraphNode.h"

class TranslationNode final : public SceneGraphNode
{
public:
	explicit TranslationNode(const float & pX, const float & pY, const float & pZ);
	TranslationNode() = default;
	~TranslationNode() = default;

	TranslationNode(const TranslationNode &) = delete;
	TranslationNode(TranslationNode &&) = delete;
	TranslationNode & operator= (const TranslationNode &) = delete;
	TranslationNode & operator= (TranslationNode &&) = delete;

	void read(std::istream& pIn) override;
};


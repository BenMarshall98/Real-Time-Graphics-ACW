#pragma once

#include "SceneGraphNode.h"
#include "VectorAnimation.h"

class AnimationNode final : public SceneGraphNode
{
	std::unique_ptr<VectorAnimation> mAnimation = nullptr;
	
public:
	explicit AnimationNode(VectorAnimation * pAnimation);
	AnimationNode() = default;
	~AnimationNode() = default;

	AnimationNode(const AnimationNode &) = delete;
	AnimationNode(AnimationNode &&) = delete;
	AnimationNode & operator= (const AnimationNode &) = delete;
	AnimationNode & operator= (AnimationNode &&) = delete;

	void read(std::istream& pIn) override;
	void update(DirectX::XMFLOAT4X4 pMatrix) override;
};


#pragma once

#include "SceneGraphNode.h"
#include "Animation.h"

class AnimationNode final : public SceneGraphNode
{
	std::unique_ptr<Animation> mAnimation = nullptr;
	
public:
	explicit AnimationNode(Animation * pAnimation);
	AnimationNode() = default;
	~AnimationNode() = default;

	AnimationNode(const AnimationNode &) = delete;
	AnimationNode(AnimationNode &&) = delete;
	AnimationNode & operator= (const AnimationNode &) = delete;
	AnimationNode & operator= (AnimationNode &&) = delete;

	void read(std::istream& pIn) override;
	void update(DirectX::XMFLOAT4X4 pMatrix) override;
};


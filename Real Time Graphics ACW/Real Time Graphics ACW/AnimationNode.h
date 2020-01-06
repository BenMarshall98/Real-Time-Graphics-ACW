#pragma once

#include "SceneGraphNode.h"
#include "Animation.h"

class AnimationNode final : public SceneGraphNode
{
	std::unique_ptr<Animation> mAnimation = nullptr;
	
public:
	explicit AnimationNode(std::unique_ptr<Animation> & pAnimation);
	AnimationNode() = default;
	~AnimationNode();

	AnimationNode(const AnimationNode &) = delete;
	AnimationNode(AnimationNode &&) = delete;
	AnimationNode & operator= (const AnimationNode &) = delete;
	AnimationNode & operator= (AnimationNode &&) = delete;

	void read(std::istream& pIn) override;
	void update(const DirectX::XMFLOAT4X4 & pFullMatrix, DirectX::XMFLOAT4X4 & pRotationMatrix) override;
};


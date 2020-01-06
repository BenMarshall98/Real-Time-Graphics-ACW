#include "AnimationNode.h"
#include <string>
#include "QuaternionAnimation.h"
#include "VectorAnimation.h"

AnimationNode::~AnimationNode() = default;

AnimationNode::AnimationNode(std::unique_ptr<Animation> & pAnimation) : mAnimation(std::move(pAnimation))
{
}

void AnimationNode::read(std::istream& pIn)
{
	std::string s, type;

	pIn >> s >> type;

	if (type == "Vector")
	{
		mAnimation = std::make_unique<VectorAnimation>();
	}
	else if (type == "Quaternion")
	{
		mAnimation = std::make_unique<QuaternionAnimation>();
	}

	mAnimation->read(pIn);
}

void AnimationNode::update(const DirectX::XMFLOAT4X4 & pMatrix, DirectX::XMFLOAT4X4 & pRotationMatrix)
{
	//TODO: update needs to pass in time
	const auto outMatrix = mAnimation->animate(0.016f);

	setMatrix(outMatrix);

	if (dynamic_cast<QuaternionAnimation *>(mAnimation.get()))
	{
		auto matrix = DirectX::XMFLOAT4X4();

		DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&pRotationMatrix), DirectX::XMLoadFloat4x4(&outMatrix)));

		SceneGraphNode::update(pMatrix, matrix);
	}
	else
	{
		SceneGraphNode::update(pMatrix, pRotationMatrix);
	}
}

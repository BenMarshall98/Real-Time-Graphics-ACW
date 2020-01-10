#include "AnimationNode.h"
#include <string>
#include "QuaternionAnimation.h"
#include "VectorAnimation.h"
#include "Game.h"

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

void AnimationNode::update(const DirectX::XMFLOAT4X4 & pMatrix, const DirectX::XMFLOAT4X4 & pRotationMatrix)
{

	auto fullMatrix = DirectX::XMFLOAT4X4();
	auto rotationMatrix = DirectX::XMFLOAT4X4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	
	mAnimation->animate(Game::mDt, fullMatrix, rotationMatrix);

	setMatrix(fullMatrix);

	auto matrix = DirectX::XMFLOAT4X4();

	DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&pRotationMatrix), DirectX::XMLoadFloat4x4(&rotationMatrix)));

	SceneGraphNode::update(pMatrix, matrix);
}

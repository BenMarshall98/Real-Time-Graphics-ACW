#include "AnimationNode.h"
#include <string>
#include "QuaternionAnimation.h"
#include "VectorAnimation.h"

AnimationNode::AnimationNode(Animation* pAnimation) : mAnimation(pAnimation)
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

void AnimationNode::update(DirectX::XMFLOAT4X4 pMatrix)
{
	//TODO: update needs to pass in time
	mMatrix = mAnimation->animate(0.016f);
	SceneGraphNode::update(pMatrix);
}

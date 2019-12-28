#include "AnimationNode.h"
#include <string>

AnimationNode::AnimationNode(VectorAnimation* pAnimation) : mAnimation(pAnimation)
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

	pIn >> mAnimation;
}

void AnimationNode::update(DirectX::XMFLOAT4X4 pMatrix)
{
	//TODO: update needs to pass in time
	mMatrix = mAnimation->animate(0.016f);
	SceneGraphNode::update(pMatrix);
}

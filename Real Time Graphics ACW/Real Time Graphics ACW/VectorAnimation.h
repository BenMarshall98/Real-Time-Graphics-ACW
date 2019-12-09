#pragma once
#include <DirectXMath.h>
#include <vector>
#include <iostream>

struct VectorNode
{
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mTangent;
	float mTime;
};

class VectorAnimation
{
	std::vector<VectorNode> mNodes;
	float mEndTime = 0.0f;
	float mCurrentTime = 0.0f;
	float mCurrentNode = 0;
	
public:
	VectorAnimation(std::vector<VectorNode> pNodes, float pEndTime);
	VectorAnimation();
	~VectorAnimation();

	void addNode(VectorNode pNode)
	{
		mNodes.push_back(pNode);
	}

	void setEndTime(float pEndTime)
	{
		mEndTime = pEndTime;
	}

	void calculateTangents();
	
	DirectX::XMFLOAT4X4 animate(const float deltaTime);
};

std::istream& operator>>(std::istream& pIn, VectorAnimation & pNode);
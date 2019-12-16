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
	VectorAnimation() = default;
	~VectorAnimation() = default;

	VectorAnimation(const VectorAnimation &) = delete;
	VectorAnimation(VectorAnimation &&) = delete;
	VectorAnimation & operator= (const VectorAnimation &) = delete;
	VectorAnimation & operator= (VectorAnimation &&) = delete;

	void addNode(const VectorNode pNode)
	{
		mNodes.push_back(pNode);
	}

	void setEndTime(const float pEndTime)
	{
		mEndTime = pEndTime;
	}

	void calculateTangents();
	
	DirectX::XMFLOAT4X4 animate(float pDeltaTime);
};

std::istream& operator>>(std::istream& pIn, VectorAnimation & pNode);
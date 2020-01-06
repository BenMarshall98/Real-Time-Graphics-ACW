#pragma once
#include <DirectXMath.h>
#include <vector>
#include <iostream>
#include "Animation.h"

struct VectorNode
{
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mTangent;
	float mTime;
};

class VectorAnimation final : public Animation
{
	std::vector<VectorNode> mNodes;

	void addNode(const VectorNode pNode)
	{
		mNodes.push_back(pNode);
	}
	
public:
	VectorAnimation(std::vector<VectorNode> pNodes, float pEndTime);
	VectorAnimation() = default;
	~VectorAnimation();

	VectorAnimation(const VectorAnimation &) = delete;
	VectorAnimation(VectorAnimation &&) = delete;
	VectorAnimation & operator= (const VectorAnimation &) = delete;
	VectorAnimation & operator= (VectorAnimation &&) = delete;

	void calculateTangents() override;
	void read(std::istream& pIn) override;
	
	void animate(float pDeltaTime, DirectX::XMFLOAT4X4 & pFullMatrix, DirectX::XMFLOAT4X4 & pRotationMatrix) override;
};
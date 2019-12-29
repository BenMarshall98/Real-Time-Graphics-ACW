#pragma once

#include "Animation.h"
#include <vector>

struct QuaternionNode
{
	DirectX::XMFLOAT4 mQuaternion0;
	DirectX::XMFLOAT4 mQuaternion1;
	DirectX::XMFLOAT4 mQuaternion2;
	DirectX::XMFLOAT4 mQuaternion3;
	float mTime;
};

class QuaternionAnimation final : public Animation
{
	std::vector<QuaternionNode> mNodes;

	void addNode(const QuaternionNode pNode)
	{
		mNodes.push_back(pNode);
	}
	
public:
	QuaternionAnimation(std::vector<QuaternionNode> pNodes, float pEndTime);
	QuaternionAnimation() = default;
	~QuaternionAnimation() = default;

	QuaternionAnimation(const QuaternionAnimation &) = delete;
	QuaternionAnimation(QuaternionAnimation &&) = delete;
	QuaternionAnimation & operator= (const QuaternionAnimation &) = delete;
	QuaternionAnimation & operator= (QuaternionAnimation &&) = delete;

	void calculateTangents() override;
	void read(std::istream& pIn) override;

	DirectX::XMFLOAT4X4 animate(float pDeltaTime) override;
};


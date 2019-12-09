#include "VectorAnimation.h"

VectorAnimation::VectorAnimation(std::vector<VectorNode> pNodes, float pEndTime)
	: mNodes(pNodes), mEndTime(pEndTime)
{
}

VectorAnimation::VectorAnimation()
{
}

VectorAnimation::~VectorAnimation()
{
}

//Uses a form of Catmull-Rom Spline
//https://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull%E2%80%93Rom_spline
void VectorAnimation::calculateTangents()
{
	if (mNodes.size() <= 2)
	{
		//Can not calculate tangents will instead treat as linear path
		return;
	}

	{
		const auto pos1 = XMLoadFloat3(&mNodes[mNodes.size() - 1].mPosition);
		const auto time1 = mEndTime - mNodes[mNodes.size() - 1].mTime;

		const auto pos2 = XMLoadFloat3(&mNodes[1].mPosition);
		const auto time2 = mNodes[1].mTime;

		const auto tang = DirectX::XMVectorScale(DirectX::XMVectorSubtract(pos2, pos1), 1.0f / (time2 - time1));

		XMStoreFloat3(&mNodes[0].mTangent, tang);
	}

	for (auto i = 1u; i < mNodes.size() - 1; i++)
	{
		const auto pos1 = XMLoadFloat3(&mNodes[i - 1].mPosition);
		const auto time1 = mNodes[i - 1].mTime;

		const auto pos2 = XMLoadFloat3(&mNodes[i + 1].mPosition);
		const auto time2 = mNodes[i + 1].mTime;

		const auto tang = DirectX::XMVectorScale(DirectX::XMVectorSubtract(pos2, pos1), 1.0f / (time2 - time1));

		XMStoreFloat3(&mNodes[i].mTangent, tang);
	}

	{
		const auto pos1 = XMLoadFloat3(&mNodes[mNodes.size() - 2].mPosition);
		const auto time1 = mNodes[mNodes.size() - 2].mTime;

		const auto pos2 = XMLoadFloat3(&mNodes[0].mPosition);
		const auto time2 = mEndTime;

		const auto tang = DirectX::XMVectorScale(DirectX::XMVectorSubtract(pos2, pos1), 1.0f / (time2 - time1));

		XMStoreFloat3(&mNodes[mNodes.size() - 1].mTangent, tang);
	}
}

DirectX::XMFLOAT4X4 VectorAnimation::animate(const float deltaTime)
{
	if (mNodes.empty())
	{
		return { 1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 };
	}

	if (mNodes.size() == 1)
	{
		const auto pos = mNodes[0].mPosition;
		DirectX::XMFLOAT4X4 result;
		XMStoreFloat4x4(&result, DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z));

		return result;
	}

	if (mNodes.size() == 2)
	{
		const auto lastTime = mCurrentTime;
		mCurrentTime += deltaTime;

		if (mCurrentTime >= mEndTime)
		{
			mCurrentNode = 0;
			mCurrentTime -= mEndTime;

			
		}
	}
}
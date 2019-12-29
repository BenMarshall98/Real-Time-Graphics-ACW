#include "VectorAnimation.h"
#include <utility>
#include <string>

VectorAnimation::VectorAnimation(std::vector<VectorNode> pNodes, const float pEndTime)
	: Animation(pEndTime), mNodes(std::move(pNodes))
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
		const auto time1 = mNodes[mNodes.size() - 1].mTime - mEndTime;

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

DirectX::XMFLOAT4X4 VectorAnimation::animate(const float pDeltaTime)
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
		mCurrentTime += pDeltaTime;

		if (mCurrentTime >= mEndTime)
		{
			mCurrentNode = 0;
			mCurrentTime -= mEndTime;
		}

		if (mCurrentTime >= mNodes[1].mTime)
		{
			mCurrentNode = 1;

			const auto pos0 = DirectX::XMLoadFloat3(&mNodes[1].mPosition);
			const auto time0 = mNodes[1].mTime;
			
			const auto pos1 = DirectX::XMLoadFloat3(&mNodes[0].mPosition);
			const auto time1 = mEndTime;

			const auto pos = DirectX::XMVectorAdd(pos0, DirectX::XMVectorScale(DirectX::XMVectorSubtract(pos1, pos0), (mCurrentTime - time0) / (time1 - time0)));
			DirectX::XMFLOAT4X4 result;
			DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixTranslationFromVector(pos));

			return result;
		}

		mCurrentNode = 0;

		const auto pos0 = DirectX::XMLoadFloat3(&mNodes[0].mPosition);
		const auto time0 = mNodes[0].mTime;

		const auto pos1 = DirectX::XMLoadFloat3(&mNodes[1].mPosition);
		const auto time1 = mNodes[1].mTime;

		const auto pos = DirectX::XMVectorAdd(pos0, DirectX::XMVectorScale(DirectX::XMVectorSubtract(pos1, pos0), (mCurrentTime - time0) / (time1 - time0)));
		DirectX::XMFLOAT4X4 result;
		DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixTranslationFromVector(pos));

		return result;
	}

	mCurrentTime += pDeltaTime;

	if (mCurrentTime >= mEndTime)
	{
		mCurrentNode = 0;
		mCurrentTime -= mEndTime;
	}

	for (auto i = mNodes.size() - 1; i >= 0; i--)
	{
		if (mNodes[i].mTime < mCurrentTime)
		{
			mCurrentNode = i;
			break;
		}
	}

	if (mCurrentNode == mNodes.size() - 1)
	{
		const auto pos0 = DirectX::XMLoadFloat3(&mNodes[mCurrentNode].mPosition);
		const auto tan0 = DirectX::XMLoadFloat3(&mNodes[mCurrentNode].mTangent);
		const auto time0 = mNodes[mCurrentNode].mTime;

		const auto pos1 = DirectX::XMLoadFloat3(&mNodes[0].mPosition);
		const auto tan1 = DirectX::XMLoadFloat3(&mNodes[0].mTangent);
		const auto time1 = mEndTime;
		
		const auto time = (mCurrentTime - time0) / (time1 - time0);
		const auto time2 = pow(time, 2);
		const auto time3 = pow(time, 3);

		const auto h0 = 2.0f * time3 - 3.0f * time2 + 1.0f;
		const auto h1 = -2.0f * time3 + 3.0f * time2;
		const auto h2 = time3 - 2.0f * time2 + time;
		const auto h3 = time3 - time2;

		const auto pos = DirectX::XMVectorAdd(DirectX::XMVectorAdd(DirectX::XMVectorAdd(DirectX::XMVectorScale(pos0, h0), DirectX::XMVectorScale(pos1, h1)), DirectX::XMVectorScale(tan0, h2)), DirectX::XMVectorScale(tan1, h3));
		DirectX::XMFLOAT4X4 result;
		DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixTranslationFromVector(pos));

		return result;
	}

	const auto pos0 = DirectX::XMLoadFloat3(&mNodes[mCurrentNode].mPosition);
	const auto tan0 = DirectX::XMLoadFloat3(&mNodes[mCurrentNode].mTangent);
	const auto time0 = mNodes[mCurrentNode].mTime;

	const auto pos1 = DirectX::XMLoadFloat3(&mNodes[mCurrentNode + 1].mPosition);
	const auto tan1 = DirectX::XMLoadFloat3(&mNodes[mCurrentNode + 1].mTangent);
	const auto time1 = mNodes[mCurrentNode + 1].mTime;

	const auto time = (mCurrentTime - time0) / (time1 - time0);
	const auto time2 = pow(time, 2);
	const auto time3 = pow(time, 3);

	const auto h0 = 2.0f * time3 - 3.0f * time2 + 1.0f;
	const auto h1 = -2.0f * time3 + 3.0f * time2;
	const auto h2 = time3 - 2.0f * time2 + time;
	const auto h3 = time3 - time2;

	const auto pos = DirectX::XMVectorAdd(DirectX::XMVectorAdd(DirectX::XMVectorAdd(DirectX::XMVectorScale(pos0, h0), DirectX::XMVectorScale(pos1, h1)), DirectX::XMVectorScale(tan0, h2)), DirectX::XMVectorScale(tan1, h3));
	DirectX::XMFLOAT4X4 result;
	DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixTranslationFromVector(pos));

	return result;
}

void VectorAnimation::read(std::istream& pIn)
{
	while (true)
	{
		std::string s;
		pIn >> s;

		if (s == "{")
		{
			char c;
			float x, y, z;
			pIn >> s >> x >> c >> y >> c >> z;
			
			float time;
			pIn >> s >> time;

			DirectX::XMFLOAT3 pos(x, y, z);

			VectorNode node =
			{
				pos,
				DirectX::XMFLOAT3(),
				time
			};

			addNode(node);

			pIn >> s;
		}
		else if (s == "EndTime:")
		{
			float endTime;
			pIn >> endTime;

			setEndTime(endTime);
			break;
		}
	}
	calculateTangents();
}
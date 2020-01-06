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
		const auto time1 = mNodes[mNodes.size() - 1].mTime - getEndTime();

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
		const auto time2 = getEndTime();

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
		auto currentTime = getCurrentTime();
		auto endTime = getEndTime();
		auto currentNode = getCurrentNode();
		
		currentTime += pDeltaTime;

		if (currentTime >= endTime)
		{
			currentNode = 0;
			currentTime -= endTime;
		}

		if (currentTime >= mNodes[1].mTime)
		{
			currentNode = 1;

			const auto pos0 = DirectX::XMLoadFloat3(&mNodes[1].mPosition);
			const auto time0 = mNodes[1].mTime;
			
			const auto pos1 = DirectX::XMLoadFloat3(&mNodes[0].mPosition);
			const auto time1 = endTime;

			const auto pos = DirectX::XMVectorAdd(pos0, DirectX::XMVectorScale(DirectX::XMVectorSubtract(pos1, pos0), (currentTime - time0) / (time1 - time0)));
			DirectX::XMFLOAT4X4 result;
			DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixTranslationFromVector(pos));

			setCurrentTime(currentTime);
			setCurrentNode(currentNode);
			
			return result;
		}

		currentNode = 0;

		const auto pos0 = DirectX::XMLoadFloat3(&mNodes[0].mPosition);
		const auto time0 = mNodes[0].mTime;

		const auto pos1 = DirectX::XMLoadFloat3(&mNodes[1].mPosition);
		const auto time1 = mNodes[1].mTime;

		const auto pos = DirectX::XMVectorAdd(pos0, DirectX::XMVectorScale(DirectX::XMVectorSubtract(pos1, pos0), (currentTime - time0) / (time1 - time0)));
		DirectX::XMFLOAT4X4 result;
		DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixTranslationFromVector(pos));

		setCurrentTime(currentTime);
		setCurrentNode(currentNode);
		
		return result;
	}

	auto currentTime = getCurrentTime();
	auto endTime = getEndTime();
	auto currentNode = getCurrentNode();

	currentTime += pDeltaTime;

	if (currentTime >= endTime)
	{
		currentNode = 0;
		currentTime -= endTime;
	}

	for (auto i = mNodes.size() - 1; i >= 0; i--)
	{
		if (mNodes[i].mTime < currentTime)
		{
			currentNode = i;
			break;
		}
	}

	if (currentNode == mNodes.size() - 1)
	{
		const auto pos0 = DirectX::XMLoadFloat3(&mNodes[currentNode].mPosition);
		const auto tan0 = DirectX::XMLoadFloat3(&mNodes[currentNode].mTangent);
		const auto time0 = mNodes[currentNode].mTime;

		const auto pos1 = DirectX::XMLoadFloat3(&mNodes[0].mPosition);
		const auto tan1 = DirectX::XMLoadFloat3(&mNodes[0].mTangent);
		const auto time1 = endTime;
		
		const auto time = (currentTime - time0) / (time1 - time0);
		const auto time2 = pow(time, 2);
		const auto time3 = pow(time, 3);

		const auto h0 = 2.0f * time3 - 3.0f * time2 + 1.0f;
		const auto h1 = -2.0f * time3 + 3.0f * time2;
		const auto h2 = time3 - 2.0f * time2 + time;
		const auto h3 = time3 - time2;

		const auto pos = DirectX::XMVectorAdd(DirectX::XMVectorAdd(DirectX::XMVectorAdd(DirectX::XMVectorScale(pos0, h0), DirectX::XMVectorScale(pos1, h1)), DirectX::XMVectorScale(tan0, h2)), DirectX::XMVectorScale(tan1, h3));
		DirectX::XMFLOAT4X4 result;
		DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixTranslationFromVector(pos));

		setCurrentTime(currentTime);
		setCurrentNode(currentNode);

		return result;
	}

	const auto pos0 = DirectX::XMLoadFloat3(&mNodes[currentNode].mPosition);
	const auto tan0 = DirectX::XMLoadFloat3(&mNodes[currentNode].mTangent);
	const auto time0 = mNodes[currentNode].mTime;

	const auto pos1 = DirectX::XMLoadFloat3(&mNodes[currentNode + 1].mPosition);
	const auto tan1 = DirectX::XMLoadFloat3(&mNodes[currentNode + 1].mTangent);
	const auto time1 = mNodes[currentNode + 1].mTime;

	const auto time = (currentTime - time0) / (time1 - time0);
	const auto time2 = pow(time, 2);
	const auto time3 = pow(time, 3);

	const auto h0 = 2.0f * time3 - 3.0f * time2 + 1.0f;
	const auto h1 = -2.0f * time3 + 3.0f * time2;
	const auto h2 = time3 - 2.0f * time2 + time;
	const auto h3 = time3 - time2;

	const auto pos = DirectX::XMVectorAdd(DirectX::XMVectorAdd(DirectX::XMVectorAdd(DirectX::XMVectorScale(pos0, h0), DirectX::XMVectorScale(pos1, h1)), DirectX::XMVectorScale(tan0, h2)), DirectX::XMVectorScale(tan1, h3));
	DirectX::XMFLOAT4X4 result;
	DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixTranslationFromVector(pos));

	setCurrentTime(currentTime);
	setCurrentNode(currentNode);

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
#include "QuaternionAnimation.h"
#include <string>
#include <Windows.h>

QuaternionAnimation::QuaternionAnimation(std::vector<QuaternionNode> pNodes, float pEndTime) :
	Animation(pEndTime), mNodes(pNodes)
{
}

void QuaternionAnimation::calculateTangents()
{
	if (mNodes.size() <= 2)
	{
		//Can not calculate tangents will instead treat as linear path
		return;
	}

	for (auto i = 0u; i < mNodes.size(); i++)
	{
		DirectX::XMFLOAT4 quaternions[4];

		int node = i - 1;

		for (auto j = 0u; j < 4; j++)
		{
			if (node == mNodes.size())
			{
				node = 0;
			}

			if (node < 0)
			{
				node = mNodes.size() - 1;
			}

			quaternions[j] = mNodes[node].mQuaternion0;
			node++;
		}

		const auto quaternion0 = XMLoadFloat4(&quaternions[0]);
		const auto quaternion1 = XMLoadFloat4(&quaternions[1]);
		const auto quaternion2 = XMLoadFloat4(&quaternions[2]);
		const auto quaternion3 = XMLoadFloat4(&quaternions[3]);

		DirectX::XMVECTOR quaternionA;
		DirectX::XMVECTOR quaternionB;
		DirectX::XMVECTOR quaternionC;

		DirectX::XMQuaternionSquadSetup(&quaternionA, &quaternionB, &quaternionC, quaternion0, quaternion1, quaternion2, quaternion3);

		XMStoreFloat4(&mNodes[i].mQuaternion1, quaternionA);
		XMStoreFloat4(&mNodes[i].mQuaternion2, quaternionB);
		XMStoreFloat4(&mNodes[i].mQuaternion3, quaternionC);
	}
}

DirectX::XMFLOAT4X4 QuaternionAnimation::animate(float pDeltaTime)
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
		const auto rotation = DirectX::XMLoadFloat4(&mNodes[0].mQuaternion0);
		DirectX::XMFLOAT4X4 result;
		XMStoreFloat4x4(&result, DirectX::XMMatrixRotationQuaternion(rotation));

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

			const auto rot0 = DirectX::XMLoadFloat4(&mNodes[1].mQuaternion0);
			const auto time0 = mNodes[1].mTime;

			const auto rot1 = DirectX::XMLoadFloat4(&mNodes[0].mQuaternion0);
			const auto time1 = mEndTime;

			const auto rot = DirectX::XMQuaternionSlerp(rot0, rot1, (mCurrentTime - time0) / (time1 - time0));
			DirectX::XMFLOAT4X4 result;
			DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixRotationQuaternion(rot));

			return result;
		}

		mCurrentNode = 0;

		const auto rot0 = DirectX::XMLoadFloat4(&mNodes[0].mQuaternion0);
		const auto time0 = mNodes[0].mTime;

		const auto rot1 = DirectX::XMLoadFloat4(&mNodes[1].mQuaternion0);
		const auto time1 = mNodes[1].mTime;

		const auto rot = DirectX::XMQuaternionSlerp(rot0, rot1, (mCurrentTime - time0) / (time1 - time0));
		DirectX::XMFLOAT4X4 result;
		DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixRotationQuaternion(rot));

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

	const auto quaternion0 = DirectX::XMLoadFloat4(&mNodes[mCurrentNode].mQuaternion0);
	const auto quaternion1 = DirectX::XMLoadFloat4(&mNodes[mCurrentNode].mQuaternion1);
	const auto quaternion2 = DirectX::XMLoadFloat4(&mNodes[mCurrentNode].mQuaternion2);
	const auto quaternion3 = DirectX::XMLoadFloat4(&mNodes[mCurrentNode].mQuaternion3);

	const auto time0 = mNodes[mCurrentNode].mTime;

	auto time1 = 0.0f;

	if (mCurrentNode == mNodes.size() - 1)
	{
		time1 = mEndTime;
	}
	else
	{
		time1 = mNodes[mCurrentNode + 1].mTime;
	}

	const auto rot = DirectX::XMQuaternionSquad(quaternion0, quaternion1, quaternion2, quaternion3, (mCurrentTime - time0) / (time1 - time0));

	DirectX::XMFLOAT4X4 result;
	DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixRotationQuaternion(rot));

	OutputDebugString(("Time: " + std::to_string(mCurrentTime) + ", Node: " + std::to_string(mCurrentNode) + "\n").c_str());

	return result;
}

void QuaternionAnimation::read(std::istream& pIn)
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

			float angle;
			pIn >> s >> angle;

			float time;
			pIn >> s >> time;

			DirectX::XMFLOAT3 dir(x, y, z);

			auto direction = DirectX::XMLoadFloat3(&dir);

			angle = DirectX::XMConvertToRadians(angle);

			direction = DirectX::XMQuaternionRotationAxis(direction, angle);

			QuaternionNode node;

			DirectX::XMStoreFloat4(&node.mQuaternion0, direction);
			node.mTime = time;

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
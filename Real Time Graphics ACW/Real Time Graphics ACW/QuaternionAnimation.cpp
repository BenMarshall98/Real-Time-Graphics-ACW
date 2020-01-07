#include "QuaternionAnimation.h"
#include <string>
#include <Windows.h>

QuaternionAnimation::QuaternionAnimation(const std::vector<QuaternionNode> & pNodes, const float pEndTime) :
	Animation(pEndTime), mNodes(pNodes)
{
}

QuaternionAnimation::~QuaternionAnimation() = default;

//https://docs.microsoft.com/en-us/previous-versions/windows/desktop/bb281657(v=vs.85)?redirectedfrom=MSDN

void QuaternionAnimation::calculateTangents()
{
	if (mNodes.size() <= 2)
	{
		//Can not calculate tangents will instead treat as linear path
		return;
	}

	for (auto i = 0u; i < mNodes.size(); i++)
	{
		std::vector<DirectX::XMFLOAT4> quaternions;

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

			quaternions.push_back(mNodes[node].mQuaternion0);
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

void QuaternionAnimation::animate(const float pDeltaTime, DirectX::XMFLOAT4X4 & pFullMatrix, DirectX::XMFLOAT4X4 & pRotationMatrix)
{
	if (mNodes.empty())
	{
		pFullMatrix = DirectX::XMFLOAT4X4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

		pRotationMatrix = pFullMatrix;
		return;
	}

	if (mNodes.size() == 1)
	{
		const auto rotation = DirectX::XMLoadFloat4(&mNodes[0].mQuaternion0);
		DirectX::XMFLOAT4X4 result;
		XMStoreFloat4x4(&result, DirectX::XMMatrixRotationQuaternion(rotation));

		pFullMatrix = result;
		pRotationMatrix = result;
		return;
	}

	if (mNodes.size() == 2)
	{
		auto currentTime = getCurrentTime();
		const auto endTime = getEndTime();
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

			const auto rot0 = DirectX::XMLoadFloat4(&mNodes[1].mQuaternion0);
			const auto time0 = mNodes[1].mTime;

			const auto rot1 = DirectX::XMLoadFloat4(&mNodes[0].mQuaternion0);
			const auto time1 = endTime;

			const auto rot = DirectX::XMQuaternionSlerp(rot0, rot1, (currentTime - time0) / (time1 - time0));
			DirectX::XMFLOAT4X4 result;
			DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixRotationQuaternion(rot));

			setCurrentTime(currentTime);
			setCurrentNode(currentNode);

			pFullMatrix = result;
			pRotationMatrix = result;
			return;
		}

		currentNode = 0;

		const auto rot0 = DirectX::XMLoadFloat4(&mNodes[0].mQuaternion0);
		const auto time0 = mNodes[0].mTime;

		const auto rot1 = DirectX::XMLoadFloat4(&mNodes[1].mQuaternion0);
		const auto time1 = mNodes[1].mTime;

		const auto rot = DirectX::XMQuaternionSlerp(rot0, rot1, (currentTime - time0) / (time1 - time0));
		DirectX::XMFLOAT4X4 result;
		DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixRotationQuaternion(rot));

		setCurrentTime(currentTime);
		setCurrentNode(currentNode);
		
		pFullMatrix = result;
		pRotationMatrix = result;
		return;
	}

	auto currentTime = getCurrentTime();
	const auto endTime = getEndTime();
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

	const auto quaternion0 = XMLoadFloat4(&mNodes[currentNode].mQuaternion0);
	const auto quaternion1 = XMLoadFloat4(&mNodes[currentNode].mQuaternion1);
	const auto quaternion2 = XMLoadFloat4(&mNodes[currentNode].mQuaternion2);
	const auto quaternion3 = XMLoadFloat4(&mNodes[currentNode].mQuaternion3);

	const auto time0 = mNodes[currentNode].mTime;

	auto time1 = 0.0f;

	if (currentNode == mNodes.size() - 1)
	{
		time1 = endTime;
	}
	else
	{
		time1 = mNodes[currentNode + 1].mTime;
	}

	const auto rot = DirectX::XMQuaternionSquad(quaternion0, quaternion1, quaternion2, quaternion3, (currentTime - time0) / (time1 - time0));

	DirectX::XMFLOAT4X4 result;
	XMStoreFloat4x4(&result, DirectX::XMMatrixRotationQuaternion(rot));

	setCurrentTime(currentTime);
	setCurrentNode(currentNode);

	pFullMatrix = result;
	pRotationMatrix = result;
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

			const DirectX::XMFLOAT3 dir(x, y, z);

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
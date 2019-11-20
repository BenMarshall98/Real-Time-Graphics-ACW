#include "RotationNode.h"
#include <string>

RotationNode::RotationNode(const DirectX::XMFLOAT3& pAxis, const float& pAngle)
{
	XMStoreFloat4x4(&mMatrix, DirectX::XMMatrixRotationAxis(XMLoadFloat3(&pAxis), DirectX::XMConvertToRadians(pAngle)));
}

void RotationNode::read(std::istream& pIn)
{
	std::string s;
	pIn >> s;

	char c;
	float x, y, z;

	pIn >> x >> c >> y >> c >> z;

	float angle;
	pIn >> s >> angle;

	DirectX::XMFLOAT3 axis(x, y, z);
	XMStoreFloat4x4(&mMatrix, DirectX::XMMatrixRotationAxis(XMLoadFloat3(&axis), DirectX::XMConvertToRadians(angle)));
}

#include "RotationNode.h"
#include <string>

RotationNode::RotationNode(const DirectX::XMFLOAT3& pAxis, const float& pAngle) :
	SceneGraphNode(DirectX::XMMatrixRotationAxis(XMLoadFloat3(&pAxis), DirectX::XMConvertToRadians(pAngle)))
{
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

	DirectX::XMFLOAT3 temp(x, y, z);
	mMatrix = DirectX::XMMatrixRotationAxis(XMLoadFloat3(&temp), DirectX::XMConvertToRadians(angle));
}

#include "RotationNode.h"
#include <string>

RotationNode::RotationNode(const DirectX::XMFLOAT3& pAxis, const float& pAngle)
{
	auto matrix = DirectX::XMFLOAT4X4();
	XMStoreFloat4x4(&matrix, DirectX::XMMatrixRotationAxis(XMLoadFloat3(&pAxis), DirectX::XMConvertToRadians(pAngle)));

	setMatrix(matrix);
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

	auto matrix = DirectX::XMFLOAT4X4();
	XMStoreFloat4x4(&matrix, DirectX::XMMatrixRotationAxis(XMLoadFloat3(&axis), DirectX::XMConvertToRadians(angle)));

	setMatrix(matrix);
}

void RotationNode::update(const DirectX::XMFLOAT4X4 & pFullMatrix, DirectX::XMFLOAT4X4 & pRotationMatrix)
{
	auto matrix = DirectX::XMFLOAT4X4();

	const auto mat = getMatrix();
	
	DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&pRotationMatrix), DirectX::XMLoadFloat4x4(&mat)));

	SceneGraphNode::update(pFullMatrix, matrix);
}
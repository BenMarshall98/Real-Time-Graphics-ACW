#include "ScaleNode.h"
#include <string>

ScaleNode::ScaleNode(const float & pX, const float & pY, const float & pZ)
{
	auto matrix = DirectX::XMFLOAT4X4();
	XMStoreFloat4x4(&matrix, DirectX::XMMatrixScaling(pX, pY, pZ));

	setMatrix(matrix);
}

ScaleNode::~ScaleNode() = default;

void ScaleNode::read(std::istream& pIn)
{
	std::string s;
	pIn >> s;

	char c;
	float x, y, z;

	pIn >> x >> c >> y >> c >> z;

	auto matrix = DirectX::XMFLOAT4X4();
	XMStoreFloat4x4(&matrix, DirectX::XMMatrixScaling(x, y, z));

	setMatrix(matrix);
}
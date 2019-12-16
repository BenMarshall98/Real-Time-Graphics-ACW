#include "ScaleNode.h"
#include <string>

ScaleNode::ScaleNode(const float & pX, const float & pY, const float & pZ)
{
	XMStoreFloat4x4(&mMatrix, DirectX::XMMatrixScaling(pX, pY, pZ));
}

void ScaleNode::read(std::istream& pIn)
{
	std::string s;
	pIn >> s;

	char c;
	float x, y, z;

	pIn >> x >> c >> y >> c >> z;

	XMStoreFloat4x4(&mMatrix, DirectX::XMMatrixScaling(x, y, z));
}
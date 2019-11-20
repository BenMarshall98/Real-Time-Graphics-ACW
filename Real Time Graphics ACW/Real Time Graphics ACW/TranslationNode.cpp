#include "TranslationNode.h"
#include <string>

TranslationNode::TranslationNode(const float& pX, const float& pY, const float& pZ)
{
	XMStoreFloat4x4(&mMatrix, DirectX::XMMatrixTranslation(pX, pY, pZ));
}

void TranslationNode::read(std::istream& pIn)
{
	std::string s;
	pIn >> s;

	char c;
	float x, y, z;

	pIn >> x >> c >> y >> c >> z;

	DirectX::XMFLOAT3 temp(x, y, z);
	XMStoreFloat4x4(&mMatrix, DirectX::XMMatrixTranslation(x, y, z));
}

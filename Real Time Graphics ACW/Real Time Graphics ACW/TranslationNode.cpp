#include "TranslationNode.h"
#include <string>

TranslationNode::TranslationNode(const float& pX, const float& pY, const float& pZ)
{
	auto matrix = DirectX::XMFLOAT4X4();
	XMStoreFloat4x4(&matrix, DirectX::XMMatrixTranslation(pX, pY, pZ));

	setMatrix(matrix);
}

void TranslationNode::read(std::istream& pIn)
{
	std::string s;
	pIn >> s;

	char c;
	float x, y, z;

	pIn >> x >> c >> y >> c >> z;

	DirectX::XMFLOAT3 temp(x, y, z);

	auto matrix = DirectX::XMFLOAT4X4();
	XMStoreFloat4x4(&matrix, DirectX::XMMatrixTranslation(x, y, z));

	setMatrix(matrix);
}

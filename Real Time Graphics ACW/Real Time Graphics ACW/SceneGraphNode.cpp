#include "SceneGraphNode.h"
#include <string>

SceneGraphNode::SceneGraphNode(const DirectX::XMFLOAT4X4& pMatrix) : mMatrix(pMatrix)
{
}

SceneGraphNode::SceneGraphNode() :
	mMatrix(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f)
{
}

SceneGraphNode::~SceneGraphNode()
{
	mChildren.clear();
}

void SceneGraphNode::update(const DirectX::XMFLOAT4X4 & pFullMatrix, const DirectX::XMFLOAT4X4 & pRotationMatrix)
{
	auto matrix = DirectX::XMFLOAT4X4();
	XMStoreFloat4x4(&matrix, XMMatrixMultiply(XMLoadFloat4x4(&mMatrix), XMLoadFloat4x4(&pFullMatrix)));

	for (const auto& child : mChildren)
	{
		child->update(matrix, pRotationMatrix);
	}
}
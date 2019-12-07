#include "SceneGraphNode.h"
#include <string>
#include "IdentityNode.h"
#include "TranslationNode.h"
#include "RotationNode.h"
#include "ScaleNode.h"
#include "ObjectNode.h"
#include "DirectionalLightnode.h"
#include "PointLightNode.h"
#include "SpotLightNode.h"

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

void SceneGraphNode::update(DirectX::XMFLOAT4X4 pMatrix)
{
	DirectX::XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&mMatrix), XMLoadFloat4x4(&pMatrix)));

	for (const auto& child : mChildren)
	{
		child->update(matrix);
	}
}

std::istream& operator>>(std::istream & pIn, SceneGraphNode & pNode)
{
	char c;
	pIn >> c;
	
	pNode.read(pIn);
	
	while(true)
	{
		std::string nodeType;
		pIn >> nodeType;

		std::shared_ptr<SceneGraphNode> child;

		if (nodeType == "Identity")
		{
			child = std::make_shared<IdentityNode>();
		}
		else if (nodeType == "Translate")
		{
			child = std::make_shared<TranslationNode>();
		}
		else if (nodeType == "Rotation")
		{
			child = std::make_shared<RotationNode>();
		}
		else if (nodeType == "Scale")
		{
			child = std::make_shared<ScaleNode>();
		}
		else if (nodeType == "Object")
		{
			child = std::make_shared<ObjectNode>();
		}
		else if (nodeType == "DirectionalLight")
		{
			child = std::make_shared<DirectionalLightNode>();
		}
		else if (nodeType == "PointLight")
		{
			child = std::make_shared<PointLightNode>();
		}
		else if (nodeType == "SpotLight")
		{
			child = std::make_shared<SpotLightNode>();
		}
		else if (nodeType == "}")
		{
			break;
		}

		if (child)
		{
			pIn >> *child;
			pNode.addChild(child);
		}
	}

	return pIn;
}

#include "SceneGraphNode.h"
#include <string>
#include "IdentityNode.h"
#include "TranslationNode.h"
#include "RotationNode.h"
#include "ScaleNode.h"
#include "ObjectNode.h"

SceneGraphNode::SceneGraphNode(const DirectX::XMMATRIX& pMatrix) : mMatrix(pMatrix)
{
}

SceneGraphNode::SceneGraphNode() : mMatrix(DirectX::XMMatrixIdentity())
{
}

SceneGraphNode::~SceneGraphNode()
{
	mChildren.clear();
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

		std::shared_ptr<SceneGraphNode> child = nullptr;

		if (nodeType == "Identity")
		{
			child = std::make_shared(new IdentityNode());
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

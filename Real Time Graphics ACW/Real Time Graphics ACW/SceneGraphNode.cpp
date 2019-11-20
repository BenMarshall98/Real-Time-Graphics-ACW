#include "SceneGraphNode.h"
#include <string>
#include "IdentityNode.h"
#include "TranslationNode.h"
#include "RotationNode.h"
#include "ScaleNode.h"
#include "ObjectNode.h"

SceneGraphNode::SceneGraphNode() : mMatrix(DirectX::XMMatrixIdentity())
{
}

SceneGraphNode::~SceneGraphNode()
{
}

std::istream & operator >> (std::istream & pIn, SceneGraphNode * pNode)
{
	pNode->read(pIn);
	
	while(true)
	{
		std::string nodeType;
		pIn >> nodeType;

		SceneGraphNode * child = nullptr;

		if (nodeType == "Identity")
		{
			child = new IdentityNode();
		}
		else if (nodeType == "Translate")
		{
			child = new TranslationNode();
		}
		else if (nodeType == "Rotation")
		{
			child = new RotationNode();
		}
		else if (nodeType == "Scale")
		{
			child = new ScaleNode();
		}
		else if (nodeType == "Object")
		{
			child = new ObjectNode();
		}
		else if (nodeType == "}")
		{
			break;
		}

		if (child)
		{
			pIn >> child;
			pNode->addChild(child);
		}
	}

	return pIn;
}

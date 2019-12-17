#include "ConfigLoader.h"
#include <string>
#include "IdentityNode.h"
#include "TranslationNode.h"
#include "RotationNode.h"
#include "ScaleNode.h"
#include "ShapeNode.h"
#include "DirectionalLightNode.h"
#include "PointLightNode.h"
#include "SpotLightNode.h"

void ConfigLoader::readScene(std::istream& pIn, std::unique_ptr<SceneGraphNode>& pNode)
{
	while(true)
	{
		std::string nodeType;
		pIn >> nodeType;

		std::unique_ptr<SceneGraphNode> child;

		if (nodeType == "Identity")
		{
			child = std::make_unique<IdentityNode>();
		}
		else if (nodeType == "Translate")
		{
			child = std::make_unique<TranslationNode>();
		}
		else if (nodeType == "Rotation")
		{
			child = std::make_unique<RotationNode>();
		}
		else if (nodeType == "Scale")
		{
			child = std::make_unique<ScaleNode>();
		}
		else if (nodeType == "Shape")
		{
			child = std::make_unique<ShapeNode>();
		}
		else if (nodeType == "DirectionalLight")
		{
			child = std::make_unique<DirectionalLightNode>();
		}
		else if (nodeType == "PointLight")
		{
			child = std::make_unique<PointLightNode>();
		}
		else if (nodeType == "SpotLight")
		{
			child = std::make_unique<SpotLightNode>();
		}
		else if (nodeType == "}")
		{
			return;
		}

		if (child)
		{
			char temp;
			pIn >> temp;
			
			child->read(pIn);
			readScene(pIn, child);
			pNode->addChild(std::move(child));
		}
	}
}

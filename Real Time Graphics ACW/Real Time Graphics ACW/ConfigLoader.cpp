#include "ConfigLoader.h"
#include <string>
#include "IdentityNode.h"
#include "TranslationNode.h"
#include "RotationNode.h"
#include "ScaleNode.h"
#include "AnimationNode.h"
#include "ShapeNode.h"
#include "DirectionalLightNode.h"
#include "PointLightNode.h"
#include "SpotLightNode.h"
#include "CameraManager.h"

void ConfigLoader::readScene(std::istream& pIn, const std::unique_ptr<SceneGraphNode> & pNode)
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
		else if (nodeType == "Animation")
		{
			child = std::make_unique<AnimationNode>();
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
			pNode->addChild(child);
		}
	}
}

void ConfigLoader::readCameras(std::istream& pIn)
{
	{
		std::string node;
		char c;

		pIn >> node >> node;
		float x, y, z;

		pIn >> node >> x >> c >> y >> c >> z;

		const auto eye = DirectX::XMFLOAT3(x, y, z);

		pIn >> node >> x >> c >> y >> c >> z;

		const auto target = DirectX::XMFLOAT3(x, y, z);

		pIn >> node >> x >> c >> y >> c >> z;

		const auto up = DirectX::XMFLOAT3(x, y, z);

		auto camera = std::make_unique<Camera>(eye, up, target);

		CameraManager::instance()->setCamera1(camera);

		pIn >> node;
	}
	{
		std::string node;
		char c;

		pIn >> node >> node;
		float x, y, z;

		pIn >> node >> x >> c >> y >> c >> z;

		const auto eye = DirectX::XMFLOAT3(x, y, z);

		pIn >> node >> x >> c >> y >> c >> z;

		const auto target = DirectX::XMFLOAT3(x, y, z);

		pIn >> node >> x >> c >> y >> c >> z;

		const auto up = DirectX::XMFLOAT3(x, y, z);

		auto camera = std::make_unique<Camera>(eye, up, target);

		CameraManager::instance()->setCamera2(camera);

		pIn >> node;
	}
}
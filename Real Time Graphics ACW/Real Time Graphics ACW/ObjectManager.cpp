#include "ObjectManager.h"
#include "IdentityNode.h"
#include "GourandShading.h"

ObjectManager * ObjectManager::mInstance = nullptr;

ObjectManager::ObjectManager()
{
	mNode = std::make_unique<IdentityNode>();
	
	std::ifstream in("Configuration.txt");
	
	if (in.good())
	{
		in >> *mNode;
	}
	
	in.close();
	
	staticTechnique = std::make_unique<GourandShading>();
	dynamicTechniques.emplace_back(std::make_unique<GourandShading>();
}

void ObjectManager::update()
{
	DirectX::XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, DirectX::XMMatrixIdentity());
	
	node->update(world);
}

void ObjectManager::render()
{
	for (int i = 0; i < dynamicObjects; i++)
	{
		staticTechnique.render(dynamicObjects[i], false);
	}
}

void ObjectManager::addStaticObject(std::shared_ptr<Object> pObject)
{
	staticObjects.push_back(pObject);
}

void ObjectManager::addDynamicObject(std::shared_ptr<Object> pObject)
{
	dynamicObjects.push_back(pObject);
}
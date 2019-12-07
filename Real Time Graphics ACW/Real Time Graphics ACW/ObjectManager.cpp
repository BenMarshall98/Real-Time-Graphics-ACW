#include "ObjectManager.h"
#include "GourandShading.h"
#include "PhongShading.h"

ObjectManager * ObjectManager::mInstance = nullptr;

ObjectManager::ObjectManager()
{
	//staticTechnique = std::make_unique<GourandShading>();
	staticTechnique = std::make_unique<PhongShading>();
	dynamicTechniques.emplace_back(std::make_unique<GourandShading>());
}

void ObjectManager::render()
{
	for (int i = 0; i < dynamicObjects.size(); i++)
	{
		staticTechnique->render(dynamicObjects[i], false);
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
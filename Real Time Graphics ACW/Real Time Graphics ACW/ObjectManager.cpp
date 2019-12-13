#include "ObjectManager.h"
#include "GourandShading.h"
#include "PhongShading.h"
#include "TextureMapping.h"
#include "DisplacementMapping.h"
#include "BumpMapping.h"
#include "ToonShading.h"

ObjectManager * ObjectManager::mInstance = nullptr;

ObjectManager::ObjectManager()
{
	//staticTechnique = std::make_unique<GourandShading>();
	//staticTechnique = std::make_unique<PhongShading>();
	//staticTechnique = std::make_unique<TextureMapping>();
	//staticTechnique = std::make_unique<BumpMapping>();
	//staticTechnique = std::make_unique<DisplacementMapping>();
	staticTechnique = std::make_unique<ToonShading>();
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
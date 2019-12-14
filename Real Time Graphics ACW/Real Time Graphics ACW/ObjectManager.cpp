#include "ObjectManager.h"
#include "GourandShading.h"
#include "PhongShading.h"
#include "TextureMapping.h"
#include "DisplacementMapping.h"
#include "BumpMapping.h"
#include "ToonShading.h"
#include "LightingManager.h"

ObjectManager * ObjectManager::mInstance = nullptr;

ObjectManager::ObjectManager()
{
	//staticTechnique = std::make_unique<GourandShading>();
	staticTechnique = std::make_unique<PhongShading>();
	//staticTechnique = std::make_unique<TextureMapping>();
	//staticTechnique = std::make_unique<BumpMapping>();
	//staticTechnique = std::make_unique<DisplacementMapping>();
	//staticTechnique = std::make_unique<ToonShading>();
	dynamicTechniques.emplace_back(std::make_unique<GourandShading>());
	dynamicTechniques.emplace_back(std::make_unique<PhongShading>());
	dynamicTechniques.emplace_back(std::make_unique<TextureMapping>());
	dynamicTechniques.emplace_back(std::make_unique<DisplacementMapping>());
	dynamicTechniques.emplace_back(std::make_unique<BumpMapping>());
	dynamicTechniques.emplace_back(std::make_unique<ToonShading>());
}

void ObjectManager::render()
{
	for (int i = 0; i < staticObjects.size(); i++)
	{
		staticTechnique->render(staticObjects[i], false);
	}
	
	for (int i = 0; i < dynamicObjects.size(); i++)
	{
		staticTechnique->render(dynamicObjects[i], false);
	}
}

void ObjectManager::renderShadows()
{
	//Directional Light
	const auto lightManager = LightingManager::instance();

	if (lightManager->updateDirectionalLightShadow())
	{
		for (int i = 0; i < staticObjects.size(); i++)
		{
			staticTechnique->renderDirectionalShadow(staticObjects[i]);
		}

		for (int i = 0; i < dynamicObjects.size(); i++)
		{
			staticTechnique->renderDirectionalShadow(dynamicObjects[i]);
		}
	}

	if (lightManager->updatePointLightShadow())
	{
		for (int i = 0; i < staticObjects.size(); i++)
		{
			staticTechnique->renderOmniDirectionalShadow(staticObjects[i]);
		}

		for (int i = 0; i < dynamicObjects.size(); i++)
		{
			staticTechnique->renderOmniDirectionalShadow(dynamicObjects[i]);
		}
	}

	for (int i = 0; i < lightManager->getNumberOfSpotLights(); i++)
	{
		lightManager->updateSpotLightShadow(i);
		
		for (int j = 0; j < staticObjects.size(); j++)
		{
			staticTechnique->renderOmniDirectionalShadow(staticObjects[j]);
		}

		for (int j = 0; j < dynamicObjects.size(); j++)
		{
			staticTechnique->renderOmniDirectionalShadow(staticObjects[j]);
		}
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
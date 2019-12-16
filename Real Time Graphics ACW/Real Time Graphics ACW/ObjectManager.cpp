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
	for (int i = 0; i < staticShapes.size(); i++)
	{
		staticTechnique->render(staticShapes[i], false);
	}
	
	for (int i = 0; i < dynamicShapes.size(); i++)
	{
		staticTechnique->render(dynamicShapes[i], false);
	}
}

void ObjectManager::renderShadows()
{
	//Directional Light
	const auto lightManager = LightingManager::instance();

	if (lightManager->updateDirectionalLightShadow())
	{
		for (int i = 0; i < staticShapes.size(); i++)
		{
			staticTechnique->renderDirectionalShadow(staticShapes[i]);
		}

		for (int i = 0; i < dynamicShapes.size(); i++)
		{
			staticTechnique->renderDirectionalShadow(dynamicShapes[i]);
		}
	}

	if (lightManager->updatePointLightShadow())
	{
		for (int i = 0; i < staticShapes.size(); i++)
		{
			staticTechnique->renderOmniDirectionalShadow(staticShapes[i]);
		}

		for (int i = 0; i < dynamicShapes.size(); i++)
		{
			staticTechnique->renderOmniDirectionalShadow(dynamicShapes[i]);
		}
	}

	for (int i = 0; i < lightManager->getNumberOfSpotLights(); i++)
	{
		lightManager->updateSpotLightShadow(i);
		
		for (int j = 0; j < staticShapes.size(); j++)
		{
			staticTechnique->renderOmniDirectionalShadow(staticShapes[j]);
		}

		for (int j = 0; j < dynamicShapes.size(); j++)
		{
			staticTechnique->renderOmniDirectionalShadow(staticShapes[j]);
		}
	}
}

void ObjectManager::addStaticShape(std::shared_ptr<Shape> pObject)
{
	staticShapes.push_back(pObject);
}

void ObjectManager::addDynamicShape(std::shared_ptr<Shape> pObject)
{
	dynamicShapes.push_back(pObject);
}
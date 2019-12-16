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
	mStaticTechnique = std::make_unique<PhongShading>();
	//staticTechnique = std::make_unique<TextureMapping>();
	//staticTechnique = std::make_unique<BumpMapping>();
	//staticTechnique = std::make_unique<DisplacementMapping>();
	//staticTechnique = std::make_unique<ToonShading>();
	mDynamicTechniques.emplace_back(std::make_unique<GourandShading>());
	mDynamicTechniques.emplace_back(std::make_unique<PhongShading>());
	mDynamicTechniques.emplace_back(std::make_unique<TextureMapping>());
	mDynamicTechniques.emplace_back(std::make_unique<DisplacementMapping>());
	mDynamicTechniques.emplace_back(std::make_unique<BumpMapping>());
	mDynamicTechniques.emplace_back(std::make_unique<ToonShading>());
}

void ObjectManager::render()
{
	for (auto& staticShape : mStaticShapes)
	{
		mStaticTechnique->render(staticShape, false);
	}
	
	for (auto& dynamicShape : mDynamicShapes)
	{
		mStaticTechnique->render(dynamicShape, false);
	}
}

void ObjectManager::renderShadows()
{
	//Directional Light
	const auto lightManager = LightingManager::instance();

	if (lightManager->updateDirectionalLightShadow())
	{
		for (auto& staticShape : mStaticShapes)
		{
			mStaticTechnique->renderDirectionalShadow(staticShape);
		}

		for (auto& dynamicShape : mDynamicShapes)
		{
			mStaticTechnique->renderDirectionalShadow(dynamicShape);
		}
	}

	if (lightManager->updatePointLightShadow())
	{
		for (auto& staticShape : mStaticShapes)
		{
			mStaticTechnique->renderOmniDirectionalShadow(staticShape);
		}

		for (auto& dynamicShape : mDynamicShapes)
		{
			mStaticTechnique->renderOmniDirectionalShadow(dynamicShape);
		}
	}

	for (auto i = 0u; i < lightManager->getNumberOfSpotLights(); i++)
	{
		lightManager->updateSpotLightShadow(i);
		
		for (auto& staticShape : mStaticShapes)
		{
			mStaticTechnique->renderOmniDirectionalShadow(staticShape);
		}

		for (auto& dynamicShape : mDynamicShapes)
		{
			mStaticTechnique->renderOmniDirectionalShadow(dynamicShape);
		}
	}
}

void ObjectManager::addStaticShape(const std::shared_ptr<Shape>& pShape)
{
	mStaticShapes.push_back(pShape);
}

void ObjectManager::addDynamicShape(const std::shared_ptr<Shape>& pShape)
{
	mDynamicShapes.push_back(pShape);
}
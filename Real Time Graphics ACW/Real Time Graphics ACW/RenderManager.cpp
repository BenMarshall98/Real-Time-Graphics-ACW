#include "RenderManager.h"

#include "GourandShading.h"
#include "PhongShading.h"
#include "TextureMapping.h"
#include "DisplacementMapping.h"
#include "BumpMapping.h"
#include "ToonShading.h"
#include "LightingManager.h"

RenderManager * RenderManager::mInstance = nullptr;

RenderManager::RenderManager()
{
	mStaticTechnique = std::make_unique<PhongShading>();
	mDynamicTechniques.emplace_back(std::make_unique<GourandShading>());
	mDynamicTechniques.emplace_back(std::make_unique<PhongShading>());
	mDynamicTechniques.emplace_back(std::make_unique<TextureMapping>());
	mDynamicTechniques.emplace_back(std::make_unique<DisplacementMapping>());
	mDynamicTechniques.emplace_back(std::make_unique<BumpMapping>());
	mDynamicTechniques.emplace_back(std::make_unique<ToonShading>());
}

void RenderManager::render()
{
	for (auto& staticShape : mStaticShapes)
	{
		mStaticTechnique->render(staticShape, false);
	}

	for (auto i = 0u; i < mDynamicShapes.size(); i++)
	{
		mDynamicTechniques[i]->render(mDynamicShapes[i], false);
	}
}

void RenderManager::renderShadows()
{
	//Directional Light
	const auto lightManager = LightingManager::instance();

	if (lightManager->updateDirectionalLightShadow())
	{
		for (auto i = 0u; i < mDynamicShapes.size(); i++)
		{
			mDynamicTechniques[i]->renderDirectionalShadow(mDynamicShapes[i]);
		}
	}

	//Point Light
	if (lightManager->updatePointLightShadow())
	{
		for (auto i = 0u; i < mDynamicShapes.size(); i++)
		{
			mDynamicTechniques[i]->renderOmniDirectionalShadow(mDynamicShapes[i]);
		}
	}

	//SpotLight
	for (auto i = 0u; i < lightManager->getNumberOfSpotLights(); i++)
	{
		lightManager->updateSpotLightShadow(i);

		for (auto j = 0u; j < mDynamicShapes.size(); j++)
		{
			mDynamicTechniques[j]->renderOmniDirectionalShadow(mDynamicShapes[j]);
		}
	}

	lightManager->useShadowTextures();
}

void RenderManager::addStaticShape(const std::shared_ptr<Shape>& pShape)
{
	mStaticShapes.push_back(pShape);
}

void RenderManager::addDynamicShape(const std::shared_ptr<Shape>& pShape)
{
	mDynamicShapes.push_back(pShape);
}

void RenderManager::removeShape(const std::shared_ptr<Shape>& pShape)
{
	auto it = std::find(mStaticShapes.begin(), mStaticShapes.end(), pShape);

	if (it != mStaticShapes.end())
	{
		mStaticShapes.erase(it);
	}

	it = std::find(mDynamicShapes.begin(), mDynamicShapes.end(), pShape);

	if (it != mDynamicShapes.end())
	{
		mDynamicShapes.erase(it);
	}

	//TODO: Deal with dynamic technique map
}

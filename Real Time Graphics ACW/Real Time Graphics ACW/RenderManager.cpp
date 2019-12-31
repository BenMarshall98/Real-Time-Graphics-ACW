#include "RenderManager.h"

#include "GourandShading.h"
#include "PhongShading.h"
#include "TextureMapping.h"
#include "DisplacementMapping.h"
#include "BumpMapping.h"
#include "ToonShading.h"
#include "LightingManager.h"
#include "EnvironmentMapping.h"
#include "ResourceManager.h"
#include "DX11Render.h"

RenderManager * RenderManager::mInstance = nullptr;

RenderManager::RenderManager()
{
	mHdrFrambuffer = std::make_unique<Framebuffer>();
	mDeferredBuffer = std::make_unique<Framebuffer>();

	if (!mHdrFrambuffer->loadFramebuffer(true, false, { DirectX::Colors::MidnightBlue, {0.0f, 0.0f, 0.0f, 1.0f } }, TextureType::TEXTURE_2D, 2))
	{
		mHdrFrambuffer.reset();
	}

	if (!mDeferredBuffer->loadFramebuffer(true, false, 
		{ {0.0f, 0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 0.0f, 1.0f} },
		TextureType::TEXTURE_2D, 7))
	{
		mDeferredBuffer.reset();
	}

	mHDRShader = ResourceManager::instance()->loadShader("HDRVertexShader.hlsl", "HDRFragmentShader.hlsl");
	mDeferredShader = ResourceManager::instance()->loadShader("DeferredVertexShader.hlsl", "DeferredFragmentShader.hlsl");
	mOutputModel = ResourceManager::instance()->loadModel("plane.obj");
	
	mStaticTechnique = std::make_unique<PhongShading>();
	mDynamicTechniques.emplace_back(std::make_unique<PhongShading>());
	//mDynamicTechniques.emplace_back(std::make_unique<GourandShading>());
	mDynamicTechniques.emplace_back(std::make_unique<TextureMapping>());
	mDynamicTechniques.emplace_back(std::make_unique<BumpMapping>());
	mDynamicTechniques.emplace_back(std::make_unique<DisplacementMapping>());
	//mDynamicTechniques.emplace_back(std::make_unique<EnvironmentMapping>());
	//mDynamicTechniques.emplace_back(std::make_unique<ToonShading>());
}

void RenderManager::render()
{
	auto deferred = true;
	
	if (mMode <= 7)
	{
		mDeferredBuffer->useFramebuffer();
	}
	else
	{
		mHdrFrambuffer->useFramebuffer();
		deferred = false;
	}
	
	for (auto& staticShape : mStaticShapes)
	{
		mStaticTechnique->render(staticShape, deferred);
	}

	for (auto i = 0u; i < mDynamicShapes.size(); i++)
	{
		mDynamicTechniques[i]->render(mDynamicShapes[i], deferred);
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

void RenderManager::renderToScreen()
{
	
	
	if (mMode <= 7)
	{
		mDeferredShader->useShader();
		mDeferredBuffer->useTexture(0);

		Dx11Render::instance()->bindDefaultFramebuffer();
		
		mOutputModel->render();
		mDeferredBuffer->releaseTexture(0);
	}
	else
	{
		mHDRShader->useShader();
		mHdrFrambuffer->useTexture(0);

		Dx11Render::instance()->bindDefaultFramebuffer();
		
		mOutputModel->render();
		mHdrFrambuffer->releaseTexture(0);
	}
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

void RenderManager::changeMode()
{
	mMode++;

	if (mMode > 9)
	{
		mMode = 0;
	}
}
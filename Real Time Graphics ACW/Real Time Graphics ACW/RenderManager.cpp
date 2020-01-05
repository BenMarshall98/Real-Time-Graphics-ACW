#include "RenderManager.h"

#include "GourandShading.h"
#include "PhongShading.h"
#include "TextureMapping.h"
#include "DisplacementMapping.h"
#include "BumpMapping.h"
#include "ToonShading.h"
#include "LightingManager.h"
#include "EnvironmentMapping.h"
#include "TransparencyShading.h"
#include "ResourceManager.h"
#include "DX11Render.h"
#include <string>

RenderManager * RenderManager::mInstance = nullptr;

RenderManager::RenderManager() : mBloom(std::make_unique<Bloom>())
{
	mHdrFramebuffer = std::make_unique<Framebuffer>();
	mDeferredBuffer = std::make_unique<Framebuffer>();
	mScreenFramebufferOne = std::make_unique<Framebuffer>();
	mScreenFramebufferTwo = std::make_unique<Framebuffer>();

	if (!mHdrFramebuffer->loadFramebuffer(true, true, { {0.0f, 0.0f, 0.0f, 0.0f } }, TextureType::TEXTURE_2D))
	{
		mHdrFramebuffer.reset();
	}

	if (!mDeferredBuffer->loadFramebuffer(true, true, 
		{ {0.0f, 0.0f, 0.0f, 0.0f},
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

	if (!mScreenFramebufferOne->loadFramebuffer(true, true, { { 0.0f, 0.0f, 0.0f, 0.0f } }))
	{
		mScreenFramebufferOne.reset();
	}

	if (!mScreenFramebufferTwo->loadFramebuffer(true, true, { {0.0f, 0.0f, 0.0f, 0.0f} }))
	{
		mScreenFramebufferTwo.reset();
	}

	mHDRShader = ResourceManager::instance()->loadShader("HDRVertexShader.hlsl", "HDRFragmentShader.hlsl");
	mDeferredShader = ResourceManager::instance()->loadShader("DeferredVertexShader.hlsl", "DeferredFragmentShader.hlsl");
	mOutputModel = ResourceManager::instance()->loadModel("plane.obj");
	
	mStaticTechnique = std::make_unique<PhongShading>();
	mDynamicTechniques.emplace_back(std::make_unique<PhongShading>());
	mDynamicTechniques.emplace_back(std::make_unique<TransparencyShading>());
	mDynamicTechniques.emplace_back(std::make_unique<GourandShading>());
	mDynamicTechniques.emplace_back(std::make_unique<TextureMapping>());
	mDynamicTechniques.emplace_back(std::make_unique<BumpMapping>());
	mDynamicTechniques.emplace_back(std::make_unique<DisplacementMapping>());
	//mDynamicTechniques.emplace_back(std::make_unique<EnvironmentMapping>());
	//mDynamicTechniques.emplace_back(std::make_unique<ToonShading>());
}

void RenderManager::setup(float pCurrentTime)
{
	GlobalBuffer gb =
	{
		pCurrentTime,
		mMode,
		0,
		0.0f
	};

	Dx11Render::instance()->useGlobalBuffer(gb);
}

void RenderManager::render()
{
	renderShadows();
	renderShapes();
	renderToScreen();
}

void RenderManager::renderShapes()
{
	auto deferred = true;
	
	if (mMode <= 7)
	{
		mDeferredBuffer->useFramebuffer();

		for (auto& staticShape : mStaticShapes)
		{
			mStaticTechnique->render(staticShape, deferred, mDeferredBuffer);
		}

		for (auto i = 0u; i < mDynamicShapes.size(); i++)
		{
			mDynamicTechniques[i]->render(mDynamicShapes[i], deferred, mDeferredBuffer);
		}
	}
	else
	{
		mScreenFramebufferOne->useFramebuffer();
		deferred = false;

		for (auto& staticShape : mStaticShapes)
		{
			mStaticTechnique->render(staticShape, deferred, mScreenFramebufferOne);
		}

		for (auto i = 0u; i < mDynamicShapes.size(); i++)
		{
			mDynamicTechniques[i]->render(mDynamicShapes[i], deferred, mScreenFramebufferOne);
		}
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
		mDeferredBuffer->useTexture(6);

		mScreenFramebufferOne->useFramebuffer();
		
		mOutputModel->render();
		mDeferredBuffer->releaseTexture(6);
	}

	if (mMode == 0 || mMode == 8 || mMode == 9)
	{
		//Post Processing
		for (auto& staticShape : mStaticShapes)
		{
			if (mFramebuffer == 0)
			{
				mScreenFramebufferOne->useTexture(6);
				if (mStaticTechnique->renderPostprocessing(mScreenFramebufferTwo))
				{
					mFramebuffer = 1;
				}
				mScreenFramebufferOne->releaseTexture(6);
				
			}
			else
			{
				mScreenFramebufferTwo->useTexture(6);
				if (mStaticTechnique->renderPostprocessing(mScreenFramebufferOne))
				{
					mFramebuffer = 0;
				}
				mScreenFramebufferTwo->releaseTexture(6);
			}
		}

		for (auto i = 0u; i < mDynamicShapes.size(); i++)
		{
			if (mFramebuffer == 0)
			{
				mScreenFramebufferOne->useTexture(6);
				if (mDynamicTechniques[i]->renderPostprocessing(mScreenFramebufferTwo))
				{
					mFramebuffer = 1;
				}
				mScreenFramebufferOne->releaseTexture(6);

			}
			else
			{
				mScreenFramebufferTwo->useTexture(6);
				if (mDynamicTechniques[i]->renderPostprocessing(mScreenFramebufferOne))
				{
					mFramebuffer = 0;
				}
				mScreenFramebufferTwo->releaseTexture(6);
			}
		}

		//Transparent
		if (mFramebuffer == 1)
		{
			mScreenFramebufferTwo->useFramebuffer(false);

			for (auto& staticShape : mStaticShapes)
			{
				mStaticTechnique->renderTransparent(staticShape, mScreenFramebufferTwo);
			}

			for (auto i = 0u; i < mDynamicShapes.size(); i++)
			{
				mDynamicTechniques[i]->renderTransparent(mDynamicShapes[i], mScreenFramebufferTwo);
			}

			//Ink
			InkRender::instance()->RenderInk();
		}
		else
		{
			//Transparent
			mScreenFramebufferOne->useFramebuffer(false);
			for (auto& staticShape : mStaticShapes)
			{
				mStaticTechnique->renderTransparent(staticShape, mScreenFramebufferOne);
			}

			for (auto i = 0u; i < mDynamicShapes.size(); i++)
			{
				mDynamicTechniques[i]->renderTransparent(mDynamicShapes[i], mScreenFramebufferOne);
			}

			//Ink
			InkRender::instance()->RenderInk();
		}

		//TODO: ink

		//TODO: explosion
	}

	if (mMode == 9)
	{
		auto strength = 5;
		if (mFramebuffer == 0)
		{
			mBloom->applyBloom(mScreenFramebufferOne, strength);
		}
		else
		{
			mBloom->applyBloom(mScreenFramebufferTwo, strength);
		}
	}

	if (mFramebuffer == 0)
	{
		mScreenFramebufferOne->useTexture(6);

		mBloom->useBloomTexture(7);

		Dx11Render::instance()->bindDefaultFramebuffer();

		mHDRShader->useShader();

		mOutputModel->render();
		mScreenFramebufferOne->releaseTexture(6);

		mBloom->releaseBloomTexture(7);
	}
	else
	{
		mScreenFramebufferTwo->useTexture(6);

		mBloom->useBloomTexture(7);

		Dx11Render::instance()->bindDefaultFramebuffer();

		mHDRShader->useShader();

		mOutputModel->render();
		mScreenFramebufferTwo->releaseTexture(6);

		mBloom->releaseBloomTexture(7);
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
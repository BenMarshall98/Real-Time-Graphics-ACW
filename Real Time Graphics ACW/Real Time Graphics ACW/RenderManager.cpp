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
#include "ParticleManager.h"
#include <string>
#include "Game.h"

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

	ResourceManager::instance()->loadShader(mHDRShader, "HDRVertexShader.hlsl", "HDRFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(mDeferredShader, "DeferredVertexShader.hlsl", "DeferredFragmentShader.hlsl");
	ResourceManager::instance()->loadModel(mOutputModel, "plane.obj");
	
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

void RenderManager::setup(const float pCurrentTime) const
{
	const GlobalBuffer gb =
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
	mFramebuffer = 0u;
	auto deferred = true;
	
	if (mMode <= 7)
	{
		mDeferredBuffer->useFramebuffer();

		for (const auto& staticShape : mStaticShapes)
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

		for (const auto& staticShape : mStaticShapes)
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
	if (LightingManager::instance()->updateDirectionalLightShadow())
	{
		for (auto i = 0u; i < mDynamicShapes.size(); i++)
		{
			mDynamicTechniques[i]->renderDirectionalShadow(mDynamicShapes[i]);
		}
	}

	//Point Light
	if (LightingManager::instance()->updatePointLightShadow())
	{
		for (auto i = 0u; i < mDynamicShapes.size(); i++)
		{
			mDynamicTechniques[i]->renderOmniDirectionalShadow(mDynamicShapes[i]);
		}
	}

	//SpotLight
	for (auto i = 0u; i < LightingManager::instance()->getNumberOfSpotLights(); i++)
	{
		LightingManager::instance()->updateSpotLightShadow(i);

		for (auto j = 0u; j < mDynamicShapes.size(); j++)
		{
			mDynamicTechniques[j]->renderOmniDirectionalShadow(mDynamicShapes[j]);
		}
	}

	LightingManager::instance()->useShadowTextures();
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

		ParticleManager::instance()->update(Game::mDt);

		//Transparent
		if (mFramebuffer == 1)
		{
			mScreenFramebufferTwo->useFramebuffer(false);

			for (const auto& staticShape : mStaticShapes)
			{
				mStaticTechnique->renderTransparent(staticShape, mScreenFramebufferTwo);
			}

			for (auto i = 0u; i < mDynamicShapes.size(); i++)
			{
				mDynamicTechniques[i]->renderTransparent(mDynamicShapes[i], mScreenFramebufferTwo);
			}

			//Particles
			ParticleManager::instance()->render();

			//Ink
			InkRender::instance()->RenderInk();

			
		}
		else
		{
			//Transparent
			mScreenFramebufferOne->useFramebuffer(false);
			for (const auto& staticShape : mStaticShapes)
			{
				mStaticTechnique->renderTransparent(staticShape, mScreenFramebufferOne);
			}

			for (auto i = 0u; i < mDynamicShapes.size(); i++)
			{
				mDynamicTechniques[i]->renderTransparent(mDynamicShapes[i], mScreenFramebufferOne);
			}

			//Particles
			ParticleManager::instance()->render();

			//Ink
			InkRender::instance()->RenderInk();
		}
	}

	if (mMode == 9)
	{
		const auto strength = 5;
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

		if (mMode == 9)
		{
			mBloom->useBloomTexture(7);
		}

		Dx11Render::instance()->bindDefaultFramebuffer();

		mHDRShader->useShader();

		mOutputModel->render();
		mScreenFramebufferOne->releaseTexture(6);

		if (mMode == 9)
		{
			mBloom->releaseBloomTexture(7);
		}
	}
	else
	{
		mScreenFramebufferTwo->useTexture(6);

		if (mMode == 9)
		{
			mBloom->useBloomTexture(7);
		}

		Dx11Render::instance()->bindDefaultFramebuffer();

		mHDRShader->useShader();

		mOutputModel->render();
		mScreenFramebufferTwo->releaseTexture(6);
	}
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
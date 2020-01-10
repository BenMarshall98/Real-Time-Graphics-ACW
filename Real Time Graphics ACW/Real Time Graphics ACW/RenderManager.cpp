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
#include "GlowingShading.h"
#include <string>
#include "Game.h"
#include <ctime>

RenderManager * RenderManager::mInstance = nullptr;

RenderManager::RenderManager() : mEngine(time(0)),  mBloom(std::make_unique<Bloom>())
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
	mDynamicTechniques.emplace_back(std::make_shared<PhongShading>());
	mDynamicTechniques.emplace_back(std::make_shared<GlowingShading>());
	mDynamicTechniques.emplace_back(std::make_shared<ToonShading>());
	mDynamicTechniques.emplace_back(std::make_shared<EnvironmentMapping>());
	mDynamicTechniques.emplace_back(std::make_shared<BumpMapping>());
	mDynamicTechniques.emplace_back(std::make_shared<DisplacementMapping>());
	mDynamicTechniques.emplace_back(std::make_shared<TransparencyShading>());
	mDynamicTechniques.emplace_back(std::make_shared<GourandShading>());
	mDynamicTechniques.emplace_back(std::make_shared<TextureMapping>());

	mNumber = std::uniform_int_distribution<int>(0, 8);

	for(auto i = 0u; i < mDynamicTechniques.size(); i++)
	{
		mRenderTechnique[mDynamicTechniques[i]] = nullptr;
	}
}

void RenderManager::setup(const float pCurrentTime) const
{
	const GlobalBuffer gb =
	{
		pCurrentTime,
		mMode,
		LightingManager::instance()->getShadowMode(),
		InkRender::instance()->getHeight()
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

		for (auto it = mRenderTechnique.begin(); it != mRenderTechnique.end(); ++it)
		{
			if (it->second)
			{
				it->first->render(it->second, deferred, mDeferredBuffer);
			}
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

		for (auto it = mRenderTechnique.begin(); it != mRenderTechnique.end(); ++it)
		{
			if (it->second)
			{
				it->first->render(it->second, deferred, mScreenFramebufferOne);
			}
		}
	}
}

void RenderManager::renderShadows()
{

	const auto shadowMode = LightingManager::instance()->getShadowMode();
	Dx11Render::instance()->cullNone();

	//Directional Light
	if (LightingManager::instance()->updateDirectionalLightShadow())
	{
		for (auto it = mRenderTechnique.begin(); it != mRenderTechnique.end(); ++it)
		{
			if (it->second)
			{
				if (shadowMode == 0)
				{
					it->first->renderDirectionalSimpleShadow(it->second);
				}
				else
				{
					it->first->renderDirectionalShadow(it->second);
				}
			}
		}
	}

	//Point Light
	if (LightingManager::instance()->updatePointLightShadow())
	{
		for (auto it = mRenderTechnique.begin(); it != mRenderTechnique.end(); ++it)
		{
			if (it->second)
			{
				if (shadowMode == 0)
				{
					it->first->renderPointSimpleShadow(it->second);
				}
				else
				{
					it->first->renderOmniDirectionalShadow(it->second);
				}
			}
		}
	}

	//SpotLight
	for (auto i = 0u; i < LightingManager::instance()->getNumberOfSpotLights(); i++)
	{
		LightingManager::instance()->updateSpotLightShadow(i);

		for (auto it = mRenderTechnique.begin(); it != mRenderTechnique.end(); ++it)
		{
			if (it->second)
			{
				if (shadowMode == 0)
				{
					it->first->renderPointSimpleShadow(it->second);
				}
				else
				{
					it->first->renderOmniDirectionalShadow(it->second);
				}
			}
		}
	}

	LightingManager::instance()->useShadowTextures();

	Dx11Render::instance()->cullFront();
}

void RenderManager::renderToScreen()
{
	if (mMode <= 7)
	{
		mDeferredShader->useShader();
		mDeferredBuffer->useTexture(12);

		mScreenFramebufferOne->useFramebuffer();
		
		mOutputModel->render();
		mDeferredBuffer->releaseTexture(12);
	}

	if (mMode == 0 || mMode == 8 || mMode == 9)
	{
		//Post Processing
		if (mFramebuffer == 0)
		{
			mScreenFramebufferOne->useTexture(12);
			if (mStaticTechnique->renderPostprocessing(mScreenFramebufferTwo))
			{
				mFramebuffer = 1;
			}
			mScreenFramebufferOne->releaseTexture(12);
			
		}
		else
		{
			mScreenFramebufferTwo->useTexture(12);
			if (mStaticTechnique->renderPostprocessing(mScreenFramebufferOne))
			{
				mFramebuffer = 0;
			}
			mScreenFramebufferTwo->releaseTexture(12);
		}

		for (auto it = mRenderTechnique.begin(); it != mRenderTechnique.end(); ++it)
		{
			if (it->second)
			{
				if (mFramebuffer == 0)
				{
					mScreenFramebufferOne->useTexture(12);
					if (it->first->renderPostprocessing(mScreenFramebufferTwo))
					{
						mFramebuffer = 1;
					}
					mScreenFramebufferOne->releaseTexture(12);

				}
				else
				{
					mScreenFramebufferTwo->useTexture(12);
					if (it->first->renderPostprocessing(mScreenFramebufferOne))
					{
						mFramebuffer = 0;
					}
					mScreenFramebufferTwo->releaseTexture(12);
				}
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

			for (auto it = mRenderTechnique.begin(); it != mRenderTechnique.end(); ++it)
			{
				if (it->second)
				{
					it->first->renderTransparent(it->second, mScreenFramebufferTwo);
				}
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

			for (auto it = mRenderTechnique.begin(); it != mRenderTechnique.end(); ++it)
			{
				if (it->second)
				{
					it->first->renderTransparent(it->second, mScreenFramebufferOne);
				}
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
		mScreenFramebufferOne->useTexture(12);

		if (mMode == 9)
		{
			mBloom->useBloomTexture(13);
		}

		Dx11Render::instance()->bindDefaultFramebuffer();

		mHDRShader->useShader();

		mOutputModel->render();
		mScreenFramebufferOne->releaseTexture(12);

		if (mMode == 9)
		{
			mBloom->releaseBloomTexture(13);
		}
	}
	else
	{
		mScreenFramebufferTwo->useTexture(12);

		if (mMode == 9)
		{
			mBloom->useBloomTexture(13);
		}

		Dx11Render::instance()->bindDefaultFramebuffer();

		mHDRShader->useShader();

		mOutputModel->render();
		mScreenFramebufferTwo->releaseTexture(12);

		if (mMode == 9)
		{
			mBloom->releaseBloomTexture(13);
		}
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

	for (auto it = mRenderTechnique.begin(); it != mRenderTechnique.end(); ++it)
	{
		if (it->second == pShape)
		{
			it->second = nullptr;
		}
	}
}

void RenderManager::changeMode()
{
	mMode++;

	if (mMode > 9)
	{
		mMode = 0;
	}
}

void RenderManager::addDynamicShape(const std::shared_ptr<Shape>& pShape)
{
	while(true)
	{
		const auto technique = mDynamicTechniques[mNumber(mEngine)];

		const auto it = mRenderTechnique.find(technique);

		if (it != mRenderTechnique.end() && it->second == nullptr)
		{
			mRenderTechnique[technique] = pShape;
			break;
		}
	}
	
	mDynamicShapes.push_back(pShape);
}

void RenderManager::changeTechniques()
{
	for (auto it = mRenderTechnique.begin(); it != mRenderTechnique.end(); ++it)
	{
		it->second = nullptr;
	}

	for (auto i = 0u; i < mDynamicShapes.size(); i++)
	{
		while (true)
		{
			const auto technique = mDynamicTechniques[mNumber(mEngine)];

			const auto it = mRenderTechnique.find(technique);

			if (it != mRenderTechnique.end() && it->second == nullptr)
			{
				mRenderTechnique[technique] = mDynamicShapes[i];
				break;
			}
		}
	}
}
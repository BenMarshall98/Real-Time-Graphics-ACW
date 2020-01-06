#include "Bloom.h"
#include "ResourceManager.h"

Bloom::Bloom() :
	
	mHorzBloomFramebuffer(std::make_unique<Framebuffer>()),
	mVertBloomFramebuffer(std::make_unique<Framebuffer>())
{
	ResourceManager::instance()->loadShader(mHorzBloomShader, "HorzBloomVertexShader.hlsl", "HorzBloomFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(mVertBloomShader, "VertBloomVertexShader.hlsl", "VertBloomFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(mBloomExtractShader, "BloomExtractVertexShader.hlsl", "BloomExtractFragmentShader.hlsl");
	ResourceManager::instance()->loadModel(mRenderModel, "Plane.obj");
	
	if (!mHorzBloomFramebuffer->loadFramebuffer(true, false, { {0.0f, 0.0f, 0.0f, 0.0f} }))
	{
		mHorzBloomFramebuffer.reset();
	}

	if (!mVertBloomFramebuffer->loadFramebuffer(true, false, { {0.0f, 0.0f, 0.0f, 0.0f} }))
	{
		mVertBloomFramebuffer.reset();
	}
}

//TODO: Source: LearnOpenGL

void Bloom::applyBloom(const std::unique_ptr<Framebuffer> & pCurrentFramebuffer, const unsigned int pBloomStrength)
{
	pCurrentFramebuffer->useTexture(0);

	mVertBloomFramebuffer->useFramebuffer();

	mBloomExtractShader->useShader();

	mRenderModel->render();

	pCurrentFramebuffer->releaseTexture(0);

	for (auto i = 0u; i < pBloomStrength; i++)
	{
		//Horz Blur
		{
			mVertBloomFramebuffer->useTexture(0);

			mHorzBloomFramebuffer->useFramebuffer();
			mHorzBloomShader->useShader();

			mRenderModel->render();

			mVertBloomFramebuffer->releaseTexture(0);
		}

		//Vert Blur
		{
			mHorzBloomFramebuffer->useTexture(0);
			
			mVertBloomFramebuffer->useFramebuffer();
			mVertBloomShader->useShader();

			mRenderModel->render();

			mHorzBloomFramebuffer->releaseTexture(0);
		}
	}
}

void Bloom::useBloomTexture(const unsigned int pSlot) const
{
	mVertBloomFramebuffer->useTexture(pSlot);
}

void Bloom::releaseBloomTexture(const unsigned int pSlot) const
{
	mVertBloomFramebuffer->releaseTexture(pSlot);
}
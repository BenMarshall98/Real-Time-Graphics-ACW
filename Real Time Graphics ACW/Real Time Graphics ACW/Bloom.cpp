#include "Bloom.h"
#include "ResourceManager.h"

Bloom::Bloom() :
	mHorzBloomShader(ResourceManager::instance()->loadShader("HorzBloomVertexShader.hlsl", "HorzBloomFragmentShader.hlsl")),
	mVertBloomShader(ResourceManager::instance()->loadShader("VertBloomVertexShader.hlsl", "VertBloomFragmentShader.hlsl")),
	mBloomExtractShader(ResourceManager::instance()->loadShader("BloomExtractVertexShader.hlsl", "BloomExtractFragmentShader.hlsl")),
	mHorzBloomFramebuffer(std::make_unique<Framebuffer>()),
	mVertBloomFramebuffer(std::make_unique<Framebuffer>()),
	mRenderModel(ResourceManager::instance()->loadModel("Plane.obj"))
{
	if (!mHorzBloomFramebuffer->loadFramebuffer(true, false, { {0.0f, 0.0f, 0.0f, 0.0f} }))
	{
		mHorzBloomFramebuffer.reset();
	}

	if (!mVertBloomFramebuffer->loadFramebuffer(true, false, { {0.0f, 0.0f, 0.0f, 0.0f} }))
	{
		mVertBloomFramebuffer.reset();
	}
}

Bloom::~Bloom()
{
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
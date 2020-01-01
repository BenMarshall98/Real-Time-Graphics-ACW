#include "GourandShading.h"
#include "ResourceManager.h"

GourandShading::GourandShading() :
	Technique(
		ResourceManager::instance()->loadShader("GourandVertexShader.hlsl", "GourandFragmentShader.hlsl"),
		nullptr,
		ResourceManager::instance()->loadShader("NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl")
	), mFramebuffer(std::make_unique<Framebuffer>())
{
	if (!mFramebuffer->loadFramebuffer(true, true, { { 0.0f, 0.0f, 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f, 0.0f } }, TextureType::TEXTURE_2D, 2))
	{
		mFramebuffer.reset();
	}
}

void GourandShading::render(std::shared_ptr<Shape>& pShape, bool, std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	mFramebuffer->useFramebuffer();
	mNormalShader->useShader();
	pShape->render();

	pCurrentFramebuffer->useFramebuffer();
}

void GourandShading::renderDirectionalShadow(std::shared_ptr<Shape>& pShape)
{
	mDirectionalShader->useShader();
	pShape->render();
}

void GourandShading::renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape)
{
	mOmniDirectionalShader->useShader();
	pShape->render();
}

void GourandShading::renderPostprocessing(std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	//TODO: Implement
}
#include "TextureMapping.h"
#include "ResourceManager.h"

TextureMapping::TextureMapping() :
	Technique(
		ResourceManager::instance()->loadShader("TextureVertexShader.hlsl", "TextureFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("TextureDeferredVertexShader.hlsl", "TextureDeferredFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl")
	)
{
}

void TextureMapping::render(std::shared_ptr<Shape>& pShape, bool pDeferred, std::unique_ptr<Framebuffer> &)
{
	if (pDeferred)
	{
		mDeferredShader->useShader();
	}
	else
	{
		mNormalShader->useShader();
	}
	
	pShape->render();
}

void TextureMapping::renderDirectionalShadow(std::shared_ptr<Shape>& pShape)
{
	mDirectionalShader->useShader();
	pShape->render();
}

void TextureMapping::renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape)
{
	mOmniDirectionalShader->useShader();
	pShape->render();
}

bool TextureMapping::renderPostprocessing(std::unique_ptr<Framebuffer> &)
{
	return false;
}

void TextureMapping::renderTransparent(std::shared_ptr<Shape> &, std::unique_ptr<Framebuffer> &)
{
	//Do Nothing
}
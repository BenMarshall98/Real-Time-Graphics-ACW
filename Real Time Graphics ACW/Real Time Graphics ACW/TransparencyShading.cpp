#include "TransparencyShading.h"
#include "ResourceManager.h"
#include "DX11Render.h"

TransparencyShading::TransparencyShading() :
	Technique(
		ResourceManager::instance()->loadShader("TransparencyVertexShader.hlsl", "TransparencyFragmentShader.hlsl"),
		nullptr,
		ResourceManager::instance()->loadShader("NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl")
	)
{
}

void TransparencyShading::render(const std::shared_ptr<Shape> & pShape, bool, std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
}

void TransparencyShading::renderDirectionalShadow(const std::shared_ptr<Shape> & pShape)
{
	useDirectionalShader();
	pShape->render();
}

void TransparencyShading::renderOmniDirectionalShadow(const std::shared_ptr<Shape> & pShape)
{
	useOmniDirectionalShader();
	pShape->render();
}

bool TransparencyShading::renderPostprocessing(std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	return false;
}

void TransparencyShading::renderTransparent(std::shared_ptr<Shape> & pShape, std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	Dx11Render::instance()->enableBlend();
	useNormalShader();
	pShape->render();
	Dx11Render::instance()->disableBlend();
}
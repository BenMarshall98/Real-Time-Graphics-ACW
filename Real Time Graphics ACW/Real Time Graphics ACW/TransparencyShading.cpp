#include "TransparencyShading.h"
#include "ResourceManager.h"
#include "DX11Render.h"

TransparencyShading::TransparencyShading()
{
	std::shared_ptr<Shader> normalShader;
	std::shared_ptr<Shader> directionalShader;
	std::shared_ptr<Shader> omniDirectionalShader;

	getNormalShader(normalShader);
	getDirectionalShader(directionalShader);
	getOmniDirectionalShader(omniDirectionalShader);

	ResourceManager::instance()->loadShader(normalShader, "TransparencyVertexShader.hlsl", "TransparencyFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(directionalShader, "NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(omniDirectionalShader, "NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl");

	setNormalShader(normalShader);
	setDirectionalShader(directionalShader);
	setOmniDirectionalShader(omniDirectionalShader);
}

TransparencyShading::~TransparencyShading() = default;

void TransparencyShading::render(const std::shared_ptr<Shape> & pShape, bool, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
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

bool TransparencyShading::renderPostprocessing(const std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	return false;
}

void TransparencyShading::renderTransparent(const std::shared_ptr<Shape> & pShape, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	Dx11Render::instance()->enableBlend();
	useNormalShader();
	pShape->render();
	Dx11Render::instance()->disableBlend();
}
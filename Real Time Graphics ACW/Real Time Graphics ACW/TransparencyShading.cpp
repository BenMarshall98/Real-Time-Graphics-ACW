#include "TransparencyShading.h"
#include "ResourceManager.h"
#include "DX11Render.h"

TransparencyShading::TransparencyShading()
{
	std::shared_ptr<Shader> normalShader;
	std::shared_ptr<Shader> directionalShader;
	std::shared_ptr<Shader> omniDirectionalShader;
	std::shared_ptr<Shader> directionalSimpleShader;
	std::shared_ptr<Shader> pointSimpleShader;

	getNormalShader(normalShader);
	getDirectionalShader(directionalShader);
	getOmniDirectionalShader(omniDirectionalShader);
	getDirectionalSimpleShader(directionalSimpleShader);
	getPointSimpleShader(pointSimpleShader);

	ResourceManager::instance()->loadShader(normalShader, "TransparencyVertexShader.hlsl", "TransparencyFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(directionalShader, "NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(omniDirectionalShader, "NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl");
	ResourceManager::instance()->loadShader(directionalSimpleShader, "NormalDirectionalSimpleShadowVertexShader.hlsl", "NormalDirectionalSimpleShadowFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(pointSimpleShader, "NormalPointSimpleShadowVertexShader.hlsl", "NormalPointSimpleShadowFragmentShader.hlsl");

	setNormalShader(normalShader);
	setDirectionalShader(directionalShader);
	setOmniDirectionalShader(omniDirectionalShader);
	setDirectionalSimpleShader(directionalSimpleShader);
	setPointSimpleShader(pointSimpleShader);
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

void TransparencyShading::renderDirectionalSimpleShadow(const std::shared_ptr<Shape> & pShape)
{
	useDirectionalSimpleShader();
	pShape->render();
}

void TransparencyShading::renderPointSimpleShadow(const std::shared_ptr<Shape> & pShape)
{
	usePointSimpleShader();
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
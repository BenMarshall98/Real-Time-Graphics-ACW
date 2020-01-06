#include "ToonShading.h"
#include "ResourceManager.h"

ToonShading::ToonShading()
{
	std::shared_ptr<Shader> normalShader;
	std::shared_ptr<Shader> directionalShader;
	std::shared_ptr<Shader> omniDirectionalShader;

	getNormalShader(normalShader);
	getDirectionalShader(directionalShader);
	getOmniDirectionalShader(omniDirectionalShader);

	ResourceManager::instance()->loadShader(normalShader, "ToonVertexShader.hlsl", "ToonFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(directionalShader, "NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(omniDirectionalShader, "NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl");

	setNormalShader(normalShader);
	setDirectionalShader(directionalShader);
	setOmniDirectionalShader(omniDirectionalShader);
}

ToonShading::~ToonShading() = default;

void ToonShading::render(const std::shared_ptr<Shape>& pShape, bool, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	//mNormalShader->useShader();
	//pShape->render();
}

void ToonShading::renderDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useDirectionalShader();
	pShape->render();
}

void ToonShading::renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useOmniDirectionalShader();
	pShape->render();
}

bool ToonShading::renderPostprocessing(const std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	return false; //TODO: Implement
}

void ToonShading::renderTransparent(const std::shared_ptr<Shape> &, const std::unique_ptr<Framebuffer> &)
{
	//Do Nothing
}
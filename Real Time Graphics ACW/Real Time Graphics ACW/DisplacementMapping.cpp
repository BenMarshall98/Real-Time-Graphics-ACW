#include "DisplacementMapping.h"
#include "ResourceManager.h"

DisplacementMapping::DisplacementMapping()
{
	std::shared_ptr<Shader> normalShader;
	std::shared_ptr<Shader> deferredShader;
	std::shared_ptr<Shader> directionalShader;
	std::shared_ptr<Shader> omniDirectionalShader;

	getNormalShader(normalShader);
	getDeferredShader(deferredShader);
	getDirectionalShader(directionalShader);
	getOmniDirectionalShader(omniDirectionalShader);

	ResourceManager::instance()->loadShader(normalShader, "DisplacementVertexShader.hlsl", "DisplacementFragmentShader.hlsl", "DisplacementHullShader.hlsl", "DisplacementDomainShader.hlsl");
	ResourceManager::instance()->loadShader(deferredShader, "DisplacementDeferredVertexShader.hlsl", "DisplacementDeferredFragmentShader.hlsl", "DisplacementDeferredHullShader.hlsl", "DisplacementDeferredDomainShader.hlsl");
	ResourceManager::instance()->loadShader(directionalShader, "DisplacementDirectionalShadowVertexShader.hlsl", "DisplacementDirectionalShadowFragmentShader.hlsl", "DisplacementDirectionalShadowHullShader.hlsl", "DisplacementDirectionalShadowDomainShader.hlsl");
	ResourceManager::instance()->loadShader(omniDirectionalShader, "DisplacementOmniShadowVertexShader.hlsl", "DisplacementOmniShadowFragmentShader.hlsl", "DisplacementOmniShadowGeometryShader.hlsl", "DisplacementOmniShadowHullShader.hlsl", "DisplacementOmniShadowDomainShader.hlsl");

	setNormalShader(normalShader);
	setDeferredShader(deferredShader);
	setDirectionalShader(directionalShader);
	setOmniDirectionalShader(omniDirectionalShader);
}

DisplacementMapping::~DisplacementMapping() = default;

void DisplacementMapping::render(const std::shared_ptr<Shape>& pShape, const bool pDeferred, const std::unique_ptr<Framebuffer> &)
{
	if (pDeferred)
	{
		useDeferredShader();
	}
	else
	{
		useNormalShader();
	}
	
	pShape->render(true);
}

void DisplacementMapping::renderDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useDirectionalShader();
	pShape->render(true);
}

void DisplacementMapping::renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useOmniDirectionalShader();
	pShape->render(true);
}

bool DisplacementMapping::renderPostprocessing(const std::unique_ptr<Framebuffer> &)
{
	return false;
}

void DisplacementMapping::renderTransparent(const std::shared_ptr<Shape> &, const std::unique_ptr<Framebuffer> &)
{
	//Do Nothing
}
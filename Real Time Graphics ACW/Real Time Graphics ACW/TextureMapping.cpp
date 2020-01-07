#include "TextureMapping.h"
#include "ResourceManager.h"

TextureMapping::TextureMapping()
{
	std::shared_ptr<Shader> normalShader;
	std::shared_ptr<Shader> deferredShader;
	std::shared_ptr<Shader> directionalShader;
	std::shared_ptr<Shader> omniDirectionalShader;

	getNormalShader(normalShader);
	getDeferredShader(deferredShader);
	getDirectionalShader(directionalShader);
	getOmniDirectionalShader(omniDirectionalShader);

	ResourceManager::instance()->loadShader(normalShader, "TextureVertexShader.hlsl", "TextureFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(deferredShader, "TextureDeferredVertexShader.hlsl", "TextureDeferredFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(directionalShader, "NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(omniDirectionalShader, "NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl");

	setNormalShader(normalShader);
	setDeferredShader(deferredShader);
	setDirectionalShader(directionalShader);
	setOmniDirectionalShader(omniDirectionalShader);
}

TextureMapping::~TextureMapping() = default;

void TextureMapping::render(const std::shared_ptr<Shape>& pShape, const bool pDeferred, const std::unique_ptr<Framebuffer> &)
{
	if (pDeferred)
	{
		useDeferredShader();
	}
	else
	{
		useNormalShader();
	}
	
	pShape->render();
}

void TextureMapping::renderDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useDirectionalShader();
	pShape->render();
}

void TextureMapping::renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useOmniDirectionalShader();
	pShape->render();
}

bool TextureMapping::renderPostprocessing(const std::unique_ptr<Framebuffer> &)
{
	return false;
}

void TextureMapping::renderTransparent(const std::shared_ptr<Shape> &, const std::unique_ptr<Framebuffer> &)
{
	//Do Nothing
}
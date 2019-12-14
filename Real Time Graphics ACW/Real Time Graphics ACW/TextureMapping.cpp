#include "TextureMapping.h"
#include "ResourceManager.h"

TextureMapping::TextureMapping() :
	Technique(
		ResourceManager::instance()->loadShader("TextureVertexShader.hlsl", "TextureFragmentShader.hlsl"),
		nullptr,
		ResourceManager::instance()->loadShader("NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl")
	)
{
}

TextureMapping::~TextureMapping()
{
}

void TextureMapping::render(std::shared_ptr<Object>& pObject, bool pDeferred)
{
	mNormalShader->useShader();
	pObject->render();
}

void TextureMapping::renderDirectionalShadow(std::shared_ptr<Object>& pObject)
{
	mDirectionalShader->useShader();
	pObject->render();
}

void TextureMapping::renderOmniDirectionalShadow(std::shared_ptr<Object>& pObject)
{
	mOmniDirectionalShader->useShader();
	pObject->render();
}

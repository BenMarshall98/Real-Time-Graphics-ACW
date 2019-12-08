#include "TextureMapping.h"
#include "ResourceManager.h"

TextureMapping::TextureMapping() :
	Technique(ResourceManager::instance()->loadShader(
		"TextureVertexShader.hlsl", "TextureFragmentShader.hlsl"
	), nullptr)
{
}

TextureMapping::~TextureMapping()
{
}

void TextureMapping::render(std::shared_ptr<Object>& pObject, bool pDeferred)
{
	mShader->useShader();
	pObject->render();
}

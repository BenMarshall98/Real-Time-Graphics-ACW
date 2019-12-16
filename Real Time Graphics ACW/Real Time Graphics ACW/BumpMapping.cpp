#include "BumpMapping.h"
#include "ResourceManager.h"

BumpMapping::BumpMapping() :
	Technique(
		ResourceManager::instance()->loadShader("BumpVertexShader.hlsl", "BumpFragmentShader.hlsl"),
		nullptr,
		ResourceManager::instance()->loadShader("NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl")
	)
{
}

BumpMapping::~BumpMapping()
{
}

void BumpMapping::render(std::shared_ptr<Shape>& pShape, bool pDeferred)
{
	mNormalShader->useShader();
	pShape->render();
}

void BumpMapping::renderDirectionalShadow(std::shared_ptr<Shape>& pShape)
{
	mDirectionalShader->useShader();
	pShape->render();
}

void BumpMapping::renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape)
{
	mOmniDirectionalShader->useShader();
	pShape->render();
}
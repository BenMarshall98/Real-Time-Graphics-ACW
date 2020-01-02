#include "InkCubeRender.h"
#include "ResourceManager.h"

InkCubeRender::InkCubeRender() :
	InkShapeRender(
		ResourceManager::instance()->loadShader("InkCubeDepthVertexShader.hlsl", "InkCubeDepthFragmentShader.hlsl", "InkCubeDepthGeometryShader.hlsl")
	)
{
	DirectX::XMVECTORF32 colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	if (!mFramebuffer->loadFramebuffer(true, false, 1024, 1024, { colour }, TextureType::TEXTURE_CUBE))
	{
		mFramebuffer.reset();
	}
}

void InkCubeRender::update(DirectX::XMFLOAT4X4 pMatrix)
{
	int i = 0;
	//TODO: Implement
}

void InkCubeRender::use()
{
	//TODO: Implement
}
#include "InkCubeRender.h"
#include "ResourceManager.h"

InkCubeRender::InkCubeRender() :
	InkShapeRender(
		ResourceManager::instance()->loadShader("InkCubeDepthVertexShader.hlsl", "InkCubeDepthFragmentShader.hlsl", "InkCubeDepthGeometryShader.hlsl")
	)
{
	if (!mFramebuffer->loadFramebuffer(true, false, 1024, 1024, TextureType::TEXTURE_CUBE))
	{
		mFramebuffer.reset();
	}
}

void InkCubeRender::update()
{
	
}

void InkCubeRender::use()
{
	
}
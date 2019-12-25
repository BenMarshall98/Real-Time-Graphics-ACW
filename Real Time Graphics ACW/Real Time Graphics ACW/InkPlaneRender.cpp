#include "InkPlaneRender.h"
#include "ResourceManager.h"

InkPlaneRender::InkPlaneRender() :
	InkShapeRender(
		ResourceManager::instance()->loadShader("InkPlaneVertexShader.hlsl", "InkPlaneFragmentShader.hlsl")
	)
{
	if (!mFramebuffer->loadFramebuffer(true, false, 1024, 1024))
	{
		mFramebuffer.reset();
	}
}

void InkPlaneRender::update(DirectX::XMFLOAT4X4 pMatrix)
{
	
	//TODO: Implement
}

void InkPlaneRender::use()
{
	//TODO: Implement
}

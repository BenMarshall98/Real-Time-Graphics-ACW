#include "InkRender.h"
#include "ResourceManager.h"
#include "Shape.h"
#include "DX11Render.h"

InkRender::InkRender()
{
	mDepthShader = ResourceManager::instance()->loadShader("InkDepthVertexShader.hlsl", "InkDepthFragmentShader.hlsl", "InkDepthHullShader.hlsl", "InkDepthDomainShader.hlsl");
	mModel = ResourceManager::instance()->loadModel("plane.obj");
}

void InkRender::RenderInk() const
{
	const auto trans = DirectX::XMMatrixTranslation(0.0f, mHeight, 0.0f);
	const auto scale = DirectX::XMMatrixScaling(5.0f, 5.0f, 5.0f);
	const auto matrix = DirectX::XMMatrixMultiply(trans, scale);

	ModelBuffer mb = {
		DirectX::XMMatrixTranspose(matrix),
		DirectX::XMMatrixInverse(nullptr, matrix)
	};

	Dx11Render::instance()->useModelBuffer(mb);

	//Dx11Render::instance()->enableBlend();
	
	mDepthShader->useShader();

	mModel->render(true);

	//Dx11Render::instance()->disableBlend();
}
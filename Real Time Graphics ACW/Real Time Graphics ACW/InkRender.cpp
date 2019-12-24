#include "InkRender.h"
#include "ResourceManager.h"
#include "Shape.h"
#include "DX11Render.h"

InkRender::InkRender()
{
	mFramebuffer = std::make_unique<Framebuffer>();

	if (!mFramebuffer->loadFramebuffer(false, true, 1024, 1024))
	{
		mFramebuffer.release();
	}

	mDepthShader = ResourceManager::instance()->loadShader("InkDepthVertexShader.hlsl", "InkDepthFragmentShader.hlsl", "InkDepthHullShader.hlsl", "InkDepthDomainShader.hlsl");
	mColorShader = ResourceManager::instance()->loadShader("InkColorVertexShader.hlsl", "InkColorFragmentShader.hlsl", "InkColorHullShader.hlsl", "InkColorDomainShader.hlsl");
	mModel = ResourceManager::instance()->loadModel("plane.obj");
}

void InkRender::UpdateInk()
{
	const auto trans = DirectX::XMMatrixTranslation(0.0f, mHeight, 0.0f);
	const auto scale = DirectX::XMMatrixScaling(5.0f, 5.0f, 5.0f);
	const auto matrix = DirectX::XMMatrixMultiply(trans, scale);
	
	ModelBuffer mb = {
		DirectX::XMMatrixTranspose(matrix),
		DirectX::XMMatrixInverse(nullptr, matrix)
	};

	Dx11Render::instance()->useModelBuffer(mb);

	InkBuffer lb;
	
	const auto pos = DirectX::XMFLOAT3(0.0f, mHeight, 0.0f);
	const auto eye = DirectX::XMLoadFloat3(&pos);
	const auto direction = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
	const auto target = DirectX::XMVectorAdd(eye, DirectX::XMLoadFloat3(&direction));
	const auto up = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	const auto view = XMMatrixTranspose(DirectX::XMMatrixLookAtLH(eye, target, up));
	XMStoreFloat4x4(&lb.mViewMatrix, view);

	const auto projection = DirectX::XMMatrixTranspose(DirectX::XMMatrixOrthographicOffCenterLH(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f));
	XMStoreFloat4x4(&lb.mProjectionMatrix, projection);

	Dx11Render::instance()->useInkBuffer(lb);

	mFramebuffer->releaseTexture(0);
	mFramebuffer->useFramebuffer();

	mDepthShader->useShader();

	mModel->render(true);
}

void InkRender::UseInk()
{
	
}

void InkRender::RenderInk()
{
	
}
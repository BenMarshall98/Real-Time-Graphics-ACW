#include "InkRender.h"
#include "ResourceManager.h"
#include "Shape.h"
#include "DX11Render.h"
#include "Game.h"

InkRender * InkRender::mInstance = nullptr;

InkRender::InkRender()
{
	ResourceManager::instance()->loadShader(mDepthShader, "InkDepthVertexShader.hlsl", "InkDepthFragmentShader.hlsl", "InkDepthHullShader.hlsl", "InkDepthDomainShader.hlsl");
	ResourceManager::instance()->loadModel(mModel, "plane.obj");
	ResourceManager::instance()->loadTexture(mDisp, "water_height.dds");
	ResourceManager::instance()->loadTexture(mNorm, "water_normal.dds");
}

void InkRender::RenderInk()
{
	if (mIncrease && !mDecrease)
	{
		mHeight += Game::mDt * 0.5f;

		if (mHeight > 5.0f)
		{
			mHeight = 5.0f;
		}
	}
	else if (!mIncrease && mDecrease)
	{
		mHeight -= Game::mDt * 0.5f;

		if (mHeight < -5.0f)
		{
			mHeight = -5.0f;
		}
	}

	const auto trans = DirectX::XMMatrixTranslation(0.0f, mHeight, 0.0f);
	const auto scale = DirectX::XMMatrixScaling(5.0f, 5.0f, 5.0f);
	const auto matrix = DirectX::XMMatrixMultiply(trans, scale);

	const ModelBuffer mb = {
		DirectX::XMMatrixTranspose(matrix),
		DirectX::XMMatrixInverse(nullptr, matrix)
	};

	Dx11Render::instance()->useModelBuffer(mb);

	Dx11Render::instance()->enableBlend();
	
	mDepthShader->useShader();

	
	mNorm->useFragment(8);
	mDisp->useDomain(0);
	mModel->render(true);

	Dx11Render::instance()->disableBlend();
}
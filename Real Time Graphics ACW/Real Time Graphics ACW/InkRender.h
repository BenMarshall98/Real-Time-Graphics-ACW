#pragma once

#include "Framebuffer.h"
#include "Shader.h"
#include "Model.h"
#include <DirectXMath.h>
#include "Texture.h"

struct InkBuffer
{
	DirectX::XMFLOAT4X4 mViewMatrix;
	DirectX::XMFLOAT4X4 mProjectionMatrix;
};

class InkRender
{
	static InkRender * mInstance;

	InkRender();

	std::shared_ptr<Shader> mDepthShader;
	std::shared_ptr<Model> mModel;
	std::shared_ptr<Texture> mDisp;
	std::shared_ptr<Texture> mNorm;
	
	float mHeight = 0.0f;

	bool mIncrease = false;
	bool mDecrease = false;
	
public:
	~InkRender() = default;
	void IncreaseHeight(const bool pIsTrue)
	{
		mIncrease = pIsTrue;
	}

	void DecreaseHeight(const bool pIsTrue)
	{
		mDecrease = pIsTrue;
	}

	float getHeight() const
	{
		return mHeight;
	}

	void RenderInk();

	static InkRender * instance()
	{
		if (!mInstance)
		{
			mInstance = new InkRender();
		}
		return mInstance;
	}
};

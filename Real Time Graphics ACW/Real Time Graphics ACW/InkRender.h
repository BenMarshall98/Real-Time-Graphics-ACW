#pragma once

#include "Framebuffer.h"
#include "Shader.h"
#include "Model.h"
#include <DirectXMath.h>

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
	
	float mHeight = 0.0f;

	bool mIncrease = false;
	bool mDecrease = false;
	
public:
	~InkRender() = default;
	void IncreaseHeight(bool pIsTrue)
	{
		mIncrease = pIsTrue;
	}

	void DecreaseHeight(bool pIsTrue)
	{
		mDecrease = pIsTrue;
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

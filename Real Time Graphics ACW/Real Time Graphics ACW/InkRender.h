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
	std::shared_ptr<Shader> mDepthShader;
	std::shared_ptr<Model> mModel;
	
	float mHeight = 0.0f;
	
public:
	InkRender();
	~InkRender() = default;
	void IncreaseHeight(float pDt);
	void DecreaseHeight(float pDt);
	void RenderInk() const;
};

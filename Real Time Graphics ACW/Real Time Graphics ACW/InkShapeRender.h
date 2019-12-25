#pragma once

#include "Framebuffer.h"
#include "Shader.h"
#include <DirectXMath.h>

class InkShapeRender
{
protected:
	std::unique_ptr<Framebuffer> mFramebuffer = nullptr;
	std::shared_ptr<Shader> mShader = nullptr;
	
public:
	InkShapeRender(std::shared_ptr<Shader> pShader);
	virtual ~InkShapeRender() = default;

	virtual void update(DirectX::XMFLOAT4X4 pMatrix) = 0;
	virtual void use() = 0;
};


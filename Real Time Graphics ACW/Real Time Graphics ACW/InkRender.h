#pragma once

#include "Framebuffer.h"
#include "Shader.h"

class InkRender
{
	std::unique_ptr<Framebuffer> mFrambuffer;
	std::unique_ptr<Shader> mDepthShader;
	std::unique_ptr<Shader> mColorShader;
	
	float mHeight = 5.0f;
	
public:
	InkRender();
	~InkRender();
	void UpdateInk();
	void IncreaseHeight(float pDt);
	void DecreaseHeight(float pDt);
	void UseInk();
	void RenderInk();
	void UsePlaneDepthShader();
	void UseCubeDepthShader();
};

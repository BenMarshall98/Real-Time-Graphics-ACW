#pragma once

#include "Shader.h"
#include "Framebuffer.h"
#include "Model.h"

class Bloom final
{
	std::shared_ptr<Shader> mHorzBloomShader;
	std::shared_ptr<Shader> mVertBloomShader;
	std::shared_ptr<Shader> mBloomExtractShader;
	std::unique_ptr<Framebuffer> mHorzBloomFramebuffer;
	std::unique_ptr<Framebuffer> mVertBloomFramebuffer;
	std::shared_ptr<Model> mRenderModel;

public:
	Bloom();
	~Bloom();

	void applyBloom(std::unique_ptr<Framebuffer> & pCurrentFramebuffer, unsigned int pBloomStrength);
	void useBloomTexture(unsigned int pSlot);
	void releaseBloomTexture(unsigned int pSlot);
};


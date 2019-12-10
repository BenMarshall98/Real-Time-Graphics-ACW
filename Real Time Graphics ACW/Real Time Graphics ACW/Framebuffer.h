#pragma once
#include <d3d11.h>
#include <vector>

enum class TextureType
{
	Texture2D,
	TextureCube
};

class Framebuffer
{
	ID3D11Texture2D * mColorTexture = nullptr;
	std::vector<ID3D11RenderTargetView *> mColorTextureTargetViews;
	std::vector<ID3D11ShaderResourceView *> mColorTextureResourceViews;

	ID3D11Texture2D * mDepthTexture = nullptr;
	ID3D11DepthStencilState * mDepthState = nullptr;
	ID3D11DepthStencilView * mDepthTextureTargetView = nullptr;
	ID3D11ShaderResourceView * mDepthTextureResourceView = nullptr;

	ID3D11SamplerState * mSampler = nullptr;

	TextureType mType;

	bool mUpdateResize = false;
	
public:
	Framebuffer() = default;
	void loadFramebuffer(bool pColour, bool pDepth, TextureType pType = TextureType::Texture2D, unsigned int pNumberOfBuffers = 1u);
	void loadFramebuffer(bool pColour, bool pDepth, int pWidth, int pHeight, TextureType pType = TextureType::Texture2D, unsigned int pNumberOfBuffers = 1u);
	~Framebuffer() = default;

	Framebuffer(const Framebuffer & pFramebuffer) = delete;
	Framebuffer & operator= (const Framebuffer & pFramebuffer) = delete;

	void useFramebuffer() const;
	void useTexture(unsigned int & pSlot);
};


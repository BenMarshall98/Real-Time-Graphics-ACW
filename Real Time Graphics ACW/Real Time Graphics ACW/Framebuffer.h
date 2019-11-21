#pragma once
#include <d3d11.h>

class Framebuffer
{
	ID3D11Texture2D * mTexture;
	ID3D11RenderTargetView * mTextureTargetView;
	ID3D11ShaderResourceView * mTextureResourceView;
	
public:
	Framebuffer(const bool & pColour, const bool & pDepth);
	~Framebuffer() = default;

	Framebuffer(const Framebuffer & pFramebuffer) = delete;
	Framebuffer & operator= (const Framebuffer & pFramebuffer) = delete;
};


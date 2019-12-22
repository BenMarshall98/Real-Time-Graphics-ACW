#pragma once
#include <d3d11.h>
#include <vector>
#include <wrl/client.h>

enum class TextureType
{
	TEXTURE_2D,
	TEXTURE_CUBE
};

class Framebuffer
{
	//TODO: Look at away of using comptr with vector
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mColorTexture = nullptr;
	std::vector<ID3D11RenderTargetView *> mColorTextureTargetViews;
	std::vector<ID3D11ShaderResourceView *> mColorTextureResourceViews;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepthTexture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthState = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthTextureTargetView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mDepthTextureResourceView = nullptr;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> mSampler = nullptr;

	TextureType mType = TextureType::TEXTURE_2D;
	unsigned int mWidth;
	unsigned int mHeight;
	bool mUpdateResize = false;
	
public:
	Framebuffer() = default;
	bool loadFramebuffer(bool pColour, bool pDepth, TextureType pType = TextureType::TEXTURE_2D, unsigned int pNumberOfBuffers = 1u);
	bool loadFramebuffer(bool pColour, bool pDepth, int pWidth, int pHeight, TextureType pType = TextureType::TEXTURE_2D, unsigned int pNumberOfBuffers = 1u);
	~Framebuffer() = default;

	Framebuffer(const Framebuffer & pFramebuffer) = delete;
	Framebuffer & operator= (const Framebuffer & pFramebuffer) = delete;

	void useFramebuffer() const;
	void useTexture(unsigned int pSlot);

	float getHeight()
	{
		return mHeight;
	}

	float getWidth()
	{
		return mWidth;
	}
};


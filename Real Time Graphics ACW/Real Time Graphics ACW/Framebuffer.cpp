#include "Framebuffer.h"
#include "DX11Render.h"
#include <directxcolors.h>
#include "Win32Window.h"
#include <algorithm>

bool Framebuffer::loadFramebuffer(const bool pColour, const bool pDepth, const TextureType pType, const unsigned int pNumberOfBuffers)
{
	//TODO: Register framebuffer to be resized
	mUpdateResize = true;
	const auto height = Win32Window::instance()->getHeight();
	const auto width = Win32Window::instance()->getWidth();
	return loadFramebuffer(pColour, pDepth, width, height, pType, pNumberOfBuffers);
}

bool Framebuffer::loadFramebuffer(const bool pColour, const bool pDepth, int pWidth, int pHeight, TextureType pType, unsigned int pNumberOfBuffers)
{
	mWidth = pWidth;
	mHeight = pHeight;
	
	const auto device = Dx11Render::instance()->getDevice();

	if (pColour)
	{
		D3D11_TEXTURE2D_DESC renderTextureDesc;
		ZeroMemory(&renderTextureDesc, sizeof D3D11_TEXTURE2D_DESC);
		renderTextureDesc.Width = pWidth;
		renderTextureDesc.Height = pHeight;
		renderTextureDesc.MipLevels = 1;

		if (pType == TextureType::TEXTURE_2D)
		{
			if (pNumberOfBuffers == 1)
			{
				renderTextureDesc.ArraySize = 1;
			}
			else
			{
				renderTextureDesc.ArraySize = pNumberOfBuffers;
			}
		}
		else
		{
			renderTextureDesc.ArraySize = 6 * pNumberOfBuffers;
		}

		renderTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		renderTextureDesc.SampleDesc.Count = 1;
		renderTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		renderTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		renderTextureDesc.CPUAccessFlags = 0;
		renderTextureDesc.MiscFlags = 0;

		auto result = device->CreateTexture2D(&renderTextureDesc, nullptr, mColorTexture.ReleaseAndGetAddressOf());

		if (FAILED(result))
		{
			return false;
		}

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		ZeroMemory(&renderTargetViewDesc, sizeof D3D11_RENDER_TARGET_VIEW_DESC);
		renderTargetViewDesc.Format = renderTextureDesc.Format;

		if (pType == TextureType::TEXTURE_2D)
		{
			if (pNumberOfBuffers == 1)
			{
				renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				renderTargetViewDesc.Texture2D.MipSlice = 0;

				ID3D11RenderTargetView * renderTargetView = nullptr;
				result = device->CreateRenderTargetView(mColorTexture.Get(), &renderTargetViewDesc, &renderTargetView);

				if (FAILED(result))
				{
					return false;
				}
				
				mColorTextureTargetViews.push_back(renderTargetView);
			}
			else
			{
				renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				renderTargetViewDesc.Texture2DArray.MipSlice = 0;
				renderTargetViewDesc.Texture2DArray.ArraySize = 1;
				
				for (auto i = 0u; i < pNumberOfBuffers; i++)
				{
					renderTargetViewDesc.Texture2DArray.FirstArraySlice = i;

					ID3D11RenderTargetView * renderTargetView = nullptr;
					result = device->CreateRenderTargetView(mColorTexture.Get(), &renderTargetViewDesc, &renderTargetView);

					if (FAILED(result))
					{
						return false;
					}
					
					mColorTextureTargetViews.push_back(renderTargetView);
				}
			}
		}
		else
		{
			renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			renderTargetViewDesc.Texture2DArray.MipSlice = 0;
			renderTargetViewDesc.Texture2DArray.ArraySize = 1;

			for (auto i = 0u; i < pNumberOfBuffers * 6; i++)
			{
				renderTargetViewDesc.Texture2DArray.FirstArraySlice = i;

				ID3D11RenderTargetView * renderTargetView = nullptr;
				result = device->CreateRenderTargetView(mColorTexture.Get(), &renderTargetViewDesc, &renderTargetView);

				if (FAILED(result))
				{
					return false;
				}
				
				mColorTextureTargetViews.push_back(renderTargetView);
			}
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ZeroMemory(&shaderResourceViewDesc, sizeof D3D11_SHADER_RESOURCE_VIEW_DESC);
		shaderResourceViewDesc.Format = renderTextureDesc.Format;

		if (pType == TextureType::TEXTURE_2D)
		{
			if (pNumberOfBuffers == 1)
			{
				shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
				shaderResourceViewDesc.Texture2D.MipLevels = 1;

				ID3D11ShaderResourceView * shaderResourceView = nullptr;
				result = device->CreateShaderResourceView(mColorTexture.Get(), &shaderResourceViewDesc, &shaderResourceView);

				if (FAILED(result))
				{
					return false;
				}
				
				mColorTextureResourceViews.push_back(shaderResourceView);
			}
			else
			{
				shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				shaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
				shaderResourceViewDesc.Texture2DArray.MipLevels = 1;
				shaderResourceViewDesc.Texture2DArray.ArraySize = 1;

				for (auto i = 0u; i < pNumberOfBuffers; i++)
				{
					shaderResourceViewDesc.Texture2DArray.FirstArraySlice = i;

					ID3D11ShaderResourceView * shaderResourceView = nullptr;
					result = device->CreateShaderResourceView(mColorTexture.Get(), &shaderResourceViewDesc, &shaderResourceView);

					if (FAILED(result))
					{
						return false;
					}
					
					mColorTextureResourceViews.push_back(shaderResourceView);
				}
			}
		}
		else
		{
			if (pNumberOfBuffers == 1)
			{
				shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
				shaderResourceViewDesc.TextureCube.MipLevels = 1;

				ID3D11ShaderResourceView * shaderResourceView = nullptr;
				result = device->CreateShaderResourceView(mColorTexture.Get(), &shaderResourceViewDesc, &shaderResourceView);

				if (FAILED(result))
				{
					return false;
				}
				
				mColorTextureResourceViews.push_back(shaderResourceView);
			}
			else
			{
				shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
				shaderResourceViewDesc.TextureCubeArray.MostDetailedMip = 0;
				shaderResourceViewDesc.TextureCubeArray.MipLevels = 1;
				shaderResourceViewDesc.TextureCubeArray.NumCubes = 1;

				for (auto i = 0u; i < pNumberOfBuffers; i++)
				{
					shaderResourceViewDesc.TextureCubeArray.First2DArrayFace = i * 6;

					ID3D11ShaderResourceView * shaderResourceView = nullptr;
					result = device->CreateShaderResourceView(mColorTexture.Get(), &shaderResourceViewDesc, &shaderResourceView);

					if (FAILED(result))
					{
						return false;
					}
					
					mColorTextureResourceViews.push_back(shaderResourceView);
				}
			}
		}
	}
	
	D3D11_TEXTURE2D_DESC depthTextureDesc;
	ZeroMemory(&depthTextureDesc, sizeof D3D11_TEXTURE2D_DESC);
	depthTextureDesc.Width = pWidth;
	depthTextureDesc.Height = pHeight;
	depthTextureDesc.MipLevels = 1;

	if (pType == TextureType::TEXTURE_2D)
	{
		depthTextureDesc.ArraySize = 1;
		depthTextureDesc.MiscFlags = 0;
	}
	else
	{
		depthTextureDesc.ArraySize = 6;
		depthTextureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	}
	
	depthTextureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.SampleDesc.Quality = 0;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthTextureDesc.CPUAccessFlags = 0;

	auto result = device->CreateTexture2D(&depthTextureDesc, nullptr, mDepthTexture.ReleaseAndGetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC depthStateDesc;
	ZeroMemory(&depthStateDesc, sizeof D3D11_DEPTH_STENCIL_DESC);
	depthStateDesc.DepthEnable = true;
	depthStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStateDesc.StencilEnable = false;
	depthStateDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStateDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	depthStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

	depthStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

	result = device->CreateDepthStencilState(&depthStateDesc, mDepthState.ReleaseAndGetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
	ZeroMemory(&depthViewDesc, sizeof D3D11_DEPTH_STENCIL_VIEW_DESC);
	depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthViewDesc.Flags = 0;

	if (pType == TextureType::TEXTURE_2D)
	{
		depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthViewDesc.Texture2D.MipSlice = 0;
	}
	else
	{
		depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		depthViewDesc.Texture2DArray.MipSlice = 0;
		depthViewDesc.Texture2DArray.FirstArraySlice = 0;
		depthViewDesc.Texture2DArray.ArraySize = 6;
	}

	result = device->CreateDepthStencilView(mDepthTexture.Get(), &depthViewDesc, mDepthTextureTargetView.ReleaseAndGetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	if (pDepth)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ZeroMemory(&shaderResourceViewDesc, sizeof D3D11_SHADER_RESOURCE_VIEW_DESC);
		shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;

		if (pType == TextureType::TEXTURE_2D)
		{
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;
		}
		else
		{
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
			shaderResourceViewDesc.TextureCube.MipLevels = 1;
		}

		result = device->CreateShaderResourceView(mDepthTexture.Get(), &shaderResourceViewDesc, mDepthTextureResourceView.ReleaseAndGetAddressOf());

		if (FAILED(result))
		{
			return false;
		}
	}

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof D3D11_SAMPLER_DESC);
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	result = device->CreateSamplerState(&samplerDesc, &mSampler);

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Framebuffer::useFramebuffer() const
{
	Dx11Render::instance()->setViewport(mWidth, mHeight);
	
	const auto deviceContext = Dx11Render::instance()->getDeviceContext();

	for (auto i = 0u; i < mColorTextureResourceViews.size(); i++)
	{
		deviceContext->ClearRenderTargetView(mColorTextureTargetViews[i], DirectX::Colors::MidnightBlue);
	}
	
	deviceContext->ClearDepthStencilView(mDepthTextureTargetView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);

	deviceContext->OMSetRenderTargets(mColorTextureTargetViews.size(), mColorTextureTargetViews.data(), mDepthTextureTargetView.Get());
}

void Framebuffer::useTexture(unsigned int)
{
	//TODO: Implement
}

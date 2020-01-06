#pragma once

#include <d3d11_1.h>
#include <WRL/client.h>
#include "Win32Window.h"
#include "Framebuffer.h"
#include <algorithm>

struct ModelBuffer;
struct MaterialBuffer;
struct CameraBuffer;
struct DirectionalLightBuffer;
struct PointLightBuffer;
struct SpotLightBuffer;
struct ShadowMatrixBuffer;
struct ShadowLightBuffer;
struct GlobalBuffer;

class Dx11Render
{
	static Dx11Render * mInstance;

	D3D_DRIVER_TYPE mDriverType;
	D3D_FEATURE_LEVEL mFeatureLevel;
	Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> mSwapChain1;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState;
	Microsoft::WRL::ComPtr<ID3D11Debug> mDebug;
	Microsoft::WRL::ComPtr<ID3D11BlendState> mBlendState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> mNoBlendState;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mModelBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mCameraBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mDirectionalLightBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mPointLightBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mSpotLightBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mMaterialBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mShadowMatrixBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mShadowLightBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mGlobalBuffer = nullptr;
	
	Dx11Render();
	bool loadRender();

	std::vector<Framebuffer *> mResizeableFramebuffer;
	
public:
	static Dx11Render * instance();

	void clearRenderTargetView(const DirectX::XMVECTORF32 & pColor) const
	{
		mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), pColor);
		mDeviceContext->ClearDepthStencilView(mDepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
	}

	void bindDefaultFramebuffer() const
	{
		mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthView.Get());
	}

	void present() const
	{
		mSwapChain->Present(1, 0);
	}

	void getDevice(Microsoft::WRL::ComPtr<ID3D11Device> & pDevice) const
	{
		pDevice = mDevice;
	}

	void getDeviceContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> & pDeviceContext) const
	{
		pDeviceContext = mDeviceContext;
	}

	void setViewport(const unsigned int pWidth, const unsigned int pHeight)
	{
		D3D11_VIEWPORT viewport;
		viewport.Width = static_cast<float>(pWidth);
		viewport.Height = static_cast<float>(pHeight);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;

		mDeviceContext->RSSetViewports(1, &viewport);
	}

	void defaultViewport()
	{
		D3D11_VIEWPORT viewport;
		viewport.Width = static_cast<float>(Win32Window::instance()->getWidth());
		viewport.Height = static_cast<float>(Win32Window::instance()->getHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;

		mDeviceContext->RSSetViewports(1, &viewport);
	}

	void enableBlend()
	{
		std::vector<float> blendFactor{ 0.0f, 0.0f, 0.0f, 0.0f };
		mDeviceContext->OMSetBlendState(mBlendState.Get(), blendFactor.data(), 0xffffffff);
	}
	 
	void disableBlend()
	{
		std::vector<float> blendFactor{ 0.0f, 0.0f, 0.0f, 0.0f };
		mDeviceContext->OMSetBlendState(mNoBlendState.Get(), blendFactor.data(), 0xffffffff);
	}

	void addFramebuffer(Framebuffer * const pFramebuffer)
	{
		mResizeableFramebuffer.push_back(pFramebuffer);
	}

	void removeFramebuffer(Framebuffer * const pFramebuffer)
	{
		const auto it = std::find(mResizeableFramebuffer.begin(), mResizeableFramebuffer.end(), pFramebuffer);

		if (it != mResizeableFramebuffer.end())
		{
			mResizeableFramebuffer.erase(it);
		}
	}

	void useGlobalBuffer(const GlobalBuffer & pGlobalBuffer) const;
	void useModelBuffer(const ModelBuffer & pModelBuffer) const;
	void useMaterialBuffer(const MaterialBuffer & pMaterialBuffer) const;
	void useCameraBuffer(const CameraBuffer & pCameraBuffer) const;
	void useDirectionalLightBuffer(const DirectionalLightBuffer & pDirectionalLightBuffer) const;
	void usePointLightBuffer(const PointLightBuffer & pPointLightBuffer) const;
	void useSpotLightBuffer(const SpotLightBuffer & pSpotLightBuffer) const;
	void useShadowMatrixBuffer(const ShadowMatrixBuffer & pShadowMatrixBuffer) const;
	void useShadowLightBuffer(const ShadowLightBuffer & pShadowLightBuffer) const;
	
	bool resize(int pWidth, int pHeight);
	
	~Dx11Render();
	Dx11Render(const Dx11Render&) = delete;
	Dx11Render(Dx11Render &&) = delete;
	Dx11Render & operator= (const Dx11Render &) = delete;
	Dx11Render & operator= (Dx11Render &&) = delete;
};


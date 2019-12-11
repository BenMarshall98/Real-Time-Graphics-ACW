#pragma once

#include <d3d11_1.h>
#include <WRL/client.h>

struct ModelBuffer;
struct MaterialBuffer;
struct CameraBuffer;
struct DirectionalLightBuffer;
struct PointLightBuffer;
struct SpotLightBuffer;

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

	Microsoft::WRL::ComPtr<ID3D11Buffer> mModelBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mCameraBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mDirectionalLightBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mPointLightBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mSpotLightBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mMaterialBuffer = nullptr;

	Dx11Render();
	
public:
	static Dx11Render * instance();

	void clearRenderTargetView(const float pColor[4]) const
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
		auto result =mSwapChain->Present(0, 0);
		result = mDevice->GetDeviceRemovedReason();
	}

	Microsoft::WRL::ComPtr<ID3D11Device> getDevice() const
	{
		return mDevice;
	}

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> getDeviceContext() const
	{
		return mDeviceContext;
	}

	void useModelBuffer(const ModelBuffer & pModelBuffer) const;
	void useMaterialBuffer(const MaterialBuffer & pMaterialBuffer) const;
	void useCameraBuffer(const CameraBuffer & pCameraBuffer) const;
	void useDirectionalLightBuffer(const DirectionalLightBuffer & pDirectionalLightBuffer) const;
	void usePointLightBuffer(const PointLightBuffer & pPointLightBuffer) const;
	void useSpotLightBuffer(const SpotLightBuffer & pSPotLightBuffer) const;
	
	void resize(int pWidth, int pHeight);
	
	~Dx11Render() = default;
	Dx11Render(const Dx11Render&) = delete;
	Dx11Render(Dx11Render &&) = delete;
	Dx11Render & operator= (const Dx11Render &) = delete;
	Dx11Render & operator= (Dx11Render &&) = delete;
};


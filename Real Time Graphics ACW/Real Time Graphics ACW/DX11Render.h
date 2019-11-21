#pragma once

#include <d3d11_1.h>

class Dx11Render
{
	static Dx11Render * mInstance;

	D3D_DRIVER_TYPE mDriverType;
	D3D_FEATURE_LEVEL mFeatureLevel;
	ID3D11Device * mDevice;
	ID3D11DeviceContext * mDeviceContext;
	ID3D11DeviceContext1 * mDeviceContext1;
	IDXGISwapChain * mSwapChain;
	IDXGISwapChain1 * mSwapChain1;
	ID3D11RenderTargetView * mRenderTargetView;
	ID3D11DepthStencilView * mDepthView;
	ID3D11RasterizerState * mRasterizerState;

	Dx11Render();
	
public:
	static Dx11Render * instance();

	void clearRenderTargetView(const float pColor[4]) const
	{
		mDeviceContext->ClearRenderTargetView(mRenderTargetView, pColor);
		mDeviceContext->ClearDepthStencilView(mDepthView, D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
	}

	void present() const
	{
		mSwapChain->Present(0, 0);
	}

	ID3D11Device * getDevice() const
	{
		return mDevice;
	}

	ID3D11DeviceContext * getDeviceContext() const
	{
		return mDeviceContext;
	}

	void resize(int pWidth, int pHeight);
	
	~Dx11Render() = default;
	Dx11Render(const Dx11Render&) = delete;
	Dx11Render(Dx11Render &&) = delete;
	Dx11Render & operator= (const Dx11Render &) = delete;
	Dx11Render & operator= (Dx11Render &&) = delete;
};


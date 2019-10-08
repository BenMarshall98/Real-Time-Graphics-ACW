#pragma once

#include <d3d11_1.h>

class DX11Render
{
private:
	static DX11Render * instance;

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Device * device;
	ID3D11Device1 * device1;
	ID3D11DeviceContext * deviceContext;
	ID3D11DeviceContext1 * deviceContext1;
	IDXGISwapChain * swapChain;
	IDXGISwapChain1 * swapChain1;
	ID3D11RenderTargetView * renderTargetView;
	ID3D11RasterizerState * rasterizerState;

	DX11Render();
	
public:
	static DX11Render * Instance();

	inline void ClearRenderTargetView(const float color[4])
	{
		deviceContext->ClearRenderTargetView(renderTargetView, color);
	}

	inline void Present()
	{
		swapChain->Present(0, 0);
	}

	inline ID3D11Device * GetDevice()
	{
		return device;
	}

	inline ID3D11DeviceContext * GetDeviceContext()
	{
		return deviceContext;
	}
	
	~DX11Render();
};


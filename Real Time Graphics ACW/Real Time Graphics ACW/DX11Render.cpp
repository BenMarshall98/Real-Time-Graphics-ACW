#include "DX11Render.h"
#include "Win32Window.h"

DX11Render * DX11Render::instance = nullptr;

DX11Render::DX11Render()
{
	Win32Window * window = Win32Window::Instance();

	driverType = D3D_DRIVER_TYPE_HARDWARE;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	HRESULT result;
	result = D3D11CreateDevice(nullptr, driverType, nullptr, 0, featureLevels, 4, D3D11_SDK_VERSION, &device, &featureLevel, &deviceContext);

	IDXGIDevice * DXGIDevice = nullptr;
	result = device->QueryInterface(__uuidof(IDXGIDevice), (void **)&DXGIDevice);

	IDXGIAdapter * DXGIAdapter = nullptr;
	result = DXGIDevice->GetAdapter(&DXGIAdapter);

	IDXGIFactory1 * DXGIFactory1 = nullptr;
	result = DXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&DXGIFactory1);

	IDXGIFactory2 * DXGIFactory2 = nullptr;
	result = DXGIFactory1->QueryInterface(__uuidof(IDXGIFactory1), (void **)&DXGIFactory2);

	result = device->QueryInterface(__uuidof(ID3D11Device1), (void **)&deviceContext1);
	result = deviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void **)&deviceContext1);

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.Width = window->GetWidth();
	swapChainDesc.Height = window->GetHeight();
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;

	DXGIFactory2->CreateSwapChainForHwnd(device, window->GetHWND(), &swapChainDesc, nullptr, nullptr, &swapChain1);

	result = swapChain1->QueryInterface(__uuidof(IDXGISwapChain), (void **)&swapChain);

	ID3D11Texture2D * backBuffer;
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&backBuffer);

	device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);

	deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);

	D3D11_VIEWPORT viewport;
	viewport.Width = (float)window->GetWidth();
	viewport.Height = (float)window->GetHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	deviceContext->RSSetViewports(1, &viewport);
}

DX11Render::~DX11Render()
{
}

DX11Render * DX11Render::Instance()
{
	if (!instance)
	{
		instance = new DX11Render();
	}

	return instance;
}
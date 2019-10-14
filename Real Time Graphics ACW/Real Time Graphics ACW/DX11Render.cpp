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

	//Create Color Backbuffer
	ID3D11Texture2D * backBuffer;
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&backBuffer);

	device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);

	//Create Depth / Stencil BackBuffer
	D3D11_TEXTURE2D_DESC depthTextureDesc;
	depthTextureDesc.Width = window->GetWidth();
	depthTextureDesc.Height = window->GetHeight();
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.SampleDesc.Quality = 0;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.CPUAccessFlags = 0;
	depthTextureDesc.MiscFlags = 0;

	ID3D11Texture2D * depthStencil = nullptr;
	result = device->CreateTexture2D(&depthTextureDesc, nullptr, &depthStencil);

	D3D11_DEPTH_STENCIL_DESC depthStateDesc;
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

	ID3D11DepthStencilState * depthState;

	result = device->CreateDepthStencilState(&depthStateDesc, &depthState);

	deviceContext->OMSetDepthStencilState(depthState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
	depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Flags = 0;
	depthViewDesc.Texture2D.MipSlice = 0;

	result = device->CreateDepthStencilView(depthStencil, &depthViewDesc, &depthView);

	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthView);

	D3D11_VIEWPORT viewport;
	viewport.Width = (float)window->GetWidth();
	viewport.Height = (float)window->GetHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	deviceContext->RSSetViewports(1, &viewport);

	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	result = device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	deviceContext->RSSetState(rasterizerState);
}

void DX11Render::Resize(int width, int height)
{
	//swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_UNKNOWN, 0);
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
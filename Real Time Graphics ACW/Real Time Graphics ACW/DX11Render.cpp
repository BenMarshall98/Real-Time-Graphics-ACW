#include "DX11Render.h"
#include "Win32Window.h"

Dx11Render * Dx11Render::mInstance = nullptr;

Dx11Render::Dx11Render() : mFeatureLevel(D3D_FEATURE_LEVEL_10_0), mDevice(nullptr), mDeviceContext(nullptr),
	mDeviceContext1(nullptr), mSwapChain(nullptr), mSwapChain1(nullptr), mRenderTargetView(nullptr), mDepthView(nullptr),
	mRasterizerState(nullptr)
{
	auto window = Win32Window::instance();

	mDriverType = D3D_DRIVER_TYPE_HARDWARE;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	auto result = D3D11CreateDevice(nullptr, mDriverType, nullptr, 0, featureLevels, 4, D3D11_SDK_VERSION, &mDevice, &mFeatureLevel, &mDeviceContext);

	IDXGIDevice * dxgiDevice = nullptr;
	result = mDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void **>(&dxgiDevice));

	IDXGIAdapter * dxgiAdapter = nullptr;
	result = dxgiDevice->GetAdapter(&dxgiAdapter);

	IDXGIFactory1 * dxgiFactory1 = nullptr;
	result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void **>(&dxgiFactory1));

	IDXGIFactory2 * dxgiFactory2 = nullptr;
	result = dxgiFactory1->QueryInterface(__uuidof(IDXGIFactory1), reinterpret_cast<void **>(&dxgiFactory2));

	result = mDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void **>(&mDeviceContext1));
	result = mDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void **>(&mDeviceContext1));

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.Width = window->getWidth();
	swapChainDesc.Height = window->getHeight();
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;

	dxgiFactory2->CreateSwapChainForHwnd(mDevice, window->getHwnd(), &swapChainDesc, nullptr, nullptr, &mSwapChain1);

	result = mSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void **>(&mSwapChain));

	//Create Color Backbuffer
	ID3D11Texture2D * backBuffer;
	result = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&backBuffer));

	mDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);

	//Create Depth / Stencil BackBuffer
	D3D11_TEXTURE2D_DESC depthTextureDesc;
	depthTextureDesc.Width = window->getWidth();
	depthTextureDesc.Height = window->getHeight();
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
	result = mDevice->CreateTexture2D(&depthTextureDesc, nullptr, &depthStencil);

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

	result = mDevice->CreateDepthStencilState(&depthStateDesc, &depthState);

	mDeviceContext->OMSetDepthStencilState(depthState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
	depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Flags = 0;
	depthViewDesc.Texture2D.MipSlice = 0;

	result = mDevice->CreateDepthStencilView(depthStencil, &depthViewDesc, &mDepthView);

	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthView);

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(window->getWidth());
	viewport.Height = static_cast<float>(window->getHeight());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	mDeviceContext->RSSetViewports(1, &viewport);

	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	result = mDevice->CreateRasterizerState(&rasterizerDesc, &mRasterizerState);
	mDeviceContext->RSSetState(mRasterizerState);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof bd);

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ModelBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	result = mDevice->CreateBuffer(&bd, nullptr, &mModelBuffer);
}

void Dx11Render::useModelBuffer(const ModelBuffer& pModelBuffer) const
{
	mDeviceContext->UpdateSubresource(mModelBuffer, 0, nullptr, &pModelBuffer, 0, 0);
	mDeviceContext->VSSetConstantBuffers(1, 1, &mModelBuffer);
}

void Dx11Render::resize(int pWidth, int pHeight)
{
	//swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_UNKNOWN, 0);
}

Dx11Render * Dx11Render::instance()
{
	if (!mInstance)
	{
		mInstance = new Dx11Render();
	}

	return mInstance;
}
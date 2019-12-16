#include "DX11Render.h"
#include "Win32Window.h"
#include "Material.h"
#include "Shape.h"
#include "Camera.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"

Dx11Render * Dx11Render::mInstance = nullptr;

Dx11Render::Dx11Render() : mFeatureLevel(D3D_FEATURE_LEVEL_10_0), mDevice(nullptr), mDeviceContext(nullptr),
	mSwapChain(nullptr), mSwapChain1(nullptr), mRenderTargetView(nullptr), mDepthView(nullptr),
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

	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	result = mDevice.As(&dxgiDevice);

	Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
	result = dxgiDevice->GetAdapter(&dxgiAdapter);

	Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory1 = nullptr;
	result = dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory1));

	Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory2 = nullptr;
	result = dxgiFactory1.As(&dxgiFactory2);

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.Width = window->getWidth();
	swapChainDesc.Height = window->getHeight();
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;

	dxgiFactory2->CreateSwapChainForHwnd(mDevice.Get(), window->getHwnd(), &swapChainDesc, nullptr, nullptr, &mSwapChain1);

	result = mSwapChain1->QueryInterface(IID_PPV_ARGS(&mSwapChain));

	//Create Color Backbuffer
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	result = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

	result = mDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, mRenderTargetView.ReleaseAndGetAddressOf());

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

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil = nullptr;
	result = mDevice->CreateTexture2D(&depthTextureDesc, nullptr, depthStencil.ReleaseAndGetAddressOf());

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

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthState = nullptr;

	result = mDevice->CreateDepthStencilState(&depthStateDesc, depthState.GetAddressOf());

	mDeviceContext->OMSetDepthStencilState(depthState.Get(), 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
	depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Flags = 0;
	depthViewDesc.Texture2D.MipSlice = 0;

	result = mDevice->CreateDepthStencilView(depthStencil.Get(), &depthViewDesc, mDepthView.GetAddressOf());

	mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthView.Get());

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(window->getWidth());
	viewport.Height = static_cast<float>(window->getHeight());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	mDeviceContext->RSSetViewports(1, &viewport);

	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	result = mDevice->CreateRasterizerState(&rasterizerDesc, mRasterizerState.ReleaseAndGetAddressOf());
	mDeviceContext->RSSetState(mRasterizerState.Get());

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof bd);

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ModelBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	result = mDevice->CreateBuffer(&bd, nullptr, mModelBuffer.ReleaseAndGetAddressOf());

	bd.ByteWidth = sizeof(MaterialBuffer);

	result = mDevice->CreateBuffer(&bd, nullptr, mMaterialBuffer.ReleaseAndGetAddressOf());
	
	bd.ByteWidth = sizeof(CameraBuffer);

	result = mDevice->CreateBuffer(&bd, nullptr, mCameraBuffer.ReleaseAndGetAddressOf());

	bd.ByteWidth = sizeof(DirectionalLightBuffer);

	result = mDevice->CreateBuffer(&bd, nullptr, mDirectionalLightBuffer.ReleaseAndGetAddressOf());

	bd.ByteWidth = sizeof(PointLightBuffer);

	result = mDevice->CreateBuffer(&bd, nullptr, mPointLightBuffer.ReleaseAndGetAddressOf());

	bd.ByteWidth = sizeof(SpotLightBuffer);

	result = mDevice->CreateBuffer(&bd, nullptr, mSpotLightBuffer.ReleaseAndGetAddressOf());
}

void Dx11Render::useModelBuffer(const ModelBuffer& pModelBuffer) const
{
	mDeviceContext->UpdateSubresource(mModelBuffer.Get(), 0, nullptr, &pModelBuffer, 0, 0);
	mDeviceContext->VSSetConstantBuffers(1, 1, mModelBuffer.GetAddressOf());
	mDeviceContext->DSSetConstantBuffers(1, 1, mModelBuffer.GetAddressOf());
}

void Dx11Render::useMaterialBuffer(const MaterialBuffer& pMaterialBuffer) const
{
	mDeviceContext->UpdateSubresource(mMaterialBuffer.Get(), 0, nullptr, &pMaterialBuffer, 0, 0);
	mDeviceContext->PSSetConstantBuffers(1, 1, mMaterialBuffer.GetAddressOf());
	mDeviceContext->VSSetConstantBuffers(2, 1, mMaterialBuffer.GetAddressOf());
}

void Dx11Render::useCameraBuffer(const CameraBuffer& pCameraBuffer) const
{
	mDeviceContext->UpdateSubresource(mCameraBuffer.Get(), 0, nullptr, &pCameraBuffer, 0, 0);
	mDeviceContext->VSSetConstantBuffers(0, 1, mCameraBuffer.GetAddressOf());
	mDeviceContext->DSSetConstantBuffers(0, 1, mCameraBuffer.GetAddressOf());
}

void Dx11Render::useDirectionalLightBuffer(const DirectionalLightBuffer& pDirectionalLightBuffer) const
{
	mDeviceContext->UpdateSubresource(mDirectionalLightBuffer.Get(), 0, nullptr, &pDirectionalLightBuffer, 0, 0);
	mDeviceContext->PSSetConstantBuffers(2, 1, mDirectionalLightBuffer.GetAddressOf());
	mDeviceContext->VSSetConstantBuffers(3, 1, mDirectionalLightBuffer.GetAddressOf());
}

void Dx11Render::usePointLightBuffer(const PointLightBuffer& pPointLightBuffer) const
{
	mDeviceContext->UpdateSubresource(mPointLightBuffer.Get(), 0, nullptr, &pPointLightBuffer, 0, 0);
	mDeviceContext->PSSetConstantBuffers(3, 1, mPointLightBuffer.GetAddressOf());
	mDeviceContext->VSSetConstantBuffers(4, 1, mPointLightBuffer.GetAddressOf());
}

void Dx11Render::useSpotLightBuffer(const SpotLightBuffer& pSpotLightBuffer) const
{
	mDeviceContext->UpdateSubresource(mSpotLightBuffer.Get(), 0, nullptr, &pSpotLightBuffer, 0, 0);
	mDeviceContext->PSSetConstantBuffers(4, 1, mSpotLightBuffer.GetAddressOf());
	mDeviceContext->VSSetConstantBuffers(5, 1, mSpotLightBuffer.GetAddressOf());
}

bool Dx11Render::resize(int pWidth, int pHeight)
{
	mRenderTargetView.Reset();
	mDepthView.Reset();
	
	mSwapChain->ResizeBuffers(1, pWidth, pHeight, DXGI_FORMAT_UNKNOWN, 0);

	//Recreate Color Backbuffer
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	auto result = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

	if (FAILED(result))
	{
		return false;
	}

	result = mDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, mRenderTargetView.ReleaseAndGetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	//Create Depth / Stencil BackBuffer
	D3D11_TEXTURE2D_DESC depthTextureDesc;
	depthTextureDesc.Width = pWidth;
	depthTextureDesc.Height = pHeight;
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.SampleDesc.Quality = 0;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.CPUAccessFlags = 0;
	depthTextureDesc.MiscFlags = 0;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil = nullptr;
	result = mDevice->CreateTexture2D(&depthTextureDesc, nullptr, depthStencil.ReleaseAndGetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
	depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Flags = 0;
	depthViewDesc.Texture2D.MipSlice = 0;

	result = mDevice->CreateDepthStencilView(depthStencil.Get(), &depthViewDesc, mDepthView.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthView.Get());

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(pWidth);
	viewport.Height = static_cast<float>(pHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	mDeviceContext->RSSetViewports(1, &viewport);

	return true;
}

Dx11Render * Dx11Render::instance()
{
	if (!mInstance)
	{
		mInstance = new Dx11Render();
	}

	return mInstance;
}
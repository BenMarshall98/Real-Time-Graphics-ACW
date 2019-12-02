#include "Game.h"
#include "Win32Window.h"
#include "DX11Render.h"

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <Windows.h>

#include "ModelLoader.h"

#include <string>
#include "IdentityNode.h"
#include <fstream>

double Game::mDt = 0.0f;
Camera * Game::mCamera = nullptr;

Game::Game()
{
	auto device = Dx11Render::instance()->getDevice();

	mShader = new Shader(L"VertexShader.hlsl", L"PixelShader.hlsl");

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CameraBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	auto hr = device->CreateBuffer(&bd, nullptr, &mCameraBuffer);
	if (FAILED(hr))
	{
		
	}

	bd.ByteWidth = sizeof(ModelBuffer);

	hr = device->CreateBuffer(&bd, nullptr, &mModelBuffer);

	// Initialize the world matrix
	mWorld = DirectX::XMMatrixIdentity();

	// Initialize the view matrix
	const auto eye = DirectX::XMFLOAT3(0.0f, 1.0f, -5.0f);
	const auto at = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	const auto up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

	mCamera = new Camera(eye, up, at);

	// Initialize the projection matrix
	mProjection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, Win32Window::instance()->getWidth() / (float)Win32Window::instance()->getHeight(), 0.01f, 100.0f);

	std::ifstream in("configuration.txt");

	node = std::make_shared<IdentityNode>();

	in >> *node;

	QueryPerformanceFrequency(&mTimer);
	mFreq = double(mTimer.QuadPart);

	QueryPerformanceCounter(&mTimer);
	mStart = mTimer.QuadPart;
}

void Game::run()
{
	while(Win32Window::instance()->windowEvents())
	{
		Dx11Render::instance()->clearRenderTargetView(DirectX::Colors::MidnightBlue);

		mShader->useShader();

		auto deviceContext = Dx11Render::instance()->getDeviceContext();
		// Update our time

		QueryPerformanceCounter(&mTimer);
		mStop = mTimer.QuadPart;
		mDt = double(mStop - mStart) / mFreq;

		mDt2 += mDt;

		mStart = mStop;
		//
		// Animate the cube
		//

		//
		// Update variables
		//
		CameraBuffer cb;
		//cb.mWorld = XMMatrixTranspose(g_World);
		const auto viewMatrix = mCamera->getViewMatrix();
		
		cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&viewMatrix));
		cb.mProjection = XMMatrixTranspose(mProjection);
		deviceContext->UpdateSubresource(mCameraBuffer, 0, nullptr, &cb, 0, 0);

		//
		// Renders a triangle
		//
		deviceContext->VSSetConstantBuffers(0, 1, &mCameraBuffer);

		//
		// Present our back buffer to our front buffer
		//
		Dx11Render::instance()->present();
	}
}
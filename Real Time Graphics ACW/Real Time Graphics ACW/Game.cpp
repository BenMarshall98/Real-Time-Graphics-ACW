#include "Game.h"
#include "Win32Window.h"
#include "DX11Render.h"

#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <Windows.h>

#include "ModelLoader.h"

#include <string>

double Game::DT = 0.0f;
Camera * Game::camera = nullptr;

Game::Game()
{
	ID3D11Device * device = DX11Render::Instance()->GetDevice();
	ID3D11DeviceContext * deviceContext = DX11Render::Instance()->GetDeviceContext();
	
	HRESULT hr;
	shader = new Shader(L"VertexShader.hlsl", L"PixelShader.hlsl");

	model = ModelLoader::CreateSphere(1, 20);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CameraBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = device->CreateBuffer(&bd, nullptr, &cameraBuffer);
	if (FAILED(hr))
	{
		
	}

	bd.ByteWidth = sizeof(ModelBuffer);

	hr = device->CreateBuffer(&bd, nullptr, &modelBuffer);

	// Initialize the world matrix
	g_World = DirectX::XMMatrixIdentity();

	// Initialize the view matrix
	DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	DirectX::XMVECTOR At =  DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR Up =  DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	camera = new Camera(Eye, Up, At);

	// Initialize the projection matrix
	g_Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, Win32Window::Instance()->GetWidth() / (FLOAT)Win32Window::Instance()->GetHeight(), 0.01f, 100.0f);

	QueryPerformanceFrequency(&timer);
	freq = double(timer.QuadPart);

	QueryPerformanceCounter(&timer);
	start = timer.QuadPart;
}

Game::~Game()
{
}

void Game::Run()
{
	while(Win32Window::Instance()->WindowEvents())
	{
		DX11Render::Instance()->ClearRenderTargetView(DirectX::Colors::MidnightBlue);

		shader->UseShader();

		ID3D11DeviceContext * deviceContext = DX11Render::Instance()->GetDeviceContext();
		// Update our time

		QueryPerformanceCounter(&timer);
		stop = timer.QuadPart;
		DT = double(stop - start) / freq;

		DT2 += DT;

		start = stop;
		//
		// Animate the cube
		//

		//
		// Update variables
		//
		CameraBuffer cb;
		//cb.mWorld = XMMatrixTranspose(g_World);
		cb.mView = XMMatrixTranspose(camera->GetViewMatrix());
		cb.mProjection = XMMatrixTranspose(g_Projection);
		deviceContext->UpdateSubresource(cameraBuffer, 0, nullptr, &cb, 0, 0);

		//
		// Renders a triangle
		//
		deviceContext->VSSetConstantBuffers(0, 1, &cameraBuffer);

		DirectX::XMMATRIX transSun = DirectX::XMMatrixTranslation(0, 0, 10);
		DirectX::XMMATRIX rotSun = DirectX::XMMatrixRotationY(DT2);
		g_World = rotSun * transSun;

		ModelBuffer mb;
		mb.mModel = XMMatrixTranspose(g_World);
		mb.mModelInverse = XMMatrixInverse(nullptr, g_World);
		mb.mColor = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
		deviceContext->UpdateSubresource(modelBuffer, 0, nullptr, &mb, 0, 0);
		deviceContext->VSSetConstantBuffers(1, 1, &modelBuffer);
		
		model->Render();

		DirectX::XMMATRIX transEarth = DirectX::XMMatrixTranslation(15, 0, 0);
		DirectX::XMMATRIX scaleEarth = DirectX::XMMatrixScaling(0.5, 0.5, 0.5);
		DirectX::XMMATRIX rotEarth = DirectX::XMMatrixRotationY(DT2 * 2);
		g_World = transEarth * rotEarth * scaleEarth * transSun;

		mb.mModel = XMMatrixTranspose(g_World);
		mb.mModelInverse = XMMatrixInverse(nullptr, g_World);
		mb.mColor = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		deviceContext->UpdateSubresource(modelBuffer, 0, nullptr, &mb, 0, 0);
		deviceContext->VSSetConstantBuffers(1, 1, &modelBuffer);

		model->Render();

		DirectX::XMMATRIX transMoon = DirectX::XMMatrixTranslation(-5, 0, 0);
		DirectX::XMMATRIX scaleMoon = DirectX::XMMatrixScaling(0.5, 0.5, 0.5);
		DirectX::XMMATRIX rotMoon = DirectX::XMMatrixRotationY(DT2 * 3);
		g_World = transMoon * rotMoon * scaleMoon * transEarth * rotEarth * scaleEarth * transSun;

		mb.mModel = XMMatrixTranspose(g_World);
		mb.mModelInverse = XMMatrixInverse(nullptr, g_World);
		mb.mColor = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		deviceContext->UpdateSubresource(modelBuffer, 0, nullptr, &mb, 0, 0);
		deviceContext->VSSetConstantBuffers(1, 1, &modelBuffer);

		model->Render();
		//
		// Present our back buffer to our front buffer
		//
		DX11Render::Instance()->Present();
	}
}
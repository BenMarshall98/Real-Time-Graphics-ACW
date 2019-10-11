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

HRESULT Game::CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

Game::Game()
{
	ID3D11Device * device = DX11Render::Instance()->GetDevice();
	ID3D11DeviceContext * deviceContext = DX11Render::Instance()->GetDeviceContext();
	
	HRESULT hr;
	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	hr = CompileShaderFromFile(L"Tutorial04.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		
	}

	// Create the vertex shader
	hr = device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = device->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
	{ }
	// Set the input layout
	deviceContext->IASetInputLayout(g_pVertexLayout);

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile(L"Tutorial04.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
	}

	// Create the pixel shader
	hr = device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
	{
		
	}

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

		ID3D11DeviceContext * deviceContext = DX11Render::Instance()->GetDeviceContext();
		// Update our time

		QueryPerformanceCounter(&timer);
		stop = timer.QuadPart;
		DT = double(stop - start) / freq;

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
		deviceContext->VSSetShader(g_pVertexShader, nullptr, 0);
		deviceContext->VSSetConstantBuffers(0, 1, &cameraBuffer);
		deviceContext->PSSetShader(g_pPixelShader, nullptr, 0);

		DirectX::XMMATRIX transSun = DirectX::XMMatrixTranslation(0, 0, 10);
		DirectX::XMMATRIX rotSun = DirectX::XMMatrixRotationY(DT);
		g_World = rotSun * transSun;

		ModelBuffer mb;
		mb.mModel = XMMatrixTranspose(g_World);
		mb.mColor = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
		deviceContext->UpdateSubresource(modelBuffer, 0, nullptr, &mb, 0, 0);
		deviceContext->VSSetConstantBuffers(1, 1, &modelBuffer);
		
		model->Render();

		DirectX::XMMATRIX transEarth = DirectX::XMMatrixTranslation(15, 0, 0);
		DirectX::XMMATRIX scaleEarth = DirectX::XMMatrixScaling(0.5, 0.5, 0.5);
		DirectX::XMMATRIX rotEarth = DirectX::XMMatrixRotationY(DT * 2);
		g_World = transEarth * rotEarth * scaleEarth * transSun;

		mb.mModel = XMMatrixTranspose(g_World);
		mb.mColor = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		deviceContext->UpdateSubresource(modelBuffer, 0, nullptr, &mb, 0, 0);
		deviceContext->VSSetConstantBuffers(1, 1, &modelBuffer);

		model->Render();

		DirectX::XMMATRIX transMoon = DirectX::XMMatrixTranslation(-5, 0, 0);
		DirectX::XMMATRIX scaleMoon = DirectX::XMMatrixScaling(0.5, 0.5, 0.5);
		DirectX::XMMATRIX rotMoon = DirectX::XMMatrixRotationY(DT * 3);
		g_World = transMoon * rotMoon * scaleMoon * transEarth * rotEarth * scaleEarth * transSun;

		mb.mModel = XMMatrixTranspose(g_World);
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
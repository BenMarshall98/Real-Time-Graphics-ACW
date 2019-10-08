#include "Game.h"
#include "Win32Window.h"
#include "DX11Render.h"

#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <Windows.h>

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
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
	// Create vertex buffer
	SimpleVertex vertices[] =
	{
		{ DirectX::XMFLOAT3(0.000000f,0.000000f,1.000000f),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(0.000000f,0.000000f,1.000000f),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.000000f,0.000000f,1.000000f),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.000000f,0.000000f,1.000000f),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.000000f,-0.000000f,1.000000f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(0.000000f,-0.000000f,1.000000f),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(0.866025f,0.000000f,0.500000f),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(0.433013f,0.750000f,0.500000f),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.433013f,0.750000f,0.500000f),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.866025f,0.000000f,0.500000f),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.433013f,-0.750000f,0.500000f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(0.433013f,-0.750000f,0.500000f),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(0.866025f,0.000000f,-0.500000f),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(0.433013f,0.750000f,-0.500000f),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.433013f,0.750000f,-0.500000f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.866025f,0.000000f,-0.500000f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.433013f,-0.750000f,-0.500000f),DirectX:: XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(0.433013f,-0.750000f,-0.500000f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(0.000000f,0.000000f,-1.000000f),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(0.000000f,0.000000f,-1.000000f),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.000000f,0.000000f,-1.000000f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.000000f,0.000000f,-1.000000f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.000000f,-0.000000f,-1.000000f),DirectX:: XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) },
		{ DirectX::XMFLOAT3(0.000000f,-0.000000f,-1.000000f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f) }
	};
	
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 24;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	hr = device->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr))
	{
		
	}

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// Create index buffer
	WORD indices[] =
	{
		0,6,7,
		1,7,8,
		2,8,9,
		3,9,10,
		4,10,11,
		5,11,6,
		6,12,13,
		6,13,7,
		7,13,14,
		7,14,8,
		8,14,15,
		8,15,9,
		9,15,16,
		9,16,10,
		10,16,17,
		10,17,11,
		11,17,12,
		11,12,6,
		12,18,13,
		13,19,14,
		14,20,15,
		15,21,16,
		16,22,17,
		17,23,12
	};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 72;        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	hr = device->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	if (FAILED(hr))
	{
		
	}

	// Set index buffer
	deviceContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// Set primitive topology
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = device->CreateBuffer(&bd, nullptr, &g_pConstantBuffer);
	if (FAILED(hr))
	{
		
	}

	// Initialize the world matrix
	g_World = DirectX::XMMatrixIdentity();

	// Initialize the view matrix
	DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	DirectX::XMVECTOR At =  DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR Up =  DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = DirectX::XMMatrixLookAtLH(Eye, At, Up);

	// Initialize the projection matrix
	g_Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, Win32Window::Instance()->GetWidth() / (FLOAT)Win32Window::Instance()->GetHeight(), 0.01f, 100.0f);
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
		static float t = 0.0f;
		static ULONGLONG timeStart = 0;
		ULONGLONG timeCur = GetTickCount64();
		if (timeStart == 0)
			timeStart = timeCur;
		t = (timeCur - timeStart) / 1000.0f;

		//
		// Animate the cube
		//
		g_World = DirectX::XMMatrixRotationY(t);

		//
		// Update variables
		//
		ConstantBuffer cb;
		cb.mWorld = XMMatrixTranspose(g_World);
		cb.mView = XMMatrixTranspose(g_View);
		cb.mProjection = XMMatrixTranspose(g_Projection);
		deviceContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);

		//
		// Renders a triangle
		//
		deviceContext->VSSetShader(g_pVertexShader, nullptr, 0);
		deviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
		deviceContext->PSSetShader(g_pPixelShader, nullptr, 0);
		deviceContext->DrawIndexed(72, 0, 0);        // 36 vertices needed for 12 triangles in a triangle list

		//
		// Present our back buffer to our front buffer
		//
		DX11Render::Instance()->Present();
	}
}
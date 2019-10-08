#pragma once

#include <directxmath.h>
#include <d3d11_1.h>

struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

struct ConstantBuffer
{
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
};

class Game
{
private:
	ID3D11VertexShader * g_pVertexShader = nullptr;
	ID3D11PixelShader * g_pPixelShader = nullptr;
	ID3D11InputLayout * g_pVertexLayout = nullptr;
	ID3D11Buffer * g_pVertexBuffer = nullptr;
	ID3D11Buffer * g_pIndexBuffer = nullptr;
	ID3D11Buffer * g_pConstantBuffer = nullptr;
	DirectX::XMMATRIX g_World;
	DirectX::XMMATRIX g_View;
	DirectX::XMMATRIX g_Projection;

	HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
public:
	Game();
	~Game();

	void Run();
};


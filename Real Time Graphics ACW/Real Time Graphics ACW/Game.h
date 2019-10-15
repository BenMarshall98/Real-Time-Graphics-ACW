#pragma once

#include <directxmath.h>
#include <d3d11_1.h>
#include "Model.h"
#include "Camera.h"
#include "Shader.h"

struct CameraBuffer
{
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
};

struct ModelBuffer
{
	DirectX::XMMATRIX mModel;
	DirectX::XMMATRIX mModelInverse;
	DirectX::XMFLOAT4 mColor;
};

class Game
{
private:
	LARGE_INTEGER timer;
	ID3D11VertexShader * g_pVertexShader = nullptr;
	ID3D11PixelShader * g_pPixelShader = nullptr;
	ID3D11InputLayout * g_pVertexLayout = nullptr;
	ID3D11Buffer * g_pVertexBuffer = nullptr;
	ID3D11Buffer * g_pIndexBuffer = nullptr;
	ID3D11Buffer * cameraBuffer = nullptr;
	ID3D11Buffer * modelBuffer = nullptr;
	DirectX::XMMATRIX g_World;
	DirectX::XMMATRIX g_Projection;
	double freq;
	double DT2 = 0.0;
	__int64 start;
	__int64 stop;

	Model * model;
	Shader * shader;
public:
	Game();
	~Game();

	static double DT;
	static Camera * camera;

	void Run();
};


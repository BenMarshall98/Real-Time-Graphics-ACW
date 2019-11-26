#pragma once

#include <DirectXMath.h>
#include <d3d11_1.h>
#include "Model.h"
#include "Camera.h"
#include "Shader.h"
#include "SceneGraphNode.h"

struct CameraBuffer
{
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
};

class Game
{
	LARGE_INTEGER mTimer;
	ID3D11Buffer * mCameraBuffer = nullptr;
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mProjection;
	double mFreq;
	double mDt2 = 0.0;
	__int64 mStart;
	__int64 mStop;

	std::unique_ptr<SceneGraphNode> mNode;

	Model * mModel;
	Shader * mShader;
public:
	Game();
	~Game() = default;
	Game(const Game &) = delete;
	Game(Game &&) = delete;
	Game & operator= (const Game &) = delete;
	Game & operator= (Game &&) = delete;

	static double mDt;
	static Camera * mCamera;

	void run();
};


#pragma once

#include <DirectXMath.h>
#include <d3d11_1.h>
#include "Model.h"
#include "Camera.h"
#include "SceneGraphNode.h"

struct MaterialBuffer
{
	DirectX::XMFLOAT4 mAmbient;
	DirectX::XMFLOAT4 mDiffuse;
	DirectX::XMFLOAT4 mSpecular;
	float mShininess;
	DirectX::XMFLOAT3 mPadding;
};

struct CameraBuffer
{
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
};

class Game
{
	LARGE_INTEGER mTimer;
	ID3D11Buffer * mCameraBuffer = nullptr;
	ID3D11Buffer * mDirectionLightBuffer = nullptr;
	ID3D11Buffer * mMaterialBuffer = nullptr;
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mProjection;
	double mFreq;
	double mDt2 = 0.0;
	__int64 mStart;
	__int64 mStop;

	std::unique_ptr<SceneGraphNode> mNode;

	Model * mModel;
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


#pragma once

#include <DirectXMath.h>
#include <d3d11_1.h>
#include "Model.h"
#include "Camera.h"
#include "SceneGraphNode.h"
#include "Texture.h"

class Game
{
	LARGE_INTEGER mTimer;

	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mProjection;
	double mFreq;
	double mDt2 = 0.0;
	__int64 mStart;
	__int64 mStop;

	std::unique_ptr<SceneGraphNode> mNode;
	std::shared_ptr<Texture> mBase;
	std::shared_ptr<Texture> mSpec;
	
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


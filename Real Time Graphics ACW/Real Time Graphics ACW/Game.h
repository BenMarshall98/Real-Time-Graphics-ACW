#pragma once

#include <DirectXMath.h>
#include <d3d11_1.h>
#include "Camera.h"
#include "SceneGraphNode.h"
#include "Texture.h"
#include "Framebuffer.h"

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
	std::shared_ptr<Texture> mDisp;
	std::shared_ptr<Texture> mNorm;

	Framebuffer * mFramebuffer;
	
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


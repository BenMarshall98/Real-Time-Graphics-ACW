#pragma once

#include <DirectXMath.h>
#include <d3d11_1.h>
#include "SceneGraphNode.h"
#include "Texture.h"
#include "InkRender.h"

class Game
{
	LARGE_INTEGER mTimer;

	DirectX::XMMATRIX mWorld;
	double mFreq;
	double mDt2 = 0.0;
	__int64 mStart = 0.0;
	__int64 mStop = 0.0;

	std::unique_ptr<SceneGraphNode> mNode;
	
public:
	Game();
	~Game();
	Game(const Game &) = delete;
	Game(Game &&) = delete;
	Game & operator= (const Game &) = delete;
	Game & operator= (Game &&) = delete;

	static double mDt;

	void run();
	static void clear();
};


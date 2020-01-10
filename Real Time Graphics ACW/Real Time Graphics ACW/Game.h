#pragma once

#include <DirectXMath.h>
#include <d3d11_1.h>
#include "SceneGraphNode.h"
#include "Texture.h"
#include "InkRender.h"

class Game
{
	static LARGE_INTEGER mTimer;

	static DirectX::XMMATRIX mWorld;
	static double mFreq;
	static double mDt2;
	static __int64 mStart;
	static __int64 mStop;
	static float mTimeFactor;
	static double mDt;

	static std::unique_ptr<SceneGraphNode> mNode;
	
public:
	Game();
	~Game() = default;
	Game(const Game &) = delete;
	Game(Game &&) = delete;
	Game & operator= (const Game &) = delete;
	Game & operator= (Game &&) = delete;

	

	void run();
	static void clear();
	static void reset();

	static void increaseTimeFactor()
	{
		mTimeFactor += 0.1f;
	}


	static void decreaseTimeFactor()
	{
		mTimeFactor -= 0.1f;

		if (mTimeFactor < 0.0f)
		{
			mTimeFactor = 0.0f;
		}
	}

	static float getTime()
	{
		return mDt * mTimeFactor;
	}

	static float getNormalTime()
	{
		return mDt;
	}
};


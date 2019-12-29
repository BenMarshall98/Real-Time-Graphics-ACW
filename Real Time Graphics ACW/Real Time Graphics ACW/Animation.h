#pragma once
#include <istream>
#include <DirectXMath.h>

class Animation
{
protected:
	float mEndTime = 0.0f;
	float mCurrentTime = 0.0f;
	unsigned int mCurrentNode = 0u;

public:
	explicit Animation(float pEndTime);
	Animation() = default;
	virtual ~Animation() = default;

	Animation(const Animation &) = delete;
	Animation(Animation &&) = delete;
	Animation & operator= (const Animation &) = delete;
	Animation & operator= (Animation &&) = delete;

	void setEndTime(const float pEndTime)
	{
		mEndTime = pEndTime;
	}

	virtual void calculateTangents() = 0;
	virtual void read(std::istream & pIn) = 0;
	virtual DirectX::XMFLOAT4X4 animate(float pDeltaTime) = 0;
};

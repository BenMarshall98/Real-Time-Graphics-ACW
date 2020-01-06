#pragma once
#include <istream>
#include <DirectXMath.h>

class Animation
{
	float mEndTime = 0.0f;
	float mCurrentTime = 0.0f;
	unsigned int mCurrentNode = 0u;

public:
	explicit Animation(float pEndTime);
	Animation() = default;
	virtual ~Animation();

	Animation(const Animation &) = delete;
	Animation(Animation &&) = delete;
	Animation & operator= (const Animation &) = delete;
	Animation & operator= (Animation &&) = delete;

	void setEndTime(const float pEndTime)
	{
		mEndTime = pEndTime;
	}

	float getEndTime() const
	{
		return mEndTime;
	}

	float getCurrentTime() const
	{
		return mCurrentTime;
	}

	void setCurrentTime(const float pCurrentTime)
	{
		mCurrentTime = pCurrentTime;
	}

	unsigned int getCurrentNode() const
	{
		return mCurrentNode;
	}

	void setCurrentNode(const unsigned int pCurrentNode)
	{
		mCurrentNode = pCurrentNode;
	}

	virtual void calculateTangents() = 0;
	virtual void read(std::istream & pIn) = 0;
	virtual DirectX::XMFLOAT4X4 animate(float pDeltaTime) = 0;
};

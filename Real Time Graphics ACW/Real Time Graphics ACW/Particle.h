#pragma once

#include <DirectXMath.h>

struct State
{
	DirectX::XMVECTOR mPosition;
	DirectX::XMVECTOR mVelocity;
};

struct Derivative
{
	DirectX::XMVECTOR mVelocity;
	DirectX::XMVECTOR mAcceleration;
};

class Particle
{
	DirectX::XMFLOAT3 mPreviousPosition;
	DirectX::XMFLOAT3 mCurrentPosition;
	DirectX::XMFLOAT3 mVelocity;
	float mTime;

	static void integrate(State & pState, float pDt);
	static void evaluate(const State& pInitial, const float pDt, const Derivative& pDerivative, Derivative & pReturnDerivative);
	static void acceleration(const State & pState, DirectX::XMVECTOR & pAcceleration);
	
public:
	Particle(const DirectX::XMFLOAT3 & pPosition, const DirectX::XMFLOAT3 & pVelocity);
	~Particle() = default;

	Particle(const Particle &) = default;
	Particle(Particle &&) = default;
	Particle & operator= (const Particle &) = default;
	Particle & operator= (Particle &&) = default;

	void calculatePhysics(float pDt);
	void update();

	void getPreviousPosition(DirectX::XMFLOAT3 & pPreviousPosition) const
	{
		pPreviousPosition = mPreviousPosition;
	}

	void getCurrentPosition(DirectX::XMFLOAT3 & pCurrentPosition) const
	{
		pCurrentPosition = mCurrentPosition;
	}

	void getVelocity(DirectX::XMFLOAT3 & pVelocity) const
	{
		pVelocity = mVelocity;
	}

	void setTime(const float pTime)
	{
		mTime = pTime;
	}

	float getTime() const
	{
		return mTime;
	}
};


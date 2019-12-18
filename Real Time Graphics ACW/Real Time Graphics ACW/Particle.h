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

	static void integrate(State & pState, float pDt);
	static Derivative evaluate(const State & pInitial, float pDt, const Derivative & pDerivative);
	static DirectX::XMVECTOR acceleration(const State & pState);
	
public:
	Particle(const DirectX::XMFLOAT3 & pPosition, const DirectX::XMFLOAT3 & pVelocity);
	~Particle() = default;

	Particle(const Particle &) = default;
	Particle(Particle &&) = default;
	Particle & operator= (const Particle &) = default;
	Particle & operator= (Particle &&) = default;

	void calculatePhysics(float pDt);
	void update();

	DirectX::XMFLOAT3 getPreviousPosition() const
	{
		return mPreviousPosition;
	}

	DirectX::XMFLOAT3 getCurrentPosition() const
	{
		return mCurrentPosition;
	}

	DirectX::XMFLOAT3 getVelocity() const
	{
		return mVelocity;
	}
};


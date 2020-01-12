#include "Particle.h"

Particle::Particle(const DirectX::XMFLOAT3& pPosition, const DirectX::XMFLOAT3& pVelocity) :
	mPreviousPosition(pPosition), mCurrentPosition(pPosition), mVelocity(pVelocity), mTime(0.0f)
{
}

void Particle::update()
{
	mPreviousPosition = mCurrentPosition;
}

//The below uses Runge-Kutta 4th Order Method to calculate the new position and velocity
//Based on the module Simulation and Artificial Intelligence - 700110

void Particle::calculatePhysics(const float pDt)
{
	State state = {
		XMLoadFloat3(&mCurrentPosition),
		XMLoadFloat3(&mVelocity)
	};

	integrate(state, pDt);

	XMStoreFloat3(&mCurrentPosition, state.mPosition);
	XMStoreFloat3(&mVelocity, state.mVelocity);

	mTime -= pDt;

	if (mTime < 0.0f)
	{
		mTime = 0.0f;
	}
}

void Particle::integrate(State& pState, const float pDt)
{
	const Derivative initial = {
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)
	};

	Derivative k1, k2, k3, k4;
	evaluate(pState, 0.0f, initial, k1);
	evaluate(pState, pDt * 0.5f, k1, k2);
	evaluate(pState, pDt * 0.5f, k2, k3);
	evaluate(pState, pDt, k3, k4);

	const auto dPos = DirectX::XMVectorScale(DirectX::XMVectorAdd(k1.mVelocity, DirectX::XMVectorAdd(DirectX::XMVectorScale(DirectX::XMVectorAdd(k2.mVelocity, k3.mVelocity), 2.0f), k4.mVelocity)), 1.0f / 6.0f);
	const auto dVel = DirectX::XMVectorScale(DirectX::XMVectorAdd(k1.mAcceleration, DirectX::XMVectorAdd(DirectX::XMVectorScale(DirectX::XMVectorAdd(k2.mAcceleration, k3.mAcceleration), 2.0f), k4.mAcceleration)), 1.0f / 6.0f);

	pState.mPosition = DirectX::XMVectorAdd(pState.mPosition, DirectX::XMVectorScale(dPos, pDt));
	pState.mVelocity = DirectX::XMVectorAdd(pState.mVelocity, DirectX::XMVectorScale(dVel, pDt));
}

void Particle::evaluate(const State& pInitial, const float pDt, const Derivative& pDerivative, Derivative & pReturnDerivative)
{
	const State state = {
		DirectX::XMVectorAdd(pInitial.mPosition, DirectX::XMVectorScale(pDerivative.mVelocity, pDt)),
		DirectX::XMVectorAdd(pInitial.mVelocity, DirectX::XMVectorScale(pDerivative.mAcceleration, pDt))
	};
	
	pReturnDerivative = Derivative();
	pReturnDerivative.mVelocity = state.mVelocity;
	acceleration(state, pReturnDerivative.mAcceleration);
}

void Particle::acceleration(const State & pState, DirectX::XMVECTOR & pAcceleration)
{
	const auto drag = DirectX::XMVectorScale(DirectX::XMVectorScale(pState.mVelocity, -1.0f), 0.05f * DirectX::XMVectorGetX(DirectX::XMVector3Length(pState.mVelocity)) + 0.05f * static_cast<float>(pow(DirectX::XMVectorGetX(DirectX::XMVector3Length(pState.mVelocity)), 2)));
	pAcceleration = DirectX::XMVectorAdd(DirectX::XMVectorSet(0.0f, -9.81f, 0.0f, 0.0f), drag);
}

#pragma once

#include <DirectXMath.h>

class Camera
{
	DirectX::XMFLOAT4X4 mViewMatrix;
	DirectX::XMFLOAT3 mUpDirection;
	DirectX::XMFLOAT3 mEyePosition;
	DirectX::XMFLOAT3 mTargetPosition;
	float mAngleSpeed = 360.0f;
	float mMovementSpeed = 1000.0f;
	
public:
	Camera(const DirectX::XMFLOAT3 & pEyePosition, const DirectX::XMFLOAT3 & pUpDirection,
		const DirectX::XMFLOAT3 & pTargetPosition);
	~Camera() = default;
	Camera(const Camera&) = delete;
	Camera(Camera &&) = delete;
	Camera & operator= (const Camera &) = delete;
	Camera & operator= (Camera &&) = delete;

	void rotateLeftRight(bool pLeft = true);
	void rotateUpDown(bool pUp = true);
	void panLeftRight(bool pLeft = true);
	void panForwardBackward(bool pForward = true);
	void panUpDown(bool pUp = true);

	void update();

	DirectX::XMFLOAT4X4 getViewMatrix() const
	{
		return mViewMatrix;
	}
};


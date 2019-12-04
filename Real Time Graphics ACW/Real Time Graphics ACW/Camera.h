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
	
	bool mRotateLeft = false;
	bool mRotateRight = false;
	bool mRotateUp = false;
	bool mRotateDown = false;
	bool mPanLeft = false;
	bool mPanRight = false;
	bool mPanUp = false;
	bool mPanDown = false;
	bool mPanForward = false;
	bool mPanBackward = false;
	
	void rotateLeftRight(bool pLeft = true);
	void rotateUpDown(bool pUp = true);
	void panLeftRight(bool pLeft = true);
	void panForwardBackward(bool pForward = true);
	void panUpDown(bool pUp = true);
	
public:
	Camera(const DirectX::XMFLOAT3 & pEyePosition, const DirectX::XMFLOAT3 & pUpDirection,
		const DirectX::XMFLOAT3 & pTargetPosition);
	~Camera() = default;
	Camera(const Camera&) = delete;
	Camera(Camera &&) = delete;
	Camera & operator= (const Camera &) = delete;
	Camera & operator= (Camera &&) = delete;

	void rotateLeft(const bool & pRotate)
	{
		mRotateLeft = pRotate;
	}
	
	void rotateRight(const bool & pRotate)
	{
		mRotateRight = pRotate;
	}
	
	void rotateUp(const bool & pRotate)
	{
		mRotateUp = pRotate;
	}
	
	void rotateDown(const bool & pRotate)
	{
		mRotateDown = pRotate;
	}
	
	void panLeft(const bool & pPan)
	{
		mPanLeft = pPan;
	}
	
	void panRight(const bool & pPan)
	{
		mPanRight = pPan;
	}
	
	void panUp(const bool & pPan)
	{
		mPanUp = pPan;
	}
	
	void panDown(const bool & pPan)
	{
		mPanDown = pPan;
	}
	
	void panForwards(const bool & pPan)
	{
		mPanForwards = pPan;
	}
	
	void panBackwards(cosnt bool & pPan)
	{
		mPanBackwards = pPan;
	}

	void update();

	DirectX::XMFLOAT4X4 getViewMatrix() const
	{
		return mViewMatrix;
	}
};


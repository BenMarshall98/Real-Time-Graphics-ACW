#pragma once

#include <DirectXMath.h>

class Camera
{
private:
	DirectX::XMMATRIX mViewMatrix;
	DirectX::XMVECTOR mUpDirection;
	DirectX::XMVECTOR mEyePosition;
	DirectX::XMVECTOR mTargetPosition;
	double angleSpeed = 360.0f;
	float movementSpeed = 1000.0f;
	
public:
	Camera(DirectX::XMVECTOR pEyePosition, DirectX::XMVECTOR pUpDirection,
		DirectX::XMVECTOR pTargetPosition);
	~Camera();

	void RotateLeftRight(bool left = true);
	void RotateUpDown(bool up = true);
	void PanLeftRight(bool left = true);
	void PanForwardBackward(bool forward = true);
	void PanUpDown(bool up = true);

	void Update();

	inline DirectX::XMMATRIX GetViewMatrix()
	{
		return mViewMatrix;
	}
};


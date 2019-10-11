#include "Camera.h"
#include "Game.h"

Camera::Camera(DirectX::XMVECTOR pEyePosition, DirectX::XMVECTOR pUpDirection,
	DirectX::XMVECTOR pTargetPosition) : mEyePosition(pEyePosition),
	mUpDirection(pUpDirection), mTargetPosition(pTargetPosition)
{
	Update();
}

Camera::~Camera()
{
}

void Camera::RotateLeftRight(bool left)
{
	double angleChange = angleSpeed * Game::DT;

	if (!left)
	{
		angleChange = -angleChange;
	}

	DirectX::XMVECTOR zAxis = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(mTargetPosition, mEyePosition));
	DirectX::XMVECTOR xAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(zAxis, mUpDirection));
	DirectX::XMVECTOR yAxis = DirectX::XMVector3Cross(zAxis, xAxis);

	DirectX::XMMATRIX leftRightMat = DirectX::XMMatrixRotationNormal(yAxis, angleChange);

	mTargetPosition = DirectX::XMVectorAdd(mEyePosition, DirectX::XMVector3Transform(zAxis, leftRightMat));

	Update();
}

void Camera::RotateUpDown(bool up)
{
	float angleChange = angleSpeed * Game::DT;

	if (!up)
	{
		angleChange = -angleChange;
	}

	DirectX::XMVECTOR zAxis = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(mTargetPosition, mEyePosition));
	DirectX::XMVECTOR xAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(zAxis, mUpDirection));

	DirectX::XMMATRIX leftRightMat = DirectX::XMMatrixRotationNormal(xAxis, angleChange);
	
	mTargetPosition = DirectX::XMVectorAdd(mEyePosition, DirectX::XMVector3Transform(zAxis, leftRightMat));
	mUpDirection = DirectX::XMVector3Transform(mUpDirection, leftRightMat);

	Update();
}

void Camera::PanLeftRight(bool left)
{
	float movementChange = movementSpeed * Game::DT;

	if (!left)
	{
		movementChange = -movementChange;
	}

	DirectX::XMVECTOR zAxis = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(mTargetPosition, mEyePosition));
	DirectX::XMVECTOR xAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(zAxis, mUpDirection));

	DirectX::XMVECTOR movement = DirectX::XMVectorScale(xAxis, movementChange);

	mEyePosition = DirectX::XMVectorAdd(mEyePosition, movement);
	mTargetPosition = DirectX::XMVectorAdd(mTargetPosition, movement);

	Update();
}

void Camera::PanForwardBackward(bool forward)
{
	float movementChange = movementSpeed * Game::DT;

	if (!forward)
	{
		movementChange = -movementChange;
	}

	DirectX::XMVECTOR zAxis = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(mTargetPosition, mEyePosition));

	DirectX::XMVECTOR movement = DirectX::XMVectorScale(zAxis, movementChange);

	mEyePosition = DirectX::XMVectorAdd(mEyePosition, movement);
	mTargetPosition = DirectX::XMVectorAdd(mTargetPosition, movement);

	Update();
}

void Camera::PanUpDown(bool up)
{
	float movementChange = movementSpeed * Game::DT;

	if (!up)
	{
		movementChange = -movementChange;
	}

	DirectX::XMVECTOR zAxis = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(mTargetPosition, mEyePosition));
	DirectX::XMVECTOR xAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(zAxis, mUpDirection));
	DirectX::XMVECTOR yAxis = DirectX::XMVector3Cross(zAxis, xAxis);

	DirectX::XMVECTOR movement = DirectX::XMVectorScale(yAxis, movementChange);

	mEyePosition = DirectX::XMVectorAdd(mEyePosition, movement);
	mTargetPosition = DirectX::XMVectorAdd(mTargetPosition, movement);

	Update();
}

void Camera::Update()
{
	mViewMatrix = DirectX::XMMatrixLookAtLH(mEyePosition, mTargetPosition, mUpDirection);
}
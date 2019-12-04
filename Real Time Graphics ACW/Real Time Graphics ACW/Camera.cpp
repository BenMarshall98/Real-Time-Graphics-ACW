#include "Camera.h"
#include "Game.h"

Camera::Camera(const DirectX::XMFLOAT3 & pEyePosition, const DirectX::XMFLOAT3 & pUpDirection,
	const DirectX::XMFLOAT3 & pTargetPosition) : mViewMatrix(), mUpDirection(pUpDirection),
	mEyePosition(pEyePosition), mTargetPosition(pTargetPosition)
{
	update();
}

void Camera::rotateLeftRight(const bool pLeft)
{
	auto angleChange = mAngleSpeed * Game::mDt;

	if (!pLeft)
	{
		angleChange = -angleChange;
	}

	const auto targetTemp = XMLoadFloat3(&mTargetPosition);
	const auto eyeTemp = XMLoadFloat3(&mEyePosition);
	const auto upTemp = XMLoadFloat3(&mUpDirection);

	const auto zAxis = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(targetTemp, eyeTemp));
	const auto xAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(zAxis, upTemp));
	const auto yAxis = DirectX::XMVector3Cross(zAxis, xAxis);

	const auto leftRightMat = DirectX::XMMatrixRotationNormal(yAxis, angleChange);

	XMStoreFloat3(&mTargetPosition, DirectX::XMVectorAdd(eyeTemp, XMVector3Transform(zAxis, leftRightMat)));
}

void Camera::rotateUpDown(const bool pUp)
{
	auto angleChange = mAngleSpeed * Game::mDt;

	if (!pUp)
	{
		angleChange = -angleChange;
	}

	const auto targetTemp = XMLoadFloat3(&mTargetPosition);
	const auto eyeTemp = XMLoadFloat3(&mEyePosition);
	const auto upTemp = XMLoadFloat3(&mUpDirection);

	const auto zAxis = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(targetTemp, eyeTemp));
	const auto xAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(zAxis, upTemp));

	const auto leftRightMat = DirectX::XMMatrixRotationNormal(xAxis, angleChange);
	
	XMStoreFloat3(&mTargetPosition, DirectX::XMVectorAdd(eyeTemp, XMVector3Transform(zAxis, leftRightMat)));
	XMStoreFloat3(&mUpDirection, XMVector3Transform(upTemp, leftRightMat));
}

void Camera::panLeftRight(const bool pLeft)
{
	auto movementChange = mMovementSpeed * Game::mDt;

	if (!pLeft)
	{
		movementChange = -movementChange;
	}

	const auto targetTemp = XMLoadFloat3(&mTargetPosition);
	const auto eyeTemp = XMLoadFloat3(&mEyePosition);
	const auto upTemp = XMLoadFloat3(&mUpDirection);

	const auto zAxis = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(targetTemp, eyeTemp));
	const auto xAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(zAxis, upTemp));

	const auto movement = DirectX::XMVectorScale(xAxis, movementChange);

	XMStoreFloat3(&mEyePosition, DirectX::XMVectorAdd(eyeTemp, movement));
	XMStoreFloat3(&mTargetPosition, DirectX::XMVectorAdd(targetTemp, movement));
}

void Camera::panForwardBackward(const bool pForward)
{
	auto movementChange = mMovementSpeed * Game::mDt;

	if (!pForward)
	{
		movementChange = -movementChange;
	}

	const auto targetTemp = XMLoadFloat3(&mTargetPosition);
	const auto eyeTemp = XMLoadFloat3(&mEyePosition);

	const auto zAxis = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(targetTemp, eyeTemp));

	const auto movement = DirectX::XMVectorScale(zAxis, movementChange);

	XMStoreFloat3(&mEyePosition, DirectX::XMVectorAdd(eyeTemp, movement));
	XMStoreFloat3(&mTargetPosition, DirectX::XMVectorAdd(targetTemp, movement));
}

void Camera::panUpDown(const bool pUp)
{
	auto movementChange = mMovementSpeed * Game::mDt;

	if (!pUp)
	{
		movementChange = -movementChange;
	}

	const auto targetTemp = XMLoadFloat3(&mTargetPosition);
	const auto eyeTemp = XMLoadFloat3(&mEyePosition);
	const auto upTemp = XMLoadFloat3(&mUpDirection);

	const auto zAxis = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(targetTemp, eyeTemp));
	const auto xAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(zAxis, upTemp));
	const auto yAxis = DirectX::XMVector3Cross(zAxis, xAxis);

	const auto movement = DirectX::XMVectorScale(yAxis, movementChange);

	XMStoreFloat3(&mEyePosition,DirectX::XMVectorAdd(eyeTemp, movement));
	XMStoreFloat3(&mTargetPosition, DirectX::XMVectorAdd(targetTemp, movement));
}

void Camera::update()
{
	if (mRotateLeft && !mRotateRight)
	{
		rotateLeftRight(true);
	}
	else if (!mRotateLeft && mRotateRight)
	{
		rotateLeftRight(false);
	}
	
	if (mRotateUp && !mRotateDown)
	{
		rotateUpDown(true);
	}
	else if (!mRotateUp && mRotateDown)
	{
		rotateUpDown(false);
	}
	
	if (mPanLeft && !mPanRight)
	{
		panLeftRight(true);
	}
	else if (!mPanLeft && mPanRight)
	{
		panLeftRight(false);
	}
	
	if (mPanUp && !mPanDown)
	{
		panUpDown(true);
	}
	else if (!mPanUp && mPanDown)
	{
		panUpDown(false);
	}
	
	if (mPanForward && !mPanBackward)
	{
		panForwardBackward(true);
	}
	else if (!mPanForward && mPanBackward)
	{
		panForwardBackward(false);
	}
	
	const auto targetTemp = XMLoadFloat3(&mTargetPosition);
	const auto eyeTemp = XMLoadFloat3(&mEyePosition);
	const auto upTemp = XMLoadFloat3(&mUpDirection);
	XMStoreFloat4x4(&mViewMatrix, DirectX::XMMatrixLookAtLH(eyeTemp, targetTemp, upTemp));
}
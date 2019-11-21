#include "Camera.h"
#include "Game.h"

//TODO: Do load float3 as temp values for each method

Camera::Camera(const DirectX::XMFLOAT3 & pEyePosition, const DirectX::XMFLOAT3 & pUpDirection,
	const DirectX::XMFLOAT3 & pTargetPosition) : mViewMatrix(), mUpDirection(pUpDirection),
	mEyePosition(pEyePosition), mTargetPosition(pTargetPosition)
{
	update();
}

void Camera::rotateLeftRight(const bool pLeft)
{
	auto angleChange = mAngleSpeed * Game::DT;

	if (!pLeft)
	{
		angleChange = -angleChange;
	}

	const auto zAxis = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(XMLoadFloat3(&mTargetPosition), XMLoadFloat3(&mEyePosition)));
	const auto xAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(zAxis, XMLoadFloat3(&mUpDirection)));
	const auto yAxis = DirectX::XMVector3Cross(zAxis, xAxis);

	const auto leftRightMat = DirectX::XMMatrixRotationNormal(yAxis, angleChange);

	XMStoreFloat3(&mTargetPosition, DirectX::XMVectorAdd(XMLoadFloat3(&mEyePosition), XMVector3Transform(zAxis, leftRightMat)));

	update();
}

void Camera::rotateUpDown(const bool pUp)
{
	auto angleChange = mAngleSpeed * Game::DT;

	if (!pUp)
	{
		angleChange = -angleChange;
	}

	const auto zAxis = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(XMLoadFloat3(&mTargetPosition), XMLoadFloat3(&mEyePosition)));
	const auto xAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(zAxis, XMLoadFloat3(&mUpDirection)));

	const auto leftRightMat = DirectX::XMMatrixRotationNormal(xAxis, angleChange);
	
	XMStoreFloat3(&mTargetPosition, DirectX::XMVectorAdd(XMLoadFloat3(&mEyePosition), XMVector3Transform(zAxis, leftRightMat)));
	XMStoreFloat3(&mUpDirection, XMVector3Transform(XMLoadFloat3(&mUpDirection), leftRightMat));

	update();
}

void Camera::panLeftRight(const bool pLeft)
{
	auto movementChange = mMovementSpeed * Game::DT;

	if (!pLeft)
	{
		movementChange = -movementChange;
	}

	const auto zAxis = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(XMLoadFloat3(&mTargetPosition), XMLoadFloat3(&mEyePosition)));
	const auto xAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(zAxis, XMLoadFloat3(&mUpDirection)));

	const auto movement = DirectX::XMVectorScale(xAxis, movementChange);

	XMStoreFloat3(&mEyePosition, DirectX::XMVectorAdd(XMLoadFloat3(&mEyePosition), movement));
	XMStoreFloat3(&mTargetPosition, DirectX::XMVectorAdd(XMLoadFloat3(&mTargetPosition), movement));

	update();
}

void Camera::panForwardBackward(const bool pForward)
{
	auto movementChange = mMovementSpeed * Game::DT;

	if (!pForward)
	{
		movementChange = -movementChange;
	}

	const auto zAxis = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(XMLoadFloat3(&mTargetPosition), XMLoadFloat3(&mEyePosition)));

	const auto movement = DirectX::XMVectorScale(zAxis, movementChange);

	XMStoreFloat3(&mEyePosition, DirectX::XMVectorAdd(XMLoadFloat3(&mEyePosition), movement));
	XMStoreFloat3(&mTargetPosition, DirectX::XMVectorAdd(XMLoadFloat3(&mTargetPosition), movement));

	update();
}

void Camera::panUpDown(const bool pUp)
{
	auto movementChange = mMovementSpeed * Game::DT;

	if (!pUp)
	{
		movementChange = -movementChange;
	}

	DirectX::XMVECTOR zAxis = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(mTargetPosition, mEyePosition));
	DirectX::XMVECTOR xAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(zAxis, mUpDirection));
	DirectX::XMVECTOR yAxis = DirectX::XMVector3Cross(zAxis, xAxis);

	DirectX::XMVECTOR movement = DirectX::XMVectorScale(yAxis, movementChange);

	mEyePosition = DirectX::XMVectorAdd(mEyePosition, movement);
	mTargetPosition = DirectX::XMVectorAdd(mTargetPosition, movement);

	update();
}

void Camera::update()
{
	mViewMatrix = DirectX::XMMatrixLookAtLH(mEyePosition, mTargetPosition, mUpDirection);
}
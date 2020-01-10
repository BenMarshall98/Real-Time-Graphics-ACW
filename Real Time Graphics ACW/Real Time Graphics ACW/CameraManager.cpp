#include "CameraManager.h"
#include "DX11Render.h"

CameraManager * CameraManager::mInstance = nullptr;

CameraManager::CameraManager()
{
	DirectX::XMStoreFloat4x4(&mPerspective, DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, static_cast<float>(Win32Window::instance()->getWidth()) / static_cast<float>(Win32Window::instance()->getWidth()), 0.01f, 20.0f));
}

CameraManager::~CameraManager()
{
	mInstance = nullptr;
}

void CameraManager::useCamera1()
{
	mCurrentCamera = 1u;
	mCamera2->rotateLeft(false);
	mCamera2->rotateRight(false);
	mCamera2->rotateUp(false);
	mCamera2->rotateDown(false);
	mCamera2->panLeft(false);
	mCamera2->panRight(false);
	mCamera2->panUp(false);
	mCamera2->panDown(false);
	mCamera2->panForward(false);
	mCamera2->panBackward(false);
}

void CameraManager::useCamera2()
{
	mCurrentCamera = 2u;
	mCamera1->rotateLeft(false);
	mCamera1->rotateRight(false);
	mCamera1->rotateUp(false);
	mCamera1->rotateDown(false);
	mCamera1->panLeft(false);
	mCamera1->panRight(false);
	mCamera1->panUp(false);
	mCamera1->panDown(false);
	mCamera1->panForward(false);
	mCamera1->panBackward(false);
}

void CameraManager::update() const
{
	CameraBuffer cb;
	DirectX::XMStoreFloat4x4(&cb.mPerspective, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&mPerspective)));

	if (mCurrentCamera == 1)
	{
		mCamera1->update(cb);
	}
	else
	{
		mCamera2->update(cb);
	}

	Dx11Render::instance()->useCameraBuffer(cb);
}
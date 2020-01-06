#pragma once

#include <DirectXMath.h>
#include "Camera.h"
#include <memory>

struct CameraBuffer
{
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mPerspective;
	DirectX::XMFLOAT4 mViewPosition;
};

class CameraManager
{
	static CameraManager * mInstance;
	CameraManager();

	std::unique_ptr<Camera> mCamera1;
	std::unique_ptr<Camera> mCamera2;
	DirectX::XMFLOAT4X4 mPerspective;
	unsigned int mCurrentCamera = 1u;

public:
	~CameraManager();

	static CameraManager * instance()
	{
		if (!mInstance)
		{
			mInstance = new CameraManager();
		}
		return mInstance;
	}

	void setCamera1(std::unique_ptr<Camera> & pCamera)
	{
		mCamera1 = std::move(pCamera);
	}

	void setCamera2(std::unique_ptr<Camera> & pCamera)
	{
		mCamera2 = std::move(pCamera);
	}

	void changeSize(const int pWidth, const int pHeight)
	{
		DirectX::XMStoreFloat4x4(&mPerspective, DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, static_cast<float>(pWidth) / static_cast<float>(pHeight), 0.01f, 20.0f));
	}

	void useCamera1();

	void useCamera2();

	void rotateLeft(const bool & pRotate)
	{
		if (mCurrentCamera == 1)
		{
			mCamera1->rotateLeft(pRotate);
		}
		else
		{
			mCamera2->rotateLeft(pRotate);
		}
	}

	void rotateRight(const bool & pRotate)
	{
		if (mCurrentCamera == 1)
		{
			mCamera1->rotateRight(pRotate);
		}
		else
		{
			mCamera2->rotateRight(pRotate);
		}
	}

	void rotateUp(const bool & pRotate)
	{
		if (mCurrentCamera == 1)
		{
			mCamera1->rotateUp(pRotate);
		}
		else
		{
			mCamera2->rotateUp(pRotate);
		}
	}

	void rotateDown(const bool & pRotate)
	{
		if (mCurrentCamera == 1)
		{
			mCamera1->rotateDown(pRotate);
		}
		else
		{
			mCamera2->rotateDown(pRotate);
		}
	}

	void panLeft(const bool & pPan)
	{
		if (mCurrentCamera == 1)
		{
			mCamera1->panLeft(pPan);
		}
		else
		{
			mCamera2->panLeft(pPan);
		}
	}

	void panRight(const bool & pPan)
	{
		if (mCurrentCamera == 1)
		{
			mCamera1->panRight(pPan);
		}
		else
		{
			mCamera2->panRight(pPan);
		}
	}

	void panUp(const bool & pPan)
	{
		if (mCurrentCamera == 1)
		{
			mCamera1->panUp(pPan);
		}
		else
		{
			mCamera2->panRight(pPan);
		}
	}

	void panDown(const bool & pPan)
	{
		if (mCurrentCamera == 1)
		{
			mCamera1->panDown(pPan);
		}
		else
		{
			mCamera2->panDown(pPan);
		}
	}

	void panForward(const bool & pPan)
	{
		if (mCurrentCamera == 1)
		{
			mCamera1->panForward(pPan);
		}
		else
		{
			mCamera2->panForward(pPan);
		}
	}

	void panBackward(const bool & pPan)
	{
		if (mCurrentCamera == 1)
		{
			mCamera1->panBackward(pPan);
		}
		else
		{
			mCamera2->panBackward(pPan);
		}
	}

	void update();

	void getViewPosition(DirectX::XMFLOAT3 & pViewPosition)
	{
		if (mCurrentCamera == 1)
		{
			mCamera1->getViewPosition(pViewPosition);
		}
		else
		{
			mCamera2->getViewPosition(pViewPosition);
		}
	}
};


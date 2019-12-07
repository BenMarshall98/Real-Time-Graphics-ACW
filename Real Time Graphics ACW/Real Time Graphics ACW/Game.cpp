#include "Game.h"
#include "Win32Window.h"
#include "DX11Render.h"

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <Windows.h>

#include "ModelLoader.h"

#include <string>
#include "IdentityNode.h"
#include <fstream>
#include "ObjectManager.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

double Game::mDt = 0.0f;
Camera * Game::mCamera = nullptr;

Game::Game()
{
	auto device = Dx11Render::instance()->getDevice();

	mNode = std::make_unique<IdentityNode>();

	std::ifstream in("Configuration.txt");

	if (in.good())
	{
		in >> *mNode;
	}

	in.close();

	// Initialize the world matrix
	mWorld = DirectX::XMMatrixIdentity();

	// Initialize the view matrix
	const auto eye = DirectX::XMFLOAT3(0.0f, 1.0f, -5.0f);
	const auto at = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	const auto up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

	mCamera = new Camera(eye, up, at);

	// Initialize the projection matrix
	mProjection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, Win32Window::instance()->getWidth() / (FLOAT)Win32Window::instance()->getHeight(), 0.01f, 100.0f);

	QueryPerformanceFrequency(&mTimer);
	mFreq = double(mTimer.QuadPart);

	QueryPerformanceCounter(&mTimer);
	mStart = mTimer.QuadPart;
}

void Game::run()
{
	while(Win32Window::instance()->windowEvents())
	{
		mCamera->update();
		Dx11Render::instance()->clearRenderTargetView(DirectX::Colors::MidnightBlue);

		const auto inst = Dx11Render::instance();
		auto deviceContext = Dx11Render::instance()->getDeviceContext();
		// Update our time

		QueryPerformanceCounter(&mTimer);
		mStop = mTimer.QuadPart;
		mDt = double(mStop - mStart) / mFreq;

		mDt2 += mDt;

		mStart = mStop;
		//
		// Animate the cube
		//

		//
		// Update variables
		//
		CameraBuffer cb;
		//cb.mWorld = XMMatrixTranspose(g_World);
		const auto viewMatrix = mCamera->getViewMatrix();
		const auto viewPosition = mCamera->getViewPosition();

		cb.mViewPosition = XMLoadFloat3(&viewPosition);
		cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&viewMatrix));
		cb.mProjection = XMMatrixTranspose(mProjection);

		inst->useCameraBuffer(cb);

		DirectionalLightBuffer db;

		db.mColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		db.mDirection = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
		db.mIsUsed = false;

		inst->useDirectionalLightBuffer(db);

		PointLightBuffer pb;

		pb.mColor = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
		pb.mPosition = DirectX::XMFLOAT4(1.0, 1.0f, 1.0f, 0.0f);
		pb.mAttenuationConstant = 1.0f;
		pb.mAttenuationLinear = 0.0f;
		pb.mAttenuationQuad = 0.1f;
		pb.mIsUsed = true;

		inst->usePointLightBuffer(pb);

		SpotLightBuffer sb;
		ZeroMemory(&sb, sizeof SpotLightBuffer);
		sb.mColor[0] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		sb.mPosition[0] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		sb.mDirection[0] = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
		sb.mInnerAngle[0] = DirectX::XMConvertToRadians(5.0f);
		sb.mInnerAngle[1] = DirectX::XMConvertToRadians(5.0f);
		sb.mInnerAngle[2] = DirectX::XMConvertToRadians(5.0f);
		sb.mInnerAngle[3] = DirectX::XMConvertToRadians(5.0f);
		sb.mOuterAngle[0] = DirectX::XMConvertToRadians(10.0f);
		sb.mAttenuationConstant[0] = 1.0f;
		sb.mAttenuationLinear[0] = 0.0f;
		sb.mAttenuationQuad[0] = 0.1f;
		sb.mIsUsed[0] = false;

		inst->useSpotLightBuffer(sb);
		
		MaterialBuffer mb;
		
		mb.mAmbient = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		mb.mDiffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		mb.mSpecular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		mb.mShininess = 32.0f;

		inst->useMaterialBuffer(mb);
		
		DirectX::XMFLOAT4X4 world;
		XMStoreFloat4x4(&world, DirectX::XMMatrixIdentity());
		
		mNode->update(world);

		ObjectManager::instance()->render();
		
		//
		// Present our back buffer to our front buffer
		//
		Dx11Render::instance()->present();
	}
}
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

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CameraVertexBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	auto hr = device->CreateBuffer(&bd, nullptr, &mCameraVertexBuffer);
	if (FAILED(hr))
	{

	}

	bd.ByteWidth = sizeof(CameraFragmentBuffer);

	hr = device->CreateBuffer(&bd, nullptr, &mCameraFragmentBuffer);

	bd.ByteWidth = sizeof(DirectionalLightBuffer);

	hr = device->CreateBuffer(&bd, nullptr, &mDirectionLightBuffer);

	bd.ByteWidth = sizeof(PointLightBuffer);

	hr = device->CreateBuffer(&bd, nullptr, &mPointLightBuffer);

	bd.ByteWidth = sizeof(SpotLightBuffer);

	hr = device->CreateBuffer(&bd, nullptr, &mSpotLightBuffer);

	bd.ByteWidth = sizeof(MaterialBuffer);

	hr = device->CreateBuffer(&bd, nullptr, &mMaterialBuffer);

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
		CameraVertexBuffer cb;
		//cb.mWorld = XMMatrixTranspose(g_World);
		const auto viewMatrix = mCamera->getViewMatrix();
		
		cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&viewMatrix));
		cb.mProjection = XMMatrixTranspose(mProjection);
		deviceContext->UpdateSubresource(mCameraVertexBuffer, 0, nullptr, &cb, 0, 0);

		deviceContext->VSSetConstantBuffers(0, 1, &mCameraVertexBuffer);

		CameraFragmentBuffer cfb;

		const auto viewPosition = mCamera->getViewPosition();

		cfb.mViewPosition = XMLoadFloat3(&viewPosition);

		deviceContext->UpdateSubresource(mCameraFragmentBuffer, 0, nullptr, &cfb, 0, 0);
		deviceContext->PSSetConstantBuffers(0, 1, &mCameraFragmentBuffer);

		DirectionalLightBuffer db;

		db.mColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		db.mDirection = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
		db.mIsUsed = false;

		deviceContext->UpdateSubresource(mDirectionLightBuffer, 0, nullptr, &db, 0, 0);
		deviceContext->PSSetConstantBuffers(2, 1, &mDirectionLightBuffer);

		PointLightBuffer pb;

		pb.mColor = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
		pb.mPosition = DirectX::XMFLOAT4(0.0, 0.0f, 0.0f, 0.0f);
		pb.mAttenuationConstant = 1.0f;
		pb.mAttenuationLinear = 0.0f;
		pb.mAttenuationQuad = 0.1f;
		pb.mIsUsed = true;

		deviceContext->UpdateSubresource(mPointLightBuffer, 0, nullptr, &pb, 0, 0);
		deviceContext->PSSetConstantBuffers(3, 1, &mPointLightBuffer);

		SpotLightBuffer sb;
		ZeroMemory(&sb, sizeof SpotLightBuffer);
		sb.mColor[0] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		sb.mPosition[0] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		sb.mDirection[0] = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
		sb.mInnerAngle[0] = DirectX::XMConvertToRadians(15.0f);
		sb.mInnerAngle[1] = DirectX::XMConvertToRadians(15.0f);
		sb.mInnerAngle[2] = DirectX::XMConvertToRadians(15.0f);
		sb.mInnerAngle[3] = DirectX::XMConvertToRadians(15.0f);
		sb.mOuterAngle[0] = DirectX::XMConvertToRadians(30.0f);
		sb.mAttenuationConstant[0] = 1.0f;
		sb.mAttenuationLinear[0] = 0.0f;
		sb.mAttenuationQuad[0] = 0.1f;
		sb.mIsUsed[0] = true;

		deviceContext->UpdateSubresource(mSpotLightBuffer, 0, nullptr, &sb, 0, 0);
		deviceContext->PSSetConstantBuffers(4, 1, &mSpotLightBuffer);
		
		MaterialBuffer mb;
		
		mb.mAmbient = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		mb.mDiffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		mb.mSpecular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		mb.mShininess = 32.0f;

		deviceContext->UpdateSubresource(mMaterialBuffer, 0, nullptr, &mb, 0, 0);
		deviceContext->PSSetConstantBuffers(1, 1, &mMaterialBuffer);
		
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
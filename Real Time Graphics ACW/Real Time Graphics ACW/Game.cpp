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
#include "LightingManager.h"
#include "ResourceManager.h"
#include "ConfigLoader.h"
#include "RenderManager.h"
#include "ParticleManager.h"
#include "CameraManager.h"

double Game::mDt = 0.0f;

Game::Game()
{
	auto device = Dx11Render::instance()->getDevice();

	mNode = std::make_unique<IdentityNode>();

	std::ifstream in("Configuration.txt");

	if (in.good())
	{
		ConfigLoader::readScene(in, mNode);
	}

	in.close();

	// Initialize the world matrix
	mWorld = DirectX::XMMatrixIdentity();

	// Initialize the view matrix
	{
		const auto eye = DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f);
		const auto at = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		const auto up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

		CameraManager::instance()->setCamera1(std::make_unique<Camera>(eye, up, at));
	}

	{
		const auto eye = DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f);
		const auto at = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		const auto up = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

		CameraManager::instance()->setCamera2(std::make_unique<Camera>(eye, up, at));
	}

	mBase = ResourceManager::instance()->loadTexture("tyre_base.dds");
	mSpec = ResourceManager::instance()->loadTexture("tyre_spec.dds");
	mDisp = ResourceManager::instance()->loadTexture("tyre_height.dds");
	mNorm = ResourceManager::instance()->loadTexture("tyre_normal.dds");
	
	QueryPerformanceFrequency(&mTimer);
	mFreq = double(mTimer.QuadPart);

	QueryPerformanceCounter(&mTimer);
	mStart = mTimer.QuadPart;
}

void Game::run()
{
	while(Win32Window::windowEvents())
	{
		CameraManager::instance()->update();
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
		//
		//ParticleManager::instance()->update(mDt);
		
		DirectX::XMFLOAT4X4 world;
		XMStoreFloat4x4(&world, DirectX::XMMatrixIdentity());
		
		mNode->update(world, world);

		LightingManager::instance()->update();

		mBase->useFragment(6);
		mSpec->useFragment(7);
		mNorm->useFragment(8);
		mDisp->useDomain(0);

		RenderManager::instance()->setup(mDt2);
		//ObjectManager::instance()->render();

		Dx11Render::instance()->defaultViewport();
		//Dx11Render::instance()->bindDefaultFramebuffer();

		mBase->useFragment(6);
		mSpec->useFragment(7);
		mNorm->useFragment(8);
		mDisp->useDomain(0);
		RenderManager::instance()->render();

		//Dx11Render::instance()->bindDefaultFramebuffer();

		//mInk->RenderInk();*/
		//
		//ParticleManager::instance()->render();
		//
		//
		// Present our back buffer to our front buffer
		//
		
		Dx11Render::instance()->present();
	}
}

Game::~Game()
{
	delete LightingManager::instance();
	delete RenderManager::instance();
	delete ObjectManager::instance();
	delete ResourceManager::instance();
	delete CameraManager::instance();
}

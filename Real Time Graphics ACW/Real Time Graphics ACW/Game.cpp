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

	mNode = std::make_unique<IdentityNode>();

	std::ifstream in("Configuration.txt");

	if (in.good())
	{
		ConfigLoader::readScene(in, mNode);
		ConfigLoader::readCameras(in);
	}

	in.close();

	// Initialize the world matrix
	mWorld = DirectX::XMMatrixIdentity();


	ResourceManager::instance()->loadTexture(mBase, "tyre_base.dds");
	ResourceManager::instance()->loadTexture(mSpec, "tyre_spec.dds");
	ResourceManager::instance()->loadTexture(mDisp, "tyre_height.dds");
	ResourceManager::instance()->loadTexture(mNorm, "tyre_normal.dds");
	
	QueryPerformanceFrequency(&mTimer);
	mFreq = static_cast<double>(mTimer.QuadPart);

	QueryPerformanceCounter(&mTimer);
	mStart = mTimer.QuadPart;
}

void Game::run()
{
	while(Win32Window::windowEvents())
	{
		CameraManager::instance()->update();
		Dx11Render::instance()->clearRenderTargetView(DirectX::Colors::MidnightBlue);

		// Update our time

		QueryPerformanceCounter(&mTimer);
		mStop = mTimer.QuadPart;
		mDt = static_cast<double>(mStop - mStart) / mFreq;

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

void Game::clear()
{
	delete LightingManager::instance();
	delete RenderManager::instance();
	delete ObjectManager::instance();
	delete ResourceManager::instance();
	delete CameraManager::instance();
}

Game::~Game()
{
}

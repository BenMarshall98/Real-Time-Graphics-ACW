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
DirectX::XMMATRIX Game::mWorld = DirectX::XMMatrixIdentity();
double Game::mFreq = 0.0f;
double Game::mDt2 = 0.0f;
__int64 Game::mStart = 0;
__int64 Game::mStop = 0;
std::unique_ptr<SceneGraphNode> Game::mNode = nullptr;
LARGE_INTEGER Game::mTimer = { 0 };
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

		RenderManager::instance()->setup(mDt2);
		//ObjectManager::instance()->render();

		Dx11Render::instance()->defaultViewport();
		//Dx11Render::instance()->bindDefaultFramebuffer();

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

void Game::reset()
{
	mNode.reset();
	
	delete LightingManager::instance();
	delete RenderManager::instance();
	delete ObjectManager::instance();
	delete CameraManager::instance();

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

	QueryPerformanceFrequency(&mTimer);
	mFreq = static_cast<double>(mTimer.QuadPart);

	QueryPerformanceCounter(&mTimer);
	mStart = mTimer.QuadPart;
}


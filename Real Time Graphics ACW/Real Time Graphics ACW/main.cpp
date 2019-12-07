#include "Win32Window.h"
#include "DX11Render.h"
#include "Game.h"
#include "IdentityNode.h"
#include <fstream>
#include "TranslationNode.h"
#include <memory>

int WINAPI wWinMain(HINSTANCE pHInstance, HINSTANCE pPrevInstance, LPWSTR pCmdLine, int pCmdShow)
{
	
	
	auto window = Win32Window::instance(pHInstance, pCmdShow);
	window->run();
	auto render = Dx11Render::instance();
	
	auto game = new Game();
	game->run();

	delete game;
	delete render;
	delete window;
}

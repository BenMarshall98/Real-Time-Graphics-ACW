#include "Win32Window.h"
#include "DX11Render.h"
#include "Game.h"
#include "IdentityNode.h"
#include <fstream>
#include "TranslationNode.h"
#include <memory>

int WINAPI wWinMain(HINSTANCE pHInstance, HINSTANCE pPrevInstance, LPWSTR pCmdLine, int pCmdShow)
{
	Win32Window::Instance(pHInstance, pCmdShow)->Run();
	DX11Render::Instance();

	Game * game = new Game();
	game->Run();

	delete game;
	delete Win32Window::Instance();
	delete DX11Render::Instance();
}

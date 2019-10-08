#include "Win32Window.h"
#include "DX11Render.h"
#include "Game.h"

int WINAPI wWinMain(HINSTANCE pHInstance, HINSTANCE pPrevInstance, LPWSTR pCmdLine, int pCmdShow)
{
	Win32Window::Instance(pHInstance, pCmdShow);
	DX11Render::Instance();

	Game * game = new Game();
	game->Run();

	delete Win32Window::Instance();
	delete DX11Render::Instance();
}
#include "Win32Window.h"
#include "DX11Render.h"
#include "Game.h"

// ReSharper disable CppParameterNamesMismatch
// ReSharper disable CppParameterMayBeConst
int WINAPI wWinMain(HINSTANCE pHInstance, HINSTANCE, LPWSTR, int pCmdShow)
// ReSharper restore CppParameterMayBeConst
// ReSharper restore CppParameterNamesMismatch
{
	
	
	const auto window = Win32Window::instance(pHInstance, pCmdShow);
	window->run();
	const auto render = Dx11Render::instance();
	
	auto game = new Game();
	game->run();

	delete game;
	delete render;
	delete window;

	return 1;
}

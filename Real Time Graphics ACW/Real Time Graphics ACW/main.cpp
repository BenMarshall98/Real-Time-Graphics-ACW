#include "Win32Window.h"
#include "DX11Render.h"
#include "Game.h"

// ReSharper disable CppParameterNamesMismatch
// ReSharper disable CppParameterMayBeConst
int WINAPI wWinMain(const HINSTANCE pHInstance, HINSTANCE, LPWSTR, const int pCmdShow)
// ReSharper restore CppParameterMayBeConst
// ReSharper restore CppParameterNamesMismatch
{
	const auto window = Win32Window::instance(pHInstance, pCmdShow);
	window->run();
	const auto render = Dx11Render::instance();
	
	const auto game = std::make_unique<Game>();
	game->run();

	game->clear();
	delete render;
	delete window;

	return 1;
}

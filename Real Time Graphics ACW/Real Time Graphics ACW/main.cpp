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

	std::ifstream in("configuration.txt");

	std::shared_ptr<SceneGraphNode> node = std::make_shared<IdentityNode>();

	in >> *node;
	
	auto game = new Game();
	game->run();

	delete game;
	delete render;
	delete window;
}

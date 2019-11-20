#include "Win32Window.h"
#include "DX11Render.h"
#include "Game.h"
#include "IdentityNode.h"
#include <fstream>

int WINAPI wWinMain(HINSTANCE pHInstance, HINSTANCE pPrevInstance, LPWSTR pCmdLine, int pCmdShow)
{
	std::ifstream fin("Configuration.txt");

	std::shared_ptr<SceneGraphNode> node = std::make_shared<IdentityNode>();

	fin >> *node;

	node.reset();
	
	Win32Window::Instance(pHInstance, pCmdShow)->Run();
	DX11Render::Instance();

	Game * game = new Game();
	game->Run();

	delete game;
	delete Win32Window::Instance();
	delete DX11Render::Instance();
}
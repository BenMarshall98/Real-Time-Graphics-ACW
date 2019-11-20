#include "Win32Window.h"
#include "DX11Render.h"
#include "Game.h"
#include "IdentityNode.h"
#include <fstream>
#include "TranslationNode.h"
#include <memory>

int WINAPI wWinMain(HINSTANCE pHInstance, HINSTANCE pPrevInstance, LPWSTR pCmdLine, int pCmdShow)
{
	std::ifstream fin("Configuration.txt");

	std::shared_ptr<IdentityNode> node(new IdentityNode());

	fin >> *node;
	fin.close();

	node.reset();
	
	
	Win32Window::Instance(pHInstance, pCmdShow)->Run();
	DX11Render::Instance();

	Game * game = new Game();
	game->Run();

	delete game;
	delete Win32Window::Instance();
	delete DX11Render::Instance();
}

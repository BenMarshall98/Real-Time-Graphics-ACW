#include "Win32Window.h"
#include "DX11Render.h"
#include "Game.h"
#include <string>

Win32Window * Win32Window::mInstance = nullptr;

Win32Window::Win32Window(HINSTANCE pHInstance, int pCmdShow) : mCmdShow(pCmdShow)
{
	WNDCLASSEX wndClassEx;
	wndClassEx.cbSize = sizeof(WNDCLASSEX);
	wndClassEx.style = CS_HREDRAW | CS_VREDRAW;
	wndClassEx.lpfnWndProc = windowProcedure;
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hInstance = pHInstance;
	wndClassEx.hIcon = LoadIcon(pHInstance, IDI_APPLICATION);
	wndClassEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClassEx.hbrBackground = NULL;
	wndClassEx.lpszMenuName = nullptr;
	wndClassEx.lpszClassName = mClassName;
	wndClassEx.hIconSm = LoadIcon(pHInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wndClassEx))
	{

	}

	RECT rect = { 0, 0, mWidth, mHeight };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	mHwnd = CreateWindow(mClassName, "Real Time Graphics ACW",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, pHInstance, nullptr);

	if (!mHwnd)
	{

	}
}

Win32Window * Win32Window::instance()
{
	return mInstance;
}

Win32Window * Win32Window::instance(HINSTANCE pHInstance, int pCmdShow)
{
	if (!mInstance)
	{
		mInstance = new Win32Window(pHInstance, pCmdShow);
	}

	return mInstance;
}

LRESULT CALLBACK Win32Window::windowProcedure(HWND pHwnd, UINT pMessage, WPARAM pWParam, LPARAM pLParam)
{
	PAINTSTRUCT paintStruct;
	HDC hdc;

	switch (pMessage)
	{
	case WM_PAINT:
		hdc = BeginPaint(pHwnd, &paintStruct);
		EndPaint(pHwnd, &paintStruct);
		break;
	case WM_CLOSE:
	{
		DestroyWindow(pHwnd);
	}
	break;
	case WM_KEYDOWN:
	{
		switch(pWParam)
		{
		case VK_LEFT:
		case 'A':
		case 'J':
			if (GetKeyState(VK_CONTROL) < 0)
			{
				Game::mCamera->panLeft(true);
				Game::mCamera->rotateLeft(false);
			}
			else
			{
				Game::mCamera->rotateLeft(true);
				Game::mCamera->panLeft(false);
			}
			break;
		case VK_RIGHT:
		case 'D':
		case 'L':
			if (GetKeyState(VK_CONTROL) < 0)
			{
				Game::mCamera->panRight(true);
				Game::mCamera->rotateRight(false);
			}
			else
			{
				Game::mCamera->rotateRight(true);
				Game::mCamera->panRight(false);
			}
			break;
		case VK_UP:
		case 'W':
		case 'I':
			if (GetKeyState(VK_CONTROL) < 0)
			{
				Game::mCamera->panForward(true);
				Game::mCamera->rotateUp(false);
			}
			else
			{
				Game::mCamera->rotateUp(true);
				Game::mCamera->panForward(false);
			}
			break;
		case VK_DOWN:
		case 'S':
		case 'K':
			if (GetKeyState(VK_CONTROL) < 0)
			{
				Game::mCamera->panBackward(true);
				Game::mCamera->rotateDown(false);
			}
			else
			{
				Game::mCamera->rotateDown(true);
				Game::mCamera->panBackward(false);
			}
			break;
		case VK_PRIOR:
			Game::mCamera->panUp(true);
			break;
		case VK_NEXT:
			Game::mCamera->panDown(true);
			break;
		}
	}
	break;
	case WM_KEYUP:
	{
		switch(pWParam)
		{
		case VK_LEFT:
		case 'A':
		case 'J':
			Game::mCamera->panLeft(false);
			Game::mCamera->rotateLeft(false);
			break;
		case VK_RIGHT:
		case 'D':
		case 'L':
			Game::mCamera->panRight(false);
			Game::mCamera->rotateRight(false);
			break;
		case VK_UP:
		case 'W':
		case 'I':
			Game::mCamera->panForward(false);
			Game::mCamera->rotateUp(false);
			break;
		case VK_DOWN:
		case 'S':
		case 'K':
			Game::mCamera->panBackward(false);
			Game::mCamera->rotateDown(false);
			break;
		case VK_PRIOR:
			Game::mCamera->panUp(false);
			break;
		case VK_NEXT:
			Game::mCamera->panDown(false);
			break;
		case VK_CONTROL:
			if (GetKeyState(VK_LEFT) < 0 ||
				GetKeyState('A') < 0 ||
				GetKeyState('J') < 0)
			{
				Game::mCamera->rotateLeft(true);
				Game::mCamera->panLeft(false);
			}

			if (GetKeyState(VK_RIGHT) < 0 ||
				GetKeyState('A') < 0 ||
				GetKeyState('J') < 0)
			{
				Game::mCamera->rotateRight(true);
				Game::mCamera->panRight(false);
			}

			if (GetKeyState(VK_UP) < 0 ||
				GetKeyState('W') < 0 ||
				GetKeyState('I') < 0)
			{
				Game::mCamera->rotateUp(true);
				Game::mCamera->panForward(false);
			}

			if (GetKeyState(VK_DOWN) < 0 ||
				GetKeyState('S') < 0 ||
				GetKeyState('K') < 0)
			{
				Game::mCamera->rotateDown(true);
				Game::mCamera->panBackward(false);
			}
			break;
		}
	}
	break;
	case WM_SIZE:
	{
		const int width = LOWORD(pLParam);
		const int height = HIWORD(pLParam);
		Dx11Render::instance()->resize(width, height);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(pHwnd, pMessage, pWParam, pLParam);
	}

	return 0;
}

bool Win32Window::windowEvents()
{
	auto result = true;
	
	MSG msg;
	
	while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
		{
			result = false;
		}
	}

	return result;
}
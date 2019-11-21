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
				Game::mCamera->panLeftRight(false);
			}
			else
			{
				Game::mCamera->rotateLeftRight();
			}
			break;
		case VK_RIGHT:
		case 'D':
		case 'L':
			if (GetKeyState(VK_CONTROL) < 0)
			{
				Game::mCamera->panLeftRight();
			}
			else
			{
				Game::mCamera->rotateLeftRight(false);
			}
			break;
		case VK_UP:
		case 'W':
		case 'I':
			if (GetKeyState(VK_CONTROL) < 0)
			{
				Game::mCamera->panForwardBackward();
			}
			else
			{
				Game::mCamera->rotateUpDown();
			}
			break;
		case VK_DOWN:
		case 'S':
		case 'K':
			if (GetKeyState(VK_CONTROL) < 0)
			{
				Game::mCamera->panForwardBackward(false);
			}
			else
			{
				Game::mCamera->rotateUpDown(false);
			}
			break;
		case VK_PRIOR:
			Game::mCamera->panUpDown();
			break;
		case VK_NEXT:
			Game::mCamera->panUpDown(false);
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
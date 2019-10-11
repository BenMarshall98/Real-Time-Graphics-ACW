#include "Win32Window.h"
#include "DX11Render.h"
#include "Game.h"
#include <string>

Win32Window * Win32Window::instance = nullptr;

Win32Window::Win32Window(HINSTANCE pHInstance, int pCmdShow) : mCmdShow(pCmdShow)
{
	WNDCLASSEX wndClassEx;
	wndClassEx.cbSize = sizeof(WNDCLASSEX);
	wndClassEx.style = CS_HREDRAW | CS_VREDRAW;
	wndClassEx.lpfnWndProc = WindowProcedure;
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hInstance = pHInstance;
	wndClassEx.hIcon = LoadIcon(pHInstance, IDI_APPLICATION);
	wndClassEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClassEx.hbrBackground = NULL;
	wndClassEx.lpszMenuName = nullptr;
	wndClassEx.lpszClassName = className;
	wndClassEx.hIconSm = LoadIcon(pHInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wndClassEx))
	{
		//TODO: Log Error
		int i = 0;
	}

	RECT rect = { 0, 0, width, height };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	mHWND = CreateWindow(className, "Real Time Graphics ACW",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, pHInstance, nullptr);

	if (!mHWND)
	{
		//TODO: Log Error
		int i = 0;
	}

	//ShowWindow(mHWND, pCmdShow);
}

Win32Window::~Win32Window()
{
}

Win32Window * Win32Window::Instance()
{
	return instance;
}

Win32Window * Win32Window::Instance(HINSTANCE pHInstance, int pCmdShow)
{
	if (!instance)
	{
		instance = new Win32Window(pHInstance, pCmdShow);
	}

	return instance;
}

LRESULT CALLBACK Win32Window::WindowProcedure(HWND pHWND, UINT pMessage, WPARAM pWParam, LPARAM pLParam)
{
	PAINTSTRUCT paintStruct;
	HDC hdc;

	switch (pMessage)
	{
	case WM_PAINT:
		hdc = BeginPaint(pHWND, &paintStruct);
		EndPaint(pHWND, &paintStruct);
		break;
	case WM_CLOSE:
	{
		DestroyWindow(pHWND);
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
				Game::camera->PanLeftRight(false);
			}
			else
			{
				Game::camera->RotateLeftRight();
			}
			break;
		case VK_RIGHT:
		case 'D':
		case 'L':
			if (GetKeyState(VK_CONTROL) < 0)
			{
				Game::camera->PanLeftRight();
			}
			else
			{
				Game::camera->RotateLeftRight(false);
			}
			break;
		case VK_UP:
		case 'W':
		case 'I':
			if (GetKeyState(VK_CONTROL) < 0)
			{
				Game::camera->PanForwardBackward();
			}
			else
			{
				Game::camera->RotateUpDown();
			}
			break;
		case VK_DOWN:
		case 'S':
		case 'K':
			if (GetKeyState(VK_CONTROL) < 0)
			{
				Game::camera->PanForwardBackward(false);
			}
			else
			{
				Game::camera->RotateUpDown(false);
			}
			break;
		case VK_PRIOR:
			Game::camera->PanUpDown();
			break;
		case VK_NEXT:
			Game::camera->PanUpDown(false);
			break;
		}
	}
	break;
	case WM_SIZE:
	{
		const int width = LOWORD(pLParam);
		const int height = HIWORD(pLParam);
		DX11Render::Instance()->Resize(width, height);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(pHWND, pMessage, pWParam, pLParam);
	}

	return 0;
}

bool Win32Window::WindowEvents()
{
	bool result = true;
	
	MSG msg = { 0 };
	
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
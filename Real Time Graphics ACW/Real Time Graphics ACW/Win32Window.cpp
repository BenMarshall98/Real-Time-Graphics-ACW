#include "Win32Window.h"
#include "DX11Render.h"
#include "CameraManager.h"
#include <string>
#include "ObjectManager.h"
#include "RenderManager.h"

Win32Window * Win32Window::mInstance = nullptr;

Win32Window::Win32Window(const HINSTANCE pHInstance, const int pCmdShow) : mCmdShow(pCmdShow)
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
	wndClassEx.hbrBackground = nullptr;
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

Win32Window * Win32Window::instance(const HINSTANCE pHInstance, const int pCmdShow)
{
	if (!mInstance)
	{
		mInstance = new Win32Window(pHInstance, pCmdShow);
	}

	return mInstance;
}

LRESULT CALLBACK Win32Window::windowProcedure(const HWND pHwnd, const UINT pMessage, const WPARAM pWParam, const LPARAM pLParam)
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
				CameraManager::instance()->panLeft(true);
				CameraManager::instance()->rotateLeft(false);
			}
			else
			{
				CameraManager::instance()->rotateLeft(true);
				CameraManager::instance()->panLeft(false);
			}
			break;
		case VK_RIGHT:
		case 'D':
		case 'L':
			if (GetKeyState(VK_CONTROL) < 0)
			{
				CameraManager::instance()->panRight(true);
				CameraManager::instance()->rotateRight(false);
			}
			else
			{
				CameraManager::instance()->rotateRight(true);
				CameraManager::instance()->panRight(false);
			}
			break;
		case VK_UP:
		case 'W':
		case 'I':
			if (GetKeyState(VK_CONTROL) < 0)
			{
				CameraManager::instance()->panForward(true);
				CameraManager::instance()->rotateUp(false);
			}
			else
			{
				CameraManager::instance()->rotateUp(true);
				CameraManager::instance()->panForward(false);
			}
			break;
		case VK_DOWN:
		case 'S':
		case 'K':
			if (GetKeyState(VK_CONTROL) < 0)
			{
				CameraManager::instance()->panBackward(true);
				CameraManager::instance()->rotateDown(false);
			}
			else
			{
				CameraManager::instance()->rotateDown(true);
				CameraManager::instance()->panBackward(false);
			}
			break;
		case VK_PRIOR:
			CameraManager::instance()->panUp(true);
			break;
		case VK_NEXT:
			CameraManager::instance()->panDown(true);
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			ObjectManager::instance()->explode(pWParam - '0');
			break;
		case VK_F8:
			RenderManager::instance()->changeMode();
			break;
		case 'F':
			if (GetKeyState(VK_SHIFT) < 0)
			{
				InkRender::instance()->IncreaseHeight(true);
				InkRender::instance()->DecreaseHeight(false);
			}
			else
			{
				InkRender::instance()->DecreaseHeight(true);
				InkRender::instance()->IncreaseHeight(false);
			}
			break;
		case VK_SHIFT:
			if (GetKeyState('F') < 0)
			{
				InkRender::instance()->IncreaseHeight(true);
				InkRender::instance()->DecreaseHeight(false);
			}
			break;
		case VK_F1:
			CameraManager::instance()->useCamera1();
			break;
		case VK_F2:
			CameraManager::instance()->useCamera2();
			break;
		default:
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
			CameraManager::instance()->panLeft(false);
			CameraManager::instance()->rotateLeft(false);
			break;
		case VK_RIGHT:
		case 'D':
		case 'L':
			CameraManager::instance()->panRight(false);
			CameraManager::instance()->rotateRight(false);
			break;
		case VK_UP:
		case 'W':
		case 'I':
			CameraManager::instance()->panForward(false);
			CameraManager::instance()->rotateUp(false);
			break;
		case VK_DOWN:
		case 'S':
		case 'K':
			CameraManager::instance()->panBackward(false);
			CameraManager::instance()->rotateDown(false);
			break;
		case VK_PRIOR:
			CameraManager::instance()->panUp(false);
			break;
		case VK_NEXT:
			CameraManager::instance()->panDown(false);
			break;
		case VK_CONTROL:
			if (GetKeyState(VK_LEFT) < 0 ||
				GetKeyState('A') < 0 ||
				GetKeyState('J') < 0)
			{
				CameraManager::instance()->rotateLeft(true);
				CameraManager::instance()->panLeft(false);
			}

			if (GetKeyState(VK_RIGHT) < 0 ||
				GetKeyState('D') < 0 ||
				GetKeyState('L') < 0)
			{
				CameraManager::instance()->rotateRight(true);
				CameraManager::instance()->panRight(false);
			}

			if (GetKeyState(VK_UP) < 0 ||
				GetKeyState('W') < 0 ||
				GetKeyState('I') < 0)
			{
				CameraManager::instance()->rotateUp(true);
				CameraManager::instance()->panForward(false);
			}

			if (GetKeyState(VK_DOWN) < 0 ||
				GetKeyState('S') < 0 ||
				GetKeyState('K') < 0)
			{
				CameraManager::instance()->rotateDown(true);
				CameraManager::instance()->panBackward(false);
			}
			break;
		case 'F':
			{
				InkRender::instance()->DecreaseHeight(false);
				InkRender::instance()->IncreaseHeight(false);
			}
			break;
		case VK_SHIFT:
			if (GetKeyState('F') < 0)
			{
				InkRender::instance()->DecreaseHeight(true);
				InkRender::instance()->IncreaseHeight(false);
			}
			else
			{
				InkRender::instance()->DecreaseHeight(false);
				InkRender::instance()->IncreaseHeight(false);
			}
			break;
		}
	}
	break;
	case WM_SIZE:
	{
		const int width = LOWORD(pLParam);
		const int height = HIWORD(pLParam);
		if(!Dx11Render::instance()->resize(width, height))
		{
			PostMessage(pHwnd, WM_CLOSE, 0, 0);
		}
		CameraManager::instance()->changeSize(width, height);
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
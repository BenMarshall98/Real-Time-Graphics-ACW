#pragma once

#define NOMINMAX
#include <Windows.h>

class Win32Window
{
	const char * mClassName = "RealTimeGraphics";
	int mWidth = 800;
	int mHeight = 600;
	HWND mHwnd;
	int mCmdShow;

	static Win32Window * mInstance;

	Win32Window(HINSTANCE pHInstance, int pCmdShow);

	static LRESULT CALLBACK windowProcedure(HWND pHwnd, UINT pMessage, WPARAM pWParam, LPARAM pLParam);
	
public:

	static Win32Window * instance();
	static Win32Window * instance(HINSTANCE pHInstance, int pCmdShow);

	static bool windowEvents();

	int getWidth() const
	{
		return mWidth;
	}

	int getHeight() const
	{
		return mHeight;
	}

	HWND getHwnd() const
	{
		return mHwnd;
	}

	void run() const
	{
		ShowWindow(mHwnd, mCmdShow);
	}
	
	~Win32Window() = default;

	Win32Window(const Win32Window&) = delete;
	Win32Window(Win32Window &&) = delete;
	Win32Window & operator= (const Win32Window &) = delete;
	Win32Window & operator= (Win32Window &&) = delete;
};


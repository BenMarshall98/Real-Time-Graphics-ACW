#pragma once

#include <Windows.h>

class Win32Window
{
private:
	const char * className = "RealTimeGraphics";
	int width = 800;
	int height = 600;
	HWND mHWND;
	int mCmdShow;

	static Win32Window * instance;

	Win32Window(HINSTANCE pHInstance, int pCmdShow);

	static LRESULT CALLBACK WindowProcedure(HWND pHWND, UINT pMessage, WPARAM pWParam, LPARAM pLParam);
	
public:

	static Win32Window * Instance();
	static Win32Window * Instance(HINSTANCE pHInstance, int pCmdShow);

	bool WindowEvents();

	inline int GetWidth()
	{
		return width;
	}

	inline int GetHeight()
	{
		return height;
	}

	inline HWND GetHWND()
	{
		return mHWND;
	}

	inline void Run()
	{
		ShowWindow(mHWND, mCmdShow);
	}
	
	~Win32Window();
};


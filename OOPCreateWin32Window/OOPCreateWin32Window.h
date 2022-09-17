#pragma once

#include "resource.h"
#define MAX_LOADSTRING 100
WCHAR szWindowClass[MAX_LOADSTRING] = TEXT("BasicWindowClass");
WCHAR szTitle[MAX_LOADSTRING] = TEXT("New Window");

HINSTANCE hInst;

template<class T> class BaseWindow {
protected:
	virtual PCWSTR ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND m_hwnd;

public:
	BaseWindow() : m_hwnd(NULL) {}
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		T* pThis = NULL;
		if (uMsg == WM_NCCREATE) {
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (T*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
			pThis->m_hwnd = hwnd;
		}
		else {
			pThis = (T*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}

		if (pThis) {
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else {
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	//
	//  FUNCTION: MyRegisterClass()
	//
	//  PURPOSE: Registers the window class.
	//
	ATOM RegisterBasicWindowClass(HINSTANCE hInstance)
	{
		WNDCLASSEXW wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = T::WindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OOPCREATEWIN32WINDOW));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_OOPCREATEWIN32WINDOW);
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		return RegisterClassExW(&wcex);
	}

	//
	//   FUNCTION: InitInstance(HINSTANCE, int)
	//
	//   PURPOSE: Saves instance handle and creates main window
	//
	//   COMMENTS:
	//
	//        In this function, we save the instance handle in a global variable and
	//        create and display the main program window.
	//
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
	{
		RegisterBasicWindowClass(hInstance);
		HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, this);

		if (!hWnd) { return FALSE; }
		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);
		return TRUE;
	}

	HWND Window() const { return m_hwnd; }
};
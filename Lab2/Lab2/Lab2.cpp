// Lab2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Lab2.h"

#define MAX_LOADSTRING 100
#define REC_WID 10   // square side


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
int coords[4] = { 0, 0, 0, 0 };                 // to save the position of a current square
int R, G, B;                                    // to save the color of a current square
RECT rect;                                      // to save the window size

// For window resizing
int resizeTime = 0;
bool isResized = false;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LAB2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB2));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB2));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB2);
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
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_CREATE:
		SetTimer(hWnd, 101, 2000, NULL);
		break;
	case WM_SIZE:
		GetClientRect(hWnd, &rect);
		if (resizeTime < 3)
			resizeTime++;
		isResized = true;
		break;
	case WM_PAINT:
	{
		HDC hDC;
		PAINTSTRUCT ps;
		HBRUSH hBrush;

		hDC = BeginPaint(hWnd, &ps);

		// if resizing is not at first time
		if (resizeTime > 1 && isResized) {
			hBrush = (HBRUSH)SelectObject(hDC, CreateSolidBrush(RGB(R, G, B)));
			Rectangle(hDC, coords[0], coords[1], coords[2], coords[3]);
			isResized = false;
		}
		else {
			int r = rand() % 255;
			int g = rand() % 255;
			int b = rand() % 255;
			
			hBrush = (HBRUSH)SelectObject(hDC, CreateSolidBrush(RGB(r, g, b)));

			// the remainder of dividing by the width of the window
			int x = rand() % (rect.right - rect.left - REC_WID);

			// the remainder of dividing by the length of the window
			int y = rand() % (rect.bottom - rect.top - REC_WID); 

			Rectangle(hDC, x, y, x + REC_WID, y + REC_WID);

			// saving the inf about current square
			coords[0] = x;
			coords[1] = y;
			coords[2] = x + REC_WID;
			coords[3] = y + REC_WID;
			R = r; G = g; B = b;

			SelectObject(hDC, hBrush);
			DeleteObject(hBrush);
		}

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_TIMER:
		UpdateWindow(hWnd);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:      // Left mouse button was clicked   	
	{
		int mousex = LOWORD(lParam);
		int mousey = HIWORD(lParam);

		// if mouse position is inside the square		
		if (mousex >= coords[0] && mousex <= coords[2]
			&& mousey >= coords[1] && mousey <= coords[3])
		{
			KillTimer(hWnd, 101);
			SetTimer(hWnd, 101, 2000, NULL);
			InvalidateRect(hWnd, NULL, TRUE);
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, 101);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



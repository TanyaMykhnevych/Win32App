// Lab2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Lab2.h"

#define MAX_LOADSTRING 100
#define REC_WID 50   //длина стороны квадрата


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
int coords[4] = { 0, 0, 0, 0 };                 //Для сохранение координат текущего квадратика

RECT rect;                                      // для размеров окна

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
	// координаты для сохранения квадратика в данный момент времени и проверки для клика мыши


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
		break;
	case WM_PAINT:
	{
		HDC hDC;
		PAINTSTRUCT ps;
		HBRUSH hOldBrush, hNewBrush;

		hDC = BeginPaint(hWnd, &ps);
		
		KillTimer(hWnd, 101);
		SetTimer(hWnd, 101, 2000, NULL);
		hNewBrush = CreateSolidBrush(RGB(0, 0, 0));
		hOldBrush = (HBRUSH)SelectObject(hDC, hNewBrush);

		int r = rand() % 255;
		int g = rand() % 255;
		int b = rand() % 255;

		hNewBrush = CreateSolidBrush(RGB(r, g, b));
		hOldBrush = (HBRUSH)SelectObject(hDC, hNewBrush);


		int x = rand() % (rect.right - rect.left - REC_WID);//остаток от деления на ширину окна
		int y = rand() % (rect.bottom - rect.top - REC_WID);//остаток от деления на длину окна


		Rectangle(hDC, x, y, x + REC_WID, y + REC_WID);

		coords[0] = x;
		coords[1] = y;
		coords[2] = x + REC_WID;
		coords[3] = y + REC_WID;

		SelectObject(hDC, hOldBrush);
		DeleteObject(hNewBrush);
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

		// если координаты мыши внутри квадрата		
		if (mousex >= coords[0] && mousex <= coords[2]
			&& mousey >= coords[1] && mousey <= coords[3])
		{
			InvalidateRect(hWnd, NULL, TRUE);   // то перерисовать окно
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



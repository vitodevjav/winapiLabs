// BmpEmbed.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "BmpEmbed.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_BMPEMBED, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BMPEMBED));

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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BMPEMBED));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BMPEMBED);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW|WS_HSCROLL| WS_VSCROLL,
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
	HDC hDC, hCompatibleDC;
	PAINTSTRUCT PaintStruct;
	static HANDLE hBitmap;
	HANDLE hOldBitmap;
	RECT Rect;
	BITMAP Bitmap;
	static int nHorizDifference = 0, nVertDifference = 0;
	static int nHorizPosition = 0, nVertPosition = 0;
	switch (message)
	{
	case WM_ERASEBKGND:
		return 0;
	case WM_CREATE:
		hBitmap = LoadImage(NULL, L"C:\\Myspace\\ecc3ecf42c75db1ffce5d06cbe95b1e6_644.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		return 0;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &PaintStruct);
		GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
		hCompatibleDC = CreateCompatibleDC(hDC);
		hOldBitmap = SelectObject(hCompatibleDC, hBitmap);
		GetClientRect(hWnd, &Rect);
		BitBlt(hDC, 0, 0, Rect.right, Rect.bottom, hCompatibleDC, nHorizPosition, nVertPosition,
			SRCCOPY);
		if ((nHorizDifference = (Bitmap.bmWidth - Rect.right))>0)
			SetScrollRange(hWnd, SB_HORZ, 0, nHorizDifference, TRUE);
		else
			SetScrollRange(hWnd, SB_HORZ, 0, 0, TRUE);
		if ((nVertDifference = (Bitmap.bmHeight - Rect.bottom))>0)
			SetScrollRange(hWnd, SB_VERT, 0, nVertDifference, TRUE);
		else
			SetScrollRange(hWnd, SB_VERT, 0, 0, TRUE);

		SelectObject(hCompatibleDC, hOldBitmap);
		DeleteDC(hCompatibleDC);
		EndPaint(hWnd, &PaintStruct);
		return 0;
	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEDOWN:
			if (nVertPosition<nVertDifference)
				nVertPosition++;
			break;
		case SB_LINEUP:
			if (nVertPosition>0)
				nVertPosition--;
			break;
		case SB_THUMBTRACK:
			nVertPosition = HIWORD(wParam);
			break;
		}
		SetScrollPos(hWnd, SB_VERT, nVertPosition, TRUE);
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_HSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEDOWN:
			if (nHorizPosition<nHorizDifference)
				nHorizPosition++;
			break;
		case SB_LINEUP:
			if (nHorizPosition>0)
				nHorizPosition--;
			break;
		case SB_THUMBTRACK:
			nHorizPosition = (HIWORD(wParam));
			break;
		}
		SetScrollPos(hWnd, SB_HORZ, nHorizPosition, TRUE);
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;


    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

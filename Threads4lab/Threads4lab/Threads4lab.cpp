// Threads4lab.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Threads4lab.h"
#include <windows.h>
#include <tchar.h>
#include <CommCtrl.h>
#include <conio.h>
#include <iostream>
using namespace std;
#define MAX_LOADSTRING 100

#define BUTTON1_ID 1001
#define BUTTON2_ID 1002
#define BUTTON3_ID 1003
#define IDM_COUNT1 1011
#define IDM_COUNT2 1012
#define IDM_COUNT3_1 1013
#define IDM_COUNT3_2 1014


HDC hdc;
PAINTSTRUCT ps;
TCHAR *pStr = (TCHAR*)"Thread app";
HWND hButton1, hButton2, hButton3, solutionLabel1, solutionLabel2, solutionLabel3_1, solutionLabel3_2;

const DWORD MaxLength = 104857600;
static HANDLE file, file3_2;
static TCHAR name[256] = _T("C:\\WorkSpace\\test.txt");
static HANDLE hFile, hFile3_1, hFile3_2, hCalc;
static char text[104857600], text2[104857600], text3_1[104857600], text3_2[104857600];
static DWORD nCharRead, nCharRead3_1, nCharRead3_2;

int count = 0, count2 = 0, count3_1 = 0, count3_2 = 0;
TCHAR result[1024], result3_1[1024], result3_2[1024];

RECT tarWnd;
int width = 0, height = 400, textXCoord = -110, textYCoord = 100;

static STARTUPINFO tin1, tin2;
static PROCESS_INFORMATION pInfo1, pInfo2;

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
    LoadStringW(hInstance, IDC_THREADS4LAB, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_THREADS4LAB));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_THREADS4LAB));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_THREADS4LAB);
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


DWORD WINAPI MyThread2(LPVOID hWnd) {

	hFile = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(hFile, text2, MaxLength, &nCharRead, NULL);
	count2 = 0;
	for (int i = 0; i < nCharRead; i++) {
		if (iswdigit(text2[i])) {
			count2++;
		}
	}
	CloseHandle(hFile);
	TCHAR buf[3];
	_stprintf(buf,_T("%d"),count2);

	SetWindowText(solutionLabel2,buf);
	InvalidateRect(HWND(hWnd), NULL, TRUE);
	return 0;
}

DWORD WINAPI MyThread3_1(LPVOID hWnd) {

	hFile3_1 = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(hFile3_1, text3_1, MaxLength, &nCharRead3_1, NULL);

	count3_1 = 0;
	for (int i = 0; i < nCharRead3_1; i++) {
		if (iswdigit(text3_1[i])) {
			count3_1++;
		}
	}
	CloseHandle(hFile3_1);
	TCHAR buf[3];
	_stprintf(buf, _T("%d"), count3_1);

	SetWindowText(solutionLabel3_1, buf);
	InvalidateRect(HWND(hWnd), NULL, TRUE);
	return 0;
}

DWORD WINAPI MyThread3_2(LPVOID hWnd) {

	hFile3_2 = CreateFile("C:\\WorkSpace\\test1.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(hFile3_2, text3_2, MaxLength, &nCharRead3_2, NULL);

	count3_2 = 0;
	for (int i = 0; i < nCharRead3_2; i++) {
		if (iswdigit(text3_2[i])) {
			count3_2++;
		}
	}
	CloseHandle(hFile3_2);
	TCHAR buf[3];
	_stprintf(buf, _T("%d"), count3_2);

	SetWindowText(solutionLabel3_2, buf);	InvalidateRect(HWND(hWnd), NULL, TRUE);
	return 0;
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
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			case BUTTON1_ID:
			{
				hFile = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				ReadFile(hFile, text, MaxLength, &nCharRead, NULL);
				int count = 0;
				for (int i = 0; i < nCharRead; i++) {
					if (iswdigit(text[i])) {
						count++;
					}
				}
				CloseHandle(hFile);
				wsprintf(result, TEXT("%d"), count);
				SendMessage(GetDlgItem(hWnd, IDM_COUNT1), WM_SETTEXT, 0, LPARAM(result));

				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}
			case BUTTON2_ID:
				CreateThread(NULL, 0, MyThread2, hWnd, 0, NULL);
				ExitThread;
				break;
			case BUTTON3_ID:
				HANDLE hThread3_1, hThread3_2;

				hThread3_1 = CreateThread(NULL, 0, MyThread3_1, hWnd, 0, NULL);
				hThread3_2 = CreateThread(NULL, 0, MyThread3_2, hWnd, 0, NULL);
				SetThreadPriority(hThread3_1, THREAD_PRIORITY_NORMAL);
				ResumeThread(hThread3_1);

				SetThreadPriority(hThread3_2, THREAD_PRIORITY_HIGHEST);
				ResumeThread(hThread3_2);

								break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_SIZE:
		width = LOWORD(lParam);
		break;
	case WM_TIMER:
		hdc = GetDC(hWnd);
		if (textXCoord < width) {
			textXCoord++;
		}
		else {
			textXCoord = -110;
		}
		if (textYCoord < 470) {
			textYCoord++;
		}
		else textYCoord = 100;
		TextOut(hdc, textXCoord, textYCoord, pStr, _tcslen(pStr));
		ReleaseDC(hWnd, hdc);
		break;
	case WM_CREATE:
		SetTimer(hWnd,NULL,1,NULL);

		hButton1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("BUTTON"), TEXT("Button 1"), WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 10, 100, 30, hWnd, (HMENU)BUTTON1_ID, NULL, NULL);
		hButton2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("BUTTON"), TEXT("Button 2"), WS_VISIBLE | WS_CHILD | SS_LEFT, 110, 10, 100, 30, hWnd, (HMENU)BUTTON2_ID, NULL, NULL);
		hButton3 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("BUTTON"), TEXT("Button 3"), WS_VISIBLE | WS_CHILD | SS_LEFT, 210, 10, 100, 30, hWnd, (HMENU)BUTTON3_ID, NULL, NULL);

		solutionLabel1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 40, 100, 30, hWnd, HMENU(IDM_COUNT1), NULL, NULL);
		solutionLabel2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD | SS_LEFT, 110, 40, 100, 30, hWnd, HMENU(IDM_COUNT2), NULL, NULL);
		solutionLabel3_1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD | SS_LEFT, 210, 40, 100, 30, hWnd, HMENU(IDM_COUNT3_1), NULL, NULL);
		solutionLabel3_2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD | SS_LEFT, 210, 70, 100, 30, hWnd, HMENU(IDM_COUNT3_2), NULL, NULL);
		tin1.cb = sizeof(STARTUPINFO);
		tin1.dwFlags = STARTF_USESHOWWINDOW;
		tin1.wShowWindow = SW_SHOWNORMAL;

		tin2.cb = sizeof(STARTUPINFO);
		tin2.dwFlags = STARTF_USESHOWWINDOW;
		tin2.wShowWindow = SW_SHOWNORMAL;
		CreateProcess("C:\\Windows\\system32\\mspaint.exe",NULL , NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &tin1, &pInfo1);
	    CreateProcess("C:\\Windows\\system32\\calc.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &tin2, &pInfo2);
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
		TerminateProcess(pInfo1.hProcess,WM_QUIT);
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

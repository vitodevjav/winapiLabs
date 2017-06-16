// Lab5.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Lab5.h"
#include <iostream>
#define MAX_LOADSTRING 100
#define MAX_SEM_COUNT 12
#define THREADCOUNT 4


using namespace std;
// Global Variables:
static HANDLE hThread_Mutex;
TCHAR buf[10];
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HANDLE ExecutedEvent = NULL, ghSemaphore,hMutex;

HWND *Status = new HWND[30], Start1, Start2, Start3,Start4;
HANDLE aThread[3];
CRITICAL_SECTION criticalSection;
DWORD id = 1;
DWORD id2 = 2, id3 = 3;
STARTUPINFO sInf;
int aInt[30];
PROCESS_INFORMATION pInf;
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void doSomethingVeryUsefull(){}

DWORD WINAPI MutexThread(LPVOID hWnd) {
	for (int i = 0; i < 30; i++) {
		aInt[i] = i;
		Sleep(500);
	}
	return 0;
}

DWORD WINAPI Worker(LPVOID hWnd) {
	for (int i = 0; i < 30; i++) {
		aInt[i] = i;
		Sleep(10);
	}
	SetEvent(ExecutedEvent);
	return 0;
}
DWORD WINAPI SemaphoreThread(LPVOID hWnd) {
	DWORD dwWaitResult;
	BOOL bContinue = TRUE;

	while (bContinue)
	{
		dwWaitResult = WaitForSingleObject(ghSemaphore, 0L);          

		switch (dwWaitResult)
		{
		case WAIT_OBJECT_0:
			bContinue = FALSE;
			for (int i = 0; i < 30; i++) {
				aInt[i]++;
			}
			ReleaseSemaphore(ghSemaphore, 1, NULL);  
			break;

		case WAIT_TIMEOUT:
			break;
		}
	}
	return 0;
}
DWORD WINAPI CriticalThread(LPVOID hWnd) {
	EnterCriticalSection(&criticalSection);
	for (int i = 0; i < 30; i++) {
		aInt[i]++;
		
	}
	for (int i = 0; i < 100000000; i++) { doSomethingVeryUsefull(); }
	LeaveCriticalSection(&criticalSection);
	return 1;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	// Initialize the critical section one time only.
	if (!InitializeCriticalSectionAndSpinCount(&criticalSection,
		0x00000400))
		return 0;
	

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB5, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB5));

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
	// Release resources used by the critical section object.
	DeleteCriticalSection(&criticalSection);
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB5));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB5);
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
			case 1111:
			{
				ExecutedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			
				WaitForSingleObject(hMutex, INFINITE);

				CreateThread(NULL, 0, &Worker, NULL, 0, &id);

				LPSTR text = NULL;
				
				DWORD dwWaitResult = WaitForSingleObject(ExecutedEvent, INFINITE);
				for (int i = 0; i < 30; i++) {
					_stprintf(buf, _T("%d"), aInt[i]);
					SetWindowText(Status[i],buf);
				}
				break;
			}	
			case 2222: {
				CreateThread(NULL, 0, &CriticalThread, NULL, 0, &id2);
				CreateThread(NULL, 0, &CriticalThread, NULL, 0, &id3);
				Sleep(3);
				EnterCriticalSection(&criticalSection);
				for (int i = 0; i < 30; i++) {
					_stprintf(buf, _T("%d"), aInt[i]);
					SetWindowText(Status[i], buf);
				}
				LeaveCriticalSection(&criticalSection);
				break;
			}
			case 3333:
			{
				
				HANDLE aThread[THREADCOUNT];
				DWORD ThreadID;
				int i;

				ghSemaphore = CreateSemaphore(NULL, MAX_SEM_COUNT, MAX_SEM_COUNT, NULL);      

				for (i = 0; i < THREADCOUNT; i++)
				{
					aThread[i] = CreateThread(NULL,	0, (LPTHREAD_START_ROUTINE)SemaphoreThread,	NULL, 0, &ThreadID);
				}

				WaitForMultipleObjects(THREADCOUNT, aThread, TRUE, INFINITE);

				for (int i = 0; i < 30; i++) {
					_stprintf(buf, _T("%d"), aInt[i]);
					SetWindowText(Status[i], buf);
				}

				for (i = 0; i < THREADCOUNT; i++)
					CloseHandle(aThread[i]);

				CloseHandle(ghSemaphore);

				break;
			}
			case 4444:
			{
				
				break; 
			}
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_CREATE:
		Start1 = CreateWindow("Button", "Start", WS_VISIBLE | WS_BORDER | WS_CHILD, 10, 110, 300, 50, hWnd, (HMENU)1111, NULL, NULL);
		Start2 = CreateWindow("Button", "Start", WS_VISIBLE | WS_BORDER | WS_CHILD, 10, 160, 300, 50, hWnd, (HMENU)2222, NULL, NULL);
		Start3 = CreateWindow("Button", "Start", WS_VISIBLE | WS_BORDER | WS_CHILD, 10, 210, 300, 50, hWnd, (HMENU)3333, NULL, NULL);
		Start4 = CreateWindow("Button", "Start", WS_VISIBLE | WS_BORDER | WS_CHILD, 10, 260, 300, 50, hWnd, (HMENU)4444, NULL, NULL);
		for (int i = 0; i < 30; i++) {
			Status[i] = CreateWindow("Edit", "", WS_VISIBLE | WS_BORDER | WS_CHILD, 110+i*20, 10, 20, 20, hWnd, (HMENU)333, NULL, NULL);
		}
		hMutex = CreateMutex(NULL, FALSE, "MyMutexLab5");
		hThread_Mutex = CreateThread(NULL, 0, &MutexThread, NULL, 0, &id2);
		WaitForSingleObject(hThread_Mutex, INFINITE);
		for (int i = 0; i < 30; i++) {
			_stprintf(buf, _T("%d"), aInt[i]);
			SetWindowText(Status[i], buf);
		}
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

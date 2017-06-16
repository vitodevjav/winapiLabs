// Pastemenu.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PasteMenu.h"

#define MAX_LOADSTRING 100
const int IDM_COLOR1 = 6, IDM_COLOR2 = 5, IDM_COLOR3 = 4, IDM_IMG1 = 3, IDM_IMG2 = 2, IDM_IMG3 = 1, IDM_EXT = 0;
int ID_SEND;

HMENU hMenu, hImage, hColor, hFile, hBckgr;
HDC hDC, hCompatibleDC;
PAINTSTRUCT PaintStruct;
static HANDLE hBitmap;
HANDLE hOldBitmap;
RECT Rect;
BITMAP Bitmap;
// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна
_In_ int nCmdShow;
// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PASTEMENU, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PASTEMENU));

	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

VOID Edit(HWND hWnd, int sender)
{
	if (sender <= 3) {
		if (sender == 3) hBitmap = LoadImage(NULL, L"C:\\Users\\Виталик\\Documents\\Visual Studio 2017\\Projects\\Pastemenu\\Tulips.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (sender == 2) hBitmap = LoadImage(NULL, L"C:\\Users\\Виталик\\Documents\\Visual Studio 2017\\Projects\\Pastemenu\\Chrysanthemum.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (sender == 1) hBitmap = LoadImage(NULL, L"C:\\Users\\Виталик\\Documents\\Visual Studio 2017\\Projects\\Pastemenu\\Hydrangeas.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hDC = GetDC(hWnd);
		GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
		hCompatibleDC = CreateCompatibleDC(hDC);
		hOldBitmap = SelectObject(hCompatibleDC, hBitmap);
		GetClientRect(hWnd, &Rect);
		StretchBlt(hDC, 0, 0, Rect.right, Rect.bottom, hCompatibleDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, SRCCOPY);
		SelectObject(hCompatibleDC, hOldBitmap);
		DeleteDC(hCompatibleDC);
		ReleaseDC(hWnd, hDC);
	}
	else {
		hDC = GetDC(hWnd);
		/*GetClientRect(hWnd, &Rect);
		InvalidateRect(hWnd,&Rect,0);
		HBRUSH hMyBrush=CreateSolidBrush(RGB(100,100,100));
		FillRect(hDC,&Rect,hMyBrush);*/
		ReleaseDC(hWnd, hDC);
	}
}


//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PASTEMENU));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_PASTEMENU);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_CREATE:
		AppendMenu(hFile = CreatePopupMenu(), MF_ENABLED | MF_STRING, IDM_EXT, L"&Exit");
		AppendMenu(hImage = CreatePopupMenu(), MF_ENABLED | MF_STRING, IDM_IMG1, L"Image1");
		AppendMenu(hImage, MF_ENABLED | MF_STRING, IDM_IMG2, L"Image2");
		AppendMenu(hImage, MF_ENABLED | MF_STRING, IDM_IMG3, L"Image3");
		AppendMenu(hColor = CreatePopupMenu(), MF_ENABLED | MF_STRING, IDM_COLOR1, L"Color1");
		AppendMenu(hColor, MF_ENABLED | MF_STRING, IDM_COLOR2, L"Color2");
		AppendMenu(hColor, MF_ENABLED | MF_STRING, IDM_COLOR3, L"Color3");
		AppendMenu(hBckgr = CreatePopupMenu(), MF_ENABLED | MF_POPUP, (UINT)hColor, L"Color");
		AppendMenu(hBckgr, MF_ENABLED | MF_POPUP, (UINT)hImage, L"Image");
		AppendMenu(hMenu = CreateMenu(), MF_ENABLED | MF_POPUP, (UINT)hFile, L"File");
		AppendMenu(hMenu, MF_ENABLED | MF_POPUP, (UINT)hBckgr, L"Background");

		SetMenu(hWnd, hMenu);
		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);
		DrawMenuBar(hWnd);
		Edit(hWnd, IDM_IMG1);
		CheckMenuItem(hImage, IDM_IMG1, MF_CHECKED);
		//AppendMenu(hBckgr=CreatePopupMenu(),MF_ENABLED|);
		ID_SEND = IDM_IMG1;
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_COLOR1:
			Edit(hWnd,IDM_COLOR1);
			ID_SEND = IDM_COLOR1;
			
			break;
		case IDM_IMG1:
			Edit(hWnd, IDM_IMG1);
			CheckMenuItem(hImage,IDM_IMG1,MF_CHECKED);	
			CheckMenuItem(hImage, IDM_IMG3, MF_UNCHECKED);
			CheckMenuItem(hImage, IDM_IMG2, MF_UNCHECKED);
			ID_SEND = IDM_IMG1;
			break;
		case IDM_IMG2:
			Edit(hWnd, IDM_IMG2);
			CheckMenuItem(hImage, IDM_IMG2, MF_CHECKED);
			CheckMenuItem(hImage, IDM_IMG3, MF_UNCHECKED);
			CheckMenuItem(hImage, IDM_IMG1, MF_UNCHECKED);
			ID_SEND = IDM_IMG2;
			break;
		case IDM_IMG3:
			Edit(hWnd, IDM_IMG3);
			CheckMenuItem(hImage, IDM_IMG3, MF_CHECKED);
			CheckMenuItem(hImage, IDM_IMG1, MF_UNCHECKED);
			CheckMenuItem(hImage, IDM_IMG2, MF_UNCHECKED);
			ID_SEND = IDM_IMG3;
			break;
		case IDM_EXT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		Edit(hWnd, ID_SEND);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
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

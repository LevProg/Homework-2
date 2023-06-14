#include "framework.h"
#include "homework6.h"
#include "Windows.h"
#include "stdio.h"
#include "commdlg.h"
#include "func.h"

#define MAX_LOADSTRING 100

#define SIZE 700

#define ID_NEW 0
#define ID_SAVE 1
#define ID_OPEN 2
#define ID_PEN 3
#define ID_ERASER 4
#define ID_COLOR 5
#define ID_SIZE_S 6
#define ID_SIZE_M 7
#define ID_SIZE_B 8
/* forward declarations */
PBITMAPINFO CreateBitmapInfoStruct(HBITMAP);
void CreateBMPFile(LPTSTR pszFile, HBITMAP hBMP);
int main(int argc, char** argv);

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
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

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_HOMEWORK6, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HOMEWORK6));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HOMEWORK6));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_HOMEWORK6);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       0, 0, 985, 750, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

HBITMAP hBitmap;
HDC oHDC;
HDC hMemDC;
HBITMAP hOldBitmap;
COLORREF rgbPen=RGB(0,0,0);
COLORREF rgbCurrent=RGB(0, 0, 0);
int size=4;
POINT pt;
HPEN bPen;
HBRUSH bBrush;
HBRUSH wBrush;
HBRUSH cBrush;
HPEN cPen;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    SelectObject(oHDC, bBrush);
    Ellipse(oHDC, 740, 590, 750, 600);
    Ellipse(oHDC, 825, 585, 845, 605);
    Ellipse(oHDC, 910, 580, 940, 610);
    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        cBrush = CreateSolidBrush(rgbCurrent);
        cPen = CreatePen(PS_SOLID, 5, rgbCurrent);
        SelectObject(hMemDC, cBrush);
        SelectObject(hMemDC, cPen);
        pt.x = LOWORD(lParam);
        pt.y = HIWORD(lParam);
        if (pt.x < 700 && pt.y < 700) {
            SetCapture(hWnd);
        }
    }
    break;
    case WM_LBUTTONUP:
        ReleaseCapture();
        break;
    case WM_MOUSEMOVE:
        pt.x = LOWORD(lParam);
        pt.y = HIWORD(lParam);
        if (pt.x < 700 && pt.y < 700 && (DWORD)wParam) {
            Ellipse(hMemDC, pt.x - size / 2, pt.y - size / 2, pt.x + size / 2, pt.y + size / 2);
            BitBlt(oHDC, 0, 0, SIZE, SIZE, hMemDC, 0, 0, SRCCOPY);
        }
        break;
    case WM_CREATE:
    {
        CreateWindowW(L"Button", L"New",
            WS_VISIBLE | WS_CHILD,
            700, 0, 120, 120, hWnd, (HMENU)ID_NEW, NULL, NULL);

        CreateWindowW(L"Button", L"Save",
            WS_VISIBLE | WS_CHILD,
            850, 0, 120, 120, hWnd, (HMENU)ID_SAVE, NULL, NULL);

        CreateWindowW(L"Button", L"Open",
            WS_VISIBLE | WS_CHILD,
            700, 150, 120, 120, hWnd, (HMENU)ID_OPEN, NULL, NULL);
        CreateWindowW(L"Button", L"Pen",
            WS_VISIBLE | WS_CHILD,
            850, 150, 120, 120, hWnd, (HMENU)ID_PEN, NULL, NULL);

        CreateWindowW(L"Button", L"Eraser",
            WS_VISIBLE | WS_CHILD,
            700, 300, 120, 120, hWnd, (HMENU)ID_ERASER, NULL, NULL);
        CreateWindowW(L"Button", L"Color",
            WS_VISIBLE | WS_CHILD,
            850, 300, 120, 120, hWnd, (HMENU)ID_COLOR, NULL, NULL);

        CreateWindowW(L"Static", L"Size",
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | SS_CENTER,
            700, 450, 270, 90, hWnd, NULL, NULL, NULL);
        CreateWindowW(L"Button", L"",
            WS_VISIBLE | WS_CHILD,
            700, 550, 90, 90, hWnd, (HMENU)ID_SIZE_S, NULL, NULL);
        CreateWindowW(L"Button", L"",
            WS_VISIBLE | WS_CHILD,
            790, 550, 90, 90, hWnd, (HMENU)ID_SIZE_M, NULL, NULL);
        CreateWindowW(L"Button", L"",
            WS_VISIBLE | WS_CHILD,
            880, 550, 90, 90, hWnd, (HMENU)ID_SIZE_B, NULL, NULL);
    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case ID_SAVE:
                SaveBitmapToBmp(hBitmap);
                break;
            case ID_OPEN:
                hMemDC=DrawBitmapFromBmpFile(hMemDC);
                BitBlt(oHDC, 0, 0, SIZE, SIZE, hMemDC, 0, 0, SRCCOPY);
                break;
            case ID_NEW:
                NewFile(hMemDC);
                BitBlt(oHDC, 0, 0, SIZE, SIZE, hMemDC, 0, 0, SRCCOPY);
                break;
            case ID_PEN:
                rgbCurrent = rgbPen;
                break;
            case ID_ERASER:
                rgbCurrent = RGB(255, 255, 255);
                break;
            case ID_COLOR:
                rgbPen = ChoseColor(hWnd);
                rgbCurrent = rgbPen;
                break;
            case ID_SIZE_S:
                size = 4;
                break;
            case ID_SIZE_M:
                size = 8;
                break;
            case ID_SIZE_B:
                size = 16;
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            wBrush = CreateSolidBrush(RGB(255, 255, 255));
            bPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
            SelectObject(hdc, wBrush);
            Rectangle(hdc, 0, 0, 700, 700);
            oHDC = hdc;
            hBitmap = CreateCompatibleBitmap(oHDC, SIZE, SIZE);
            hMemDC = CreateCompatibleDC(oHDC);
            hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
            NewFile(hMemDC);
            rgbCurrent = rgbPen;
            cBrush = CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(oHDC, cBrush);
            Ellipse(oHDC, 740, 590, 750, 600);
            BitBlt(oHDC, 0, 0, SIZE, SIZE, hMemDC, 0, 0, SRCCOPY);
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

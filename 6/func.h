#pragma once
#include "framework.h"
#include "commdlg.h"
#include "stdbool.h"

COLORREF ChoseColor(HWND hWnd);
void SaveBitmapToBmp(HBITMAP hBitmap);
HDC DrawBitmapFromBmpFile(HDC hDC);
void NewFile(HDC hDC);
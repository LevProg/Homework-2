#define _CRT_SECURE_NO_WARNINGS
#include "func.h"
#include "framework.h"
#include "commdlg.h"
#include "stdbool.h"
#define MAX_PATH 1000

COLORREF ChoseColor(HWND hWnd) {
    CHOOSECOLOR cc;                 // common dialog box structure 
    static COLORREF acrCustClr[16]; // array of custom colors 
    HWND hwnd = hWnd;                      // owner window
    HBRUSH hbrush;                  // brush handle
    static DWORD rgbCurrent;        // initial color selection
    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwnd;
    cc.lpCustColors = (LPDWORD)acrCustClr;
    cc.rgbResult = rgbCurrent;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc) == TRUE)
    {
        hbrush = CreateSolidBrush(cc.rgbResult);
        rgbCurrent = cc.rgbResult;
        return cc.rgbResult;
    }
}
bool OpenFileDlg(char* filePath, int bufferSize)
{
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = L"Bitmap Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = bufferSize;
    ofn.lpstrTitle = L"Select Bitmap File";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileName(&ofn))
        return true;
    else
        return false;
}
void NewFile(HDC hDC) {
    HBRUSH wBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN bPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
    SelectObject(hDC, wBrush);
    Rectangle(hDC, 0, 0, 700, 700);
}
void SaveBitmapToBmp(HBITMAP hBitmap) {
    BITMAP bmp;
    HDC hDC;
    FILE* file;
    BITMAPFILEHEADER bmpFileHeader;
    BITMAPINFOHEADER bmpInfoHeader;
    DWORD dwTotal;
    BYTE* pPixels;
    DWORD dwBytes;
    BYTE* pBuffer;
    DWORD dwWidth;
    DWORD dwHeight;
    WORD wBitsPerPixel;

    // Get information about the bitmap
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    // Set up bitmap info header
    dwWidth = bmp.bmWidth;
    dwHeight = bmp.bmHeight;
    wBitsPerPixel = bmp.bmBitsPixel;

    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfoHeader.biWidth = dwWidth;
    bmpInfoHeader.biHeight = dwHeight;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = wBitsPerPixel;
    bmpInfoHeader.biCompression = BI_RGB;
    bmpInfoHeader.biSizeImage = 0;
    bmpInfoHeader.biXPelsPerMeter = 0;
    bmpInfoHeader.biYPelsPerMeter = 0;
    bmpInfoHeader.biClrUsed = 0;
    bmpInfoHeader.biClrImportant = 0;

    // Calculate the size of the image buffer
    dwBytes = ((dwWidth * wBitsPerPixel + 31) & ~31) / 8;
    dwBytes *= dwHeight;
    // Allocate memory for the image buffer
    pBuffer = (BYTE*)malloc(dwBytes);
    if (!pBuffer)
        return false;

    // Create a device context and select the bitmap into it
    hDC = CreateCompatibleDC(NULL);
    SelectObject(hDC, hBitmap);

    // Retrieve the pixel data
    dwTotal = GetDIBits(hDC, hBitmap, 0, dwHeight, pBuffer, (LPBITMAPINFO)&bmpInfoHeader, DIB_RGB_COLORS);

    // Create the file
    char filePath[MAX_PATH] = "";
    
    OpenFileDlg(filePath, MAX_PATH);
    int i = 0;
    char str[100];
    while (filePath[i] != '.')
    {
        if (filePath[i] == '\\')
        {
            str[i/2] = '/';
        }
        else {
            str[i / 2] = filePath[i];
        }
        i+=2;
    }
    str[i/2] = '\0';
    char result[100];
    strcpy(result, str);
    strcat(result, ".");
    strcat(result, "bmp");

    file = fopen(result, "wb");
    // Set up the bitmap file header
    bmpFileHeader.bfType = 0x4D42;  // "BM"
    bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwBytes;
    bmpFileHeader.bfReserved1 = 0;
    bmpFileHeader.bfReserved2 = 0;
    bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // Write the bitmap file header
    fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file);

    // Write the bitmap info header
    fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);

    // Write the pixel data
    fwrite(pBuffer, dwBytes, 1, file);

    // Clean up
    fclose(file);
    free(pBuffer);
    DeleteDC(hDC);
   
}
HDC DrawBitmapFromBmpFile(HDC hDC)
{
    char filePath[MAX_PATH] = "";
    OpenFileDlg(filePath, MAX_PATH);
    HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, (LPSTR)filePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hBitmap)
    {
        HDC hMemDC = CreateCompatibleDC(hDC);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

        // Get the bitmap dimensions
        BITMAP bmp;
        GetObject(hBitmap, sizeof(BITMAP), &bmp);

        HBRUSH wBrush = CreateSolidBrush(RGB(255, 255, 255));
        HPEN bPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
        Rectangle(hDC, 0, 0, 700, 700);
        // Draw the bitmap on the device context
        BitBlt(hDC, 0, 0, 700, 700, hMemDC, 0, 0, SRCCOPY);
        
        return hMemDC;
    }
}
#pragma once
#include "framework.h"
#include "Windowsx.h"

int Score;
void ChangeDirection(int direction);
void Update(HDC hdc);
void Init(HWND hWnd);
void ChoseButton(int chose, HDC hdc);
void Start();
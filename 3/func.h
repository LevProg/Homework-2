#pragma once
#include "framework.h"

void SystemOpen(HDC hdc);
void SystemClose();
void SystemInitialise(char* fileName);
void SystemRelease();
void SystemUpdate(int direction);
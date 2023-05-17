#include "framework.h"
#include "Windowsx.h"
#include "func.h"
#include "time.h"
#include "stdlib.h"
#include "windows.h"
#define SIZE 30
#define SPAWNDELAY 500
#define HUNGRYDELAY 500
#define TRACKLEN 70

struct frogTag{
	POINT pos;
};
struct stoneTag{
	POINT pos;
};
struct pythonTag {
	int pythonLenght;
	POINT pos;
	struct pythonTag* next;
};
struct button {
	int pythonLenght;
	struct button* next;
	struct button* prev;
};
struct stoneTag* sHead = NULL;
struct pythonTag *pHead = NULL;
struct frogTag* fHead = NULL;
POINT track[TRACKLEN];
HWND pWnd = NULL;
HDC pHdc = NULL;
int isPause = 1;
int dir = 3;
int speed =0;
int spawnTimerFrog = SPAWNDELAY;
int spawnTimerStone = SPAWNDELAY;
int hungerTimer = 0;

void Lose() {
	PostQuitMessage(0);
}
void Start() {
	speed =3;
}
void DrawTextonButton() {
	TCHAR* str = TEXT("Start");
	HWND wnd = GetForegroundWindow();
	HDC dc = GetDC(wnd);
	RECT rc;
	HFONT font = CreateFont(-50, 0, 0, 0, 0, 0u, 0U, 0U, ANSI_CHARSET, 0U, 0U, 0U, 0U, TEXT("Arial"));
	HGDIOBJ old = SelectObject(dc, font);
	GetClientRect(wnd, &rc);
	rc.left = 700;
	rc.top = 40;
	SetBkMode(dc, TRANSPARENT);
	SetTextColor(dc, 0xFFFFFFFF);
	DrawText(dc, str, lstrlen(str), &rc, DT_CENTER);
	str = TEXT("Exit");
	rc.left = 700;
	rc.top = 190;
	DrawText(dc, str, lstrlen(str), &rc, DT_CENTER);
	SelectObject(dc, old);
	ReleaseDC(wnd, dc);
	DeleteObject(font);
	getchar();
}
void ChoseButton(int chose, HDC hdc) {
	HBRUSH hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH hGreyBrush = CreateSolidBrush(RGB(150, 150, 150));
	HPEN hBlackPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	SelectObject(hdc, hBlackPen);
	if (chose) {
		SelectObject(hdc, hGreyBrush);
		Rectangle(hdc, 700, 0, 1000, 130);
		SelectObject(hdc, hWhiteBrush);
		Rectangle(hdc, 700, 150, 1000, 280);
	}
	else {
		SelectObject(hdc, hWhiteBrush);
		Rectangle(hdc, 700, 0, 1000, 130);
		SelectObject(hdc, hGreyBrush);
		Rectangle(hdc, 700, 150, 1000, 280);
	}
	DrawTextonButton();
}
void ChangeDirection(int direction) {
	dir = direction;
}
void RandPos(int *x1, int *y1, int* x2, int* y2) {
	srand(time(0));
	int n1 = (rand()) % (700 - SIZE);
	*x1 = rand() % (700 - SIZE);
	*y1 = rand() % (700 - SIZE);
	*x2 = rand() % (700 - SIZE);
	*y2 = rand() % (700 - SIZE);
	while (n1 == *x1)
	{
		*x1 = rand() % (700 - SIZE);
	}
	while (*x1 == *y1)
	{
		*y1 = rand() % (700 - SIZE);
	};
	while (*y1 == *x2)
	{
		*x2 = rand() % (700 - SIZE);
	}
	while (*x2 == *y2)
	{
		*y2 = rand() % (700 - SIZE);
	};
}
void IncreasePython() {
	struct pythonTag* cur = pHead;
	while (cur->next != NULL) {
		cur = cur->next;
	}
	pHead->pythonLenght++;
	cur->next = (struct frogTag*)malloc(sizeof(struct frogTag));
	cur->next->next = NULL;
	cur->next->pos = (POINT){ cur->pos.x, cur->pos.y };
}
void DecreasePython() {
	struct pythonTag* cur = pHead;
	pHead->pythonLenght--;
	if (cur->next != NULL) {
		while (cur->next->next != NULL) {
			cur = cur->next;
		}
		cur->next = NULL;
	}
	else {
		Lose();
	}
}
void SpawnFrog() {
	int x, y, n,m;
	RandPos(&x, &y, &n, &m);
	if (fHead != NULL) {
		free(fHead);
	}
	fHead = (struct frogTag*)malloc(sizeof(struct frogTag));
	fHead->pos = (POINT){ x,y };
}
void SpawnStone() {
	int x, y, n, m;
	RandPos(&n, &m, &x, &y);
	if (sHead != NULL) {
		free(sHead);
	}
	sHead = (struct stoneTag*)malloc(sizeof(struct stoneTag));
	sHead->pos = (POINT){ x,y };
}
void CheckCollision() {
	if (pHead->next != NULL) {
		struct pythonTag* cur = pHead->next;
		while (cur->next != NULL) {
			if (abs(pHead->pos.x - cur->pos.x) + abs(pHead->pos.y - cur->pos.y) < SIZE) {
				Lose();
			}
			cur = cur->next;
		}
		if (abs(pHead->pos.x - cur->pos.x) + abs(pHead->pos.y - cur->pos.y) < SIZE) {
			Lose();
		}
	}
	if (fHead != NULL) {
		if (abs(fHead->pos.x - pHead->pos.x) < SIZE && abs(fHead->pos.y - pHead->pos.y) < SIZE) {
			SpawnFrog();
			hungerTimer = 0;
			spawnTimerFrog = 0;
			IncreasePython();
		}
	}
	if (sHead != NULL) {
		if (abs(sHead->pos.x - pHead->pos.x) < SIZE && abs(sHead->pos.y - pHead->pos.y) < SIZE) {
			SpawnStone();
			spawnTimerStone = 0;
			DecreasePython();
		}
	}
}
void Move() {
	if (pHead->pos.y > 0 && dir == 0) pHead->pos.y -= speed;
	else if (pHead->pos.y < (700 - SIZE) && dir == 1) pHead->pos.y += speed;
	else if (pHead->pos.x > 0 && dir == 2) pHead->pos.x -= speed;
	else if (pHead->pos.x < (700 - SIZE) && dir == 3) pHead->pos.x += speed;
	else Lose();
	CheckCollision();
}
void Draw(HDC hdc) {
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmMem = CreateCompatibleBitmap(hdc, 700, 700);
	HANDLE hOld = SelectObject(hdcMem, hbmMem);
	HBRUSH hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));
	HBRUSH hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
	HBRUSH hGreyBrush = CreateSolidBrush(RGB(100, 100, 100));
	HPEN hBluePen = CreatePen(PS_SOLID, 0, RGB(0, 0, 255));
	HPEN hBlackPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	Rectangle(hdcMem, 0, 0, 700, 700);
	SelectObject(hdcMem, hWhiteBrush);
	SelectObject(hdcMem, hBlackPen);
	Rectangle(hdcMem, 0, 0, 700, 700);
	struct pythonTag* curPHead = pHead;
	SelectObject(hdcMem, hBlueBrush);
	SelectObject(hdcMem, hBluePen);
	for (int i = 0; i <= (pHead->pythonLenght-1) * 10; i++) {
		Rectangle(hdcMem, track[i].x, track[i].y, track[i].x + SIZE, track[i].y + SIZE);
	}
	SelectObject(hdcMem, hGreenBrush);
	SelectObject(hdcMem, hBlackPen);
	if (fHead != NULL) {
		Rectangle(hdcMem, fHead->pos.x, fHead->pos.y, fHead->pos.x + SIZE, fHead->pos.y + SIZE);
	}
	SelectObject(hdcMem, hGreyBrush);
	if (sHead != NULL) {
		Rectangle(hdcMem, sHead->pos.x, sHead->pos.y, sHead->pos.x + SIZE, sHead->pos.y + SIZE);
	}
	BitBlt(hdc, 0, 0, 700, 700, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hOld);
	DeleteObject(hbmMem);
	DeleteDC(hdcMem);
	DeleteObject(hOld);
	DeleteObject(hGreenBrush);
	DeleteObject(hWhiteBrush);
	DeleteObject(hBlueBrush);
	DeleteObject(hBlackPen);
}
void Update(HDC hdc) {
	pHdc = hdc;
	Draw(pHdc);
	spawnTimerFrog++;
	spawnTimerStone++;
	hungerTimer++;
	Sleep(10);
	for (int i = TRACKLEN - 1; i > 0; i--) {
		track[i] = track[i - 1];
	}
	track[0] = pHead->pos;
	if (pHead->next != NULL) {
		struct pythonTag* cur = pHead->next;
		int i = 0;
		while (cur->next != NULL) {
			i += 10;
			cur->pos = track[i];
			cur = cur->next;
		}
		cur->pos = track[i + 10];
	}
	if (spawnTimerFrog > SPAWNDELAY) {
		spawnTimerFrog = 0;
		SpawnFrog();
	}
	if (spawnTimerStone > SPAWNDELAY) {
		spawnTimerStone = 0;
		SpawnStone();
	}
	if (hungerTimer > HUNGRYDELAY) {
		hungerTimer = 0;
		DecreasePython();
	}
	Move();
	InvalidateRect(pWnd, NULL, FALSE);
}
void Init(HWND hWnd) {
	pWnd = hWnd;
	pHead = (struct pythonTag*)malloc(sizeof(struct pythonTag));
	pHead->next = NULL;
	pHead->pos = (POINT){ 350, 350 };
	pHead->pythonLenght = 1;
}
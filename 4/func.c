#include "framework.h"
#include "Windowsx.h"
#include "func.h"
#include "time.h"
#include "stdlib.h"
#include "windows.h"
#define SIZE 30
#define SPAWNDELAY 50
#define HUNGRYDELAY 50
#define TRACKLEN 70
#define FIELDSIZE 690
#define position(tag) (int)(tag.x/SIZE)*SIZE, (int)(tag.y/SIZE)*SIZE, (int)(tag.x/SIZE+1)*SIZE, (int)(tag.y/SIZE+1)*SIZE
#define point(x) (int)(x/SIZE)*SIZE

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
HWND pWnd = NULL;
HDC pHdc = NULL;
int dir = 3;
int speed =0;
int spawnTimerFrog = SPAWNDELAY;
int spawnTimerStone = SPAWNDELAY;
int hungerTimer = 0;

void Lose() {
	PostQuitMessage(0);
}
void Start() {
	speed =30;
}
void ChangeDirection(int direction) {
	dir = direction;
}
void RandPos(int *x1, int *y1, int* x2, int* y2) {
	srand(time(0));
	int n1 = (rand()) % (FIELDSIZE - SIZE);
	*x1 = rand() % (FIELDSIZE - SIZE);
	*y1 = rand() % (FIELDSIZE - SIZE);
	*x2 = rand() % (FIELDSIZE - SIZE);
	*y2 = rand() % (FIELDSIZE - SIZE);
	while (n1 == *x1)
	{
		*x1 = rand() % 12;
	}
	while (*x1 == *y1)
	{
		*y1 = rand() % 12;
	};
	while (*y1 == *x2)
	{
		*x2 = rand() % 12;
	}
	while (*x2 == *y2)
	{
		*y2 = rand() % 12;
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
			cur = cur->next;
			if (point(cur->pos.x) == point(pHead->pos.x) && (point(cur->pos.y) == point(pHead->pos.y))) {
				Lose();
			}
		}
	}
	if (fHead != NULL) {
		if (point(fHead->pos.x)== point(pHead->pos.x) && (point(fHead->pos.y) == point(pHead->pos.y))) {
			SpawnFrog();
			hungerTimer = 0;
			spawnTimerFrog = 0;
			IncreasePython();
		}
	}
	if (sHead != NULL) {
		if (point(sHead->pos.x) == point(pHead->pos.x) && (point(sHead->pos.y) == point(pHead->pos.y))) {
			SpawnStone();
			spawnTimerStone = 0;
			DecreasePython();
		}
	}
}
void Move() {
	if (pHead->pos.y > 0 && dir == 0) pHead->pos.y -= speed;
	else if (pHead->pos.y < (FIELDSIZE - SIZE) && dir == 1) pHead->pos.y += speed;
	else if (pHead->pos.x > 0 && dir == 2) pHead->pos.x -= speed;
	else if (pHead->pos.x < (FIELDSIZE - SIZE) && dir == 3) pHead->pos.x += speed;
	else Lose();
	CheckCollision();
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
	rc.left = FIELDSIZE;
	rc.top = 190;
	DrawText(dc, str, lstrlen(str), &rc, DT_CENTER);
	SelectObject(dc, old);
	ReleaseDC(wnd, dc);
	DeleteObject(font);
	getchar();
}
void ChoseButton(int chose, HDC hdc) {
	HBRUSH hFieldBrush = CreateSolidBrush(RGB(177, 255, 163));
	HBRUSH hWhiteBrush = CreateSolidBrush(RGB(255, 255, 153));
	HBRUSH hGreyBrush = CreateSolidBrush(RGB(254, 254, 34));
	HPEN hBlackPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	SelectObject(hdc, hBlackPen);
	SelectObject(hdc, hFieldBrush);
	Rectangle(hdc, 0, 0, 1000, FIELDSIZE);
	if (chose) {
		SelectObject(hdc, hGreyBrush);
		Rectangle(hdc, FIELDSIZE, 0, 1000, 130);
		SelectObject(hdc, hWhiteBrush);
		Rectangle(hdc, FIELDSIZE, 150, 1000, 280);
	}
	else {
		SelectObject(hdc, hWhiteBrush);
		Rectangle(hdc, FIELDSIZE, 0, 1000, 130);
		SelectObject(hdc, hGreyBrush);
		Rectangle(hdc, FIELDSIZE, 150, 1000, 280);
	}
	DrawTextonButton();
}
void Draw(HDC hdc) {
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmMem = CreateCompatibleBitmap(hdc, FIELDSIZE, FIELDSIZE);
	HANDLE hOld = SelectObject(hdcMem, hbmMem);
	HBRUSH hFieldBrush = CreateSolidBrush(RGB(177, 255, 163));
	HBRUSH hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));
	HBRUSH hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
	HBRUSH hGreyBrush = CreateSolidBrush(RGB(100, 100, 100));
	HPEN hBluePen = CreatePen(PS_SOLID, 0, RGB(0, 0, 255));
	HPEN hBlackPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	HPEN hFieldPen = CreatePen(PS_SOLID, 0, RGB(202, 218, 186));
	Rectangle(hdcMem, 0, 0, FIELDSIZE, FIELDSIZE);
	SelectObject(hdcMem, hFieldBrush);
	SelectObject(hdcMem, hFieldPen);
	for (int y = 0; y < (FIELDSIZE / SIZE); y++) {
		for (int x = 0; x < (FIELDSIZE / SIZE); x++) {
			Rectangle(hdcMem, x*SIZE, y * SIZE, (x +1 ) * SIZE, (y + 1) * SIZE);
		}
	}
	struct pythonTag* curPHead = pHead;
	SelectObject(hdcMem, hBlueBrush);
	SelectObject(hdcMem, hBluePen);
	for (int i = 0; i < (pHead->pythonLenght); i++) {
		Rectangle(hdcMem, position(curPHead->pos));
		curPHead = curPHead->next;
	}
	SelectObject(hdcMem, hGreenBrush);
	SelectObject(hdcMem, hBlackPen);
	if (fHead != NULL) {
		Rectangle(hdcMem, position(fHead->pos));
	}
	SelectObject(hdcMem, hGreyBrush);
	if (sHead != NULL) {
		Rectangle(hdcMem, position(sHead->pos));
	}
	BitBlt(hdc, 0, 0, FIELDSIZE, FIELDSIZE, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hOld);
	DeleteObject(hbmMem);
	DeleteDC(hdcMem);
	DeleteObject(hOld);
	DeleteObject(hGreenBrush);
	DeleteObject(hFieldBrush);
	DeleteObject(hBlueBrush);
	DeleteObject(hBlackPen);
}
void Update(HDC hdc) {
	pHdc = hdc;
	spawnTimerFrog++;
	spawnTimerStone++;
	hungerTimer++;
	Sleep(150);
	Move();
	Draw(pHdc);
	if (pHead->next != NULL) {
		for (int i = 0; i < (pHead->pythonLenght); i++) {
			struct pythonTag* cur = pHead;
			POINT p= pHead->pos;
			for (int k = pHead->pythonLenght - 1 - i; k > 0; k--) {
				p = cur->pos;
				cur = cur->next;
			}
			cur->pos = p;
		}
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
	InvalidateRect(pWnd, NULL, FALSE);
}
void Init(HWND hWnd) {
	pWnd = hWnd;
	pHead = (struct pythonTag*)malloc(sizeof(struct pythonTag));
	pHead->next = NULL;
	pHead->pos = (POINT){ 350, 350 };
	pHead->pythonLenght = 1;
}
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
#define wWidht 600
#define wHeight 500
#define bWidht 200
#define bHeight 100

typedef struct {
    POINT pos;
    int color[3];
}Window;
struct Menu {
    Window* wind;
    int highlihtColor[3];
    char name[50];
    int isBut;
    struct Menu* next;
    struct Menu* prev;
};
HDC curHDC;
HBRUSH hbrush;
struct Menu* menuHead = NULL;
struct Menu* curB = NULL;

void SystemUpdate(int direction) {
    if (direction == 1) {
        if (curB->next->isBut) {
            hbrush = CreateSolidBrush(RGB(curB->wind->color[0], curB->wind->color[1], curB->wind->color[2]));
            SelectObject(curHDC, hbrush);
            Rectangle(curHDC, curB->wind->pos.x, curB->wind->pos.y, curB->wind->pos.x + bWidht, curB->wind->pos.y + bHeight);
            hbrush = CreateSolidBrush(RGB(curB->next->highlihtColor[0], curB->next->highlihtColor[1], curB->next->highlihtColor[2]));
            SelectObject(curHDC, hbrush);
            Rectangle(curHDC, curB->next->wind->pos.x, curB->next->wind->pos.y, curB->next->wind->pos.x + bWidht, curB->next->wind->pos.y + bHeight);
            curB = curB->next;
        }

    }
    else if (direction == -1) {
        if (curB->prev->isBut) {
            hbrush = CreateSolidBrush(RGB(curB->wind->color[0], curB->wind->color[1], curB->wind->color[2]));
            SelectObject(curHDC, hbrush);
            Rectangle(curHDC, curB->wind->pos.x, curB->wind->pos.y, curB->wind->pos.x + bWidht, curB->wind->pos.y + bHeight);
            hbrush = CreateSolidBrush(RGB(curB->prev->highlihtColor[0], curB->prev->highlihtColor[1], curB->prev->highlihtColor[2]));
            SelectObject(curHDC, hbrush);
            Rectangle(curHDC, curB->prev->wind->pos.x, curB->prev->wind->pos.y, curB->prev->wind->pos.x + bWidht, curB->prev->wind->pos.y + bHeight);
            curB = curB->prev;
        }
    }
    else if (direction == 0) {
        hbrush = CreateSolidBrush(RGB(curB->highlihtColor[0], curB->highlihtColor[1], curB->highlihtColor[2]));
        SelectObject(curHDC, hbrush);
        Rectangle(curHDC, curB->wind->pos.x, curB->wind->pos.y, curB->wind->pos.x + bWidht, curB->wind->pos.y + bHeight);
    }
    else {
        OutputDebugStringA(curB->name);
        OutputDebugStringA("\n");
    }
}
void DrawMenu() {
    struct Menu* cur = menuHead;
    int fl = 0;
    while (cur != NULL) {
        HBRUSH hbrush = CreateSolidBrush(RGB(cur->wind->color[0], cur->wind->color[1], cur->wind->color[2]));
        SelectObject(curHDC, hbrush);
        if (!cur->isBut && !fl) {
            Rectangle(curHDC, cur->wind->pos.x, cur->wind->pos.y, cur->wind->pos.x + wWidht, cur->wind->pos.y + wHeight);
        }
        else if (cur->isBut && fl) {
            curB = cur;
            Rectangle(curHDC, cur->wind->pos.x, cur->wind->pos.y, cur->wind->pos.x + bWidht, cur->wind->pos.y + bHeight);
        }
        if (cur->next == NULL && !cur->isBut && !fl) {
            cur = menuHead;
            fl++;
        }
        else {
            cur = cur->next;
        }
    }
}
void SystemOpen(HDC hdc) {
    curHDC = hdc;
}
void SystemClose(HDC hdc) {
    free(hbrush);
    curHDC = NULL;
}
void SystemInitialise(fileName){
    FILE* file;
    struct Menu* but = NULL;
    char str[100];
    file = fopen(fileName, "r");
    while (fgets(str, 50, file) != NULL) {
        if (strcmp(str, "MenuBegin\n") == 0 || strcmp(str, "MenuEnd") == 0) continue;
        if (strcmp(str, "\tWindowBegin\n") == 0 || strcmp(str, "\tButtonBegin\n") == 0) {
            but = (struct Menu*)malloc(sizeof(struct Menu));
            but->wind = (Window*)malloc(sizeof(Window));
            but->isBut = (strcmp(str, "\tButtonBegin\n") == 0) ? 1 : 0;
            but->next = NULL;
            if (menuHead == NULL) menuHead = but;
            else {
                if(menuHead->next!=NULL)
                    menuHead->next->prev = but;
                but->prev = menuHead;
                but->next = menuHead->next;
                menuHead->next = but;
            }
        }
        sscanf(str, "\t\tPosition=(%d,%d)\n", &but->wind->pos.x, &but->wind->pos.y);
        sscanf(str, "\t\tColor=(%d,%d,%d)\n", &but->wind->color[0], &but->wind->color[1], &but->wind->color[2]);
        sscanf(str, "\t\tHiglightColor=(%d,%d,%d)\n", &but->highlihtColor[0], &but->highlihtColor[1], &but->highlihtColor[2]);
        sscanf(str, "\t\tName=%50s\n", &but->name);
    }
    DrawMenu();
}
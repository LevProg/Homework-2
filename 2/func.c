#define _USE_MATH_DEFINES 
#include "math.h"
#include <windows.h>
#include "func.h"

// Структура точки
typedef struct {
    double x;
    double y;
} Point;

// Структура отрезка
typedef struct {
    Point start;
    Point end;
} Segment;

const Point st={ 200,500 };
const Point en = { 600,500 };

// Функция рисования отрезка на экране
void drawSegment(HDC hdc, Segment segment) {
    MoveToEx(hdc, (int)segment.start.x, (int)segment.start.y, NULL);
    LineTo(hdc, (int)segment.end.x, (int)segment.end.y);
}

// Функция построения N-й итерации кривой Леви
void levyCurve(HDC hdc, int N, Segment segment) {
    if (N == 0) {
        drawSegment(hdc,segment);
        return;
    }
    double len = sqrt(pow(segment.start.x - segment.end.x, 2) + pow(segment.end.y - segment.start.y, 2));
    double angle = atan2(segment.end.y - segment.start.y, segment.end.x - segment.start.x);
    Point mid = {
        (segment.start.x + segment.end.x) / 2,
        (segment.start.y + segment.end.y) / 2
    };
    Point p1 = { 
            mid.x + len / 2 * cos(angle - M_PI / 2),
            mid.y + len / 2 * sin(angle - M_PI / 2) 
    };

    Segment newSegment1 = { segment.start, p1 };
    Segment newSegment2 = { p1, segment.end };
    levyCurve(hdc, N - 1, newSegment1);
    levyCurve(hdc, N - 1, newSegment2);
}

void draw(HDC hdc, int N) {
    levyCurve(hdc, N, (Segment){ st, en });
}
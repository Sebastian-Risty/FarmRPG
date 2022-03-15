#include "basic.h"

POINT withinSquare(POINT pt1, POINT pt2) {
    srand(time(nullptr)); // generate seed
    return POINT{ rand() % LONG(pt2.x - pt1.x) + pt1.x , rand() % LONG(pt2.y - pt1.y) + pt1.y };
}

POINT withinCircle(POINT pt, LONG r) {
    srand(time(nullptr)); // generate seed
    LONG radius = rand() % r, theta = rand() % 360 + 1;
    return POINT{ radius * LONG(sin(theta)) + pt.x , radius * LONG(cos(theta)) + pt.y };
}

COLORREF getColor(POINT pt) {
    COLORREF color = 0x00FFFFFF;
    HDC hdc = GetDC(NULL);
    color = GetPixel(hdc, pt.x, pt.y);
    ReleaseDC(NULL, hdc);
    return color;
}
#include "mouse.h"

void click() {
    INPUT input{ 0 };
    input.type = INPUT_MOUSE;

    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(input)); //click

    ZeroMemory(&input, sizeof(input));

    input.mi.dwFlags = MOUSEEVENTF_LEFTUP; // unclick

    SendInput(1, &input, sizeof(input));
}

void scroll(LONG dir) {
    Sleep(500);
    INPUT input{ 0 };
    input.type = INPUT_MOUSE;

    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input.mi.mouseData = dir * 120;
    SendInput(1, &input, sizeof(input));
}

void move(POINT pt, bool checkForFish) {
    WINDOWPLACEMENT wp;
    GetWindowPlacement(globals::hwnd, &wp);

    // get cur point
    POINT curP{ 0, 0 };
    GetCursorPos(&curP);
    ScreenToClient(GetDesktopWindow(), &curP);

    double distance = sqrt(pow(pt.x - curP.x, 2) + pow(pt.y - curP.y, 2) * 1.0);

    std::vector<POINT> movePoints;

    // Bresenham’s Line Generation Algorithm
    LONG dx = abs(pt.x - curP.x);
    LONG sx = curP.x < pt.x ? 1 : -1;
    LONG dy = -abs(pt.y - curP.y);
    LONG sy = curP.y < pt.y ? 1 : -1;
    LONG error = dx + dy;

    while (true) {
        movePoints.push_back(POINT{ curP.x, curP.y });
        if (curP.x == pt.x && curP.y == pt.y) break;

        LONG e2 = 2 * error;

        if (e2 >= dy) {
            if (curP.x == pt.x) break;
            error += dy;
            curP.x += sx;
        }

        if (e2 <= dx) {
            if (curP.y == pt.y) break;
            error += dx;
            curP.y += sy;
        }
    }


    LONG count = 0;
    for (auto i = movePoints.begin(); i < movePoints.end(); ++i) { // could look into skipping points to go faster

        SetCursorPos(i->x, i->y);

        LONG gangStalk = ceil(distance / 100.0);

        if (gangStalk < 4) {
            gangStalk = 4;
        }

        if (count % (gangStalk * 2) - 3 == 0) {
            Sleep(1);
        }
        count++;


        // need a second thread so that we can have mouse move while still checking this shit
        /*
        if (count % 200 == 0 && checkForFish) { // stop moving to point if there is a fish to go for
            for (POINT pt : fishPoints) {
                COLORREF color = getColor(pt);
                if ((int(GetRValue(color)) + int(GetGValue(color)) + int(GetBValue(color))) < 380) { // make enums for these vals
                    return;
                }
            }
        }
        */
    }
}
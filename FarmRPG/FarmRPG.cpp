#include <iostream>
#include <Windows.h>
#include <curses.h>
#include <vector>
#include <random>

// TODO
// function to move, look into fancy randomized smoothing
// functions to perform repeated actions in game such as going to home, and other common areas
// make sure window detection is reliable

// making this shit global cuz ye
HWND cancer = FindWindow(L"Chrome_WidgetWin_1", NULL);
HWND hwnd = GetWindow(cancer, GW_HWNDNEXT);

void returnHome();
POINT withinSquare(double x1, double y1, double x2, double y2);
POINT withinCircle(double x, double y, double r);
void moveClick(POINT pt);
COLORREF getColor(POINT pt);
void fish();

int main()
{
    initscr(); // inti curses

    srand(time(nullptr)); // generate seed

    while (!hwnd) {
        Sleep(50);
        clear();
        printw("chromium not found");
        refresh();
    }

    SetForegroundWindow(hwnd);
    SetActiveWindow(hwnd);
    SetFocus(hwnd);

    while (true) {
        Sleep(50);

        // close
        //if (GetAsyncKeyState(VK_END) & 1) {
        //    return 0;
        //}

        // debug cursor pos
        if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
            POINT p {0, 0};
            GetCursorPos(&p);
            ScreenToClient(GetDesktopWindow(), &p);

            COLORREF color = 0x00FFFFFF;
            HDC hdc = GetDC(NULL);
            color = GetPixel(hdc, p.x, p.y);
            ReleaseDC(NULL, hdc);

            double dx = ::MulDiv(p.x, 65536, 1920);;
            double dy = ::MulDiv(p.y, 65536, 1080);;
            clear();
            printw("X: %d | Y: %d\n", p.x, p.y);
            printw("DX: %d | DY: %d\n", dx, dy);
            printw("R: %d | G: %d | B: %d", int(GetRValue(color)), int(GetGValue(color)), int(GetBValue(color)));
            refresh();
        }

        // debug mousePos
        if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
            INPUT input{ 0 };

            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            SendInput(1, &input, sizeof(input));

            ZeroMemory(&input, sizeof(input));

            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;

            Sleep(rand() % 70 + 45); // wait between 45ms - 115ms for humanized left click

            SendInput(1, &input, sizeof(input));
        }

        // debug keystroke
        if (GetAsyncKeyState(VK_NUMPAD3) & 1) { 
            INPUT input{ 0 };
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = VkKeyScanA('a');
            SendInput(1, &input, sizeof(input));
            ZeroMemory(&input, sizeof(input));
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &input, sizeof(input));
        }

        if (GetAsyncKeyState(VK_NUMPAD4) & 1) {
            fish();
        }

    }
}

void returnHome() {
    moveClick(withinSquare(10, 130, 240, 160));
}

POINT withinSquare(double x1, double y1, double x2, double y2) {
    POINT temp{0, 0};
    temp.x = rand() % int(x2 - x1) + x1;
    temp.y = rand() % int(y2 - y1) + y1;

    return temp;
}

POINT withinCircle(double x, double y, double r) {
    std::uniform_real_distribution<double> randRadius(0, r);
    std::uniform_real_distribution<double> randAngle(0, 360);
    std::default_random_engine re;

    double radius = randRadius(re);
    double theta = randAngle(re);

    POINT temp{0, 0};
    temp.x = radius * sin(theta) + x;
    temp.y = radius * cos(theta) + y;

    return temp;
}

void moveClick(POINT pt) {
    WINDOWPLACEMENT wp;
    GetWindowPlacement(hwnd, &wp);

    // get cur point
    POINT curP{ 0, 0 };
    GetCursorPos(&curP);
    ScreenToClient(GetDesktopWindow(), &curP);

    double distance = sqrt(pow(pt.x - curP.x, 2) + pow(pt.y - curP.y, 2) * 1.0);

    std::vector<POINT> movePoints;

    // Bresenham’s Line Generation Algorithm
    int dx = abs(pt.x - curP.x);
    int sx = curP.x < pt.x ? 1 : -1;
    int dy = -abs(pt.y - curP.y);
    int sy = curP.y < pt.y ? 1 : -1;
    int error = dx + dy;

    while (true) {
        movePoints.push_back(POINT{ curP.x, curP.y });
        if (curP.x == pt.x && curP.y == pt.y) break;

        int e2 = 2 * error;

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
    

    int count = 0;
    for (auto i = movePoints.begin(); i < movePoints.end(); ++i) { // could look into skipping points to go faster
        SetCursorPos(i->x, i->y);
        
        int gangStalk = ceil(distance / 100.0);

        if (gangStalk < 5) {
            gangStalk = 5;
        }

        if (count % gangStalk == 0 ) {
            Sleep(2);
        }
        count++;
    }

    Sleep(10);

    INPUT input{ 0 };
    input.type = INPUT_MOUSE;

    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(input)); //click

    ZeroMemory(&input, sizeof(input));

    input.mi.dwFlags = MOUSEEVENTF_LEFTUP; // unclick

    Sleep(rand() % 70 + 45); // wait between 45ms - 115ms for humanized left click

    SendInput(1, &input, sizeof(input));
}

COLORREF getColor(POINT pt) {
    COLORREF color = 0x00FFFFFF;
    HDC hdc = GetDC(NULL);
    color = GetPixel(hdc, pt.x, pt.y);
    ReleaseDC(NULL, hdc);
    return color;
}

void fish() {
    returnHome();

    Sleep(rand() % 300 + 1000);

    moveClick(withinSquare(331, 440, 592, 470));
    
    Sleep(rand() % 300 + 1000);

    // eventually add options to select area
    moveClick(withinSquare(333, 610, 574, 636)); // go to crystal for testin

    Sleep(rand() % 300 + 1000);

    std::vector<POINT> fishPoints;

    // add fishing points 
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            POINT temp { 746 + (100 * j), 295 + (75 * i) }; // can use ~15 for radius
            fishPoints.emplace_back(temp);
        }
    }

    while (true) {
        int fishCaught = 0;

        for (POINT pt : fishPoints) {
            COLORREF color = getColor(pt);
            if ((int(GetRValue(color)) + int(GetGValue(color)) + int(GetBValue(color))) < 380) { // may need to change limits depending on area or use diff method
                moveClick(withinCircle(pt.x, pt.y, 15));
            }
        }

        if (GetAsyncKeyState(VK_END) & 1) {
            break;
        }

        clear();
        printw("Fish caught: %d", fishCaught);

        printw("\n\nPress NUM0 to stop fishing");
        refresh();
    }
}
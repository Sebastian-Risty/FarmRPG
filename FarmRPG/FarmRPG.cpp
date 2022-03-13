#include <iostream>
#include <Windows.h>
#include <curses.h>
#include <vector>
#include <random>
#include <ctime>

// TODO
// function to move, look into fancy randomized smoothing
// make sure window detection is reliable
// organize into seperate files
// add multithreading

// look into parabolic / sinusoidal line algorithm to make it look more human

// making this shit global cuz fuck you
HWND cancer = FindWindow(L"Chrome_WidgetWin_1", NULL);
HWND hwnd = GetWindow(cancer, GW_HWNDNEXT);

std::vector<POINT> fishPoints;

void returnHome();
POINT withinSquare(double x1, double y1, double x2, double y2); // make these take in POINT for gods sake
POINT withinCircle(double x, double y, double r);
void move(POINT pt, bool checkForFish = false);
COLORREF getColor(POINT pt);
void fish();
void click();
void scrollUp(); // TODO

int main()
{
    initscr(); // inti curses

    while (!hwnd) {
        Sleep(50);
        clear();
        printw("chromium not found");
        refresh();
    }

    SetForegroundWindow(hwnd);
    SetActiveWindow(hwnd);
    SetFocus(hwnd);

    // add fishing points 
    for (int i = 0; i < 3; ++i) { // TODO: these should be init at program start tbh
        for (int j = 0; j < 4; ++j) {
            POINT temp{ 746 + (100 * j), 295 + (75 * i) }; // can use ~15 for radius
            fishPoints.emplace_back(temp);
        }
    }

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
    move(withinSquare(10, 130, 240, 160));
    click();
}

POINT withinSquare(double x1, double y1, double x2, double y2) {
    srand(time(nullptr)); // generate seed
    POINT temp{0, 0};
    temp.x = rand() % int(x2 - x1) + x1;
    temp.y = rand() % int(y2 - y1) + y1;

    return temp;
}

POINT withinCircle(double x, double y, int r) {
    srand(time(nullptr)); // generate seed
    int radius = rand() % r;
    int theta = rand() % 360 + 1;

    POINT temp{0, 0};
    temp.x = radius * sin(theta) + x;
    temp.y = radius * cos(theta) + y;

    return temp;
}

void move(POINT pt, bool checkForFish) {
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

        if (gangStalk < 4) {
            gangStalk = 4;
        }

        if (count % (gangStalk * 2) - 2 == 0 ) {
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

COLORREF getColor(POINT pt) {
    COLORREF color = 0x00FFFFFF;
    HDC hdc = GetDC(NULL);
    color = GetPixel(hdc, pt.x, pt.y);
    ReleaseDC(NULL, hdc);
    return color;
}

void fish() { // make it so you can stop bot nomatter where this is at, probably another thread looking for input??
    returnHome();
    Sleep(rand() % 200 + 400);

    move(withinSquare(331, 440, 592, 470));
    click();
    Sleep(rand() % 200 + 400);

    move(withinSquare(333, 610, 574, 636)); // go to crystal for testin, TODO: eventually add options to select area
    click();
    
    Sleep(400);

    // generate the colors at each point for current area
    std::vector<COLORREF> colorAtFishPoints;
    for (POINT pt : fishPoints) {
        colorAtFishPoints.emplace_back(getColor(pt));
    }

    //Sleep(rand() % 80 + 250);
    //move(withinSquare(745, 295, 1070, 430), true); // prepare to catch by moving to fishing area

    unsigned int fishCaught = 0;

    while (true) {
        for (auto i = 0; i < fishPoints.size(); ++i) {
            COLORREF curColor = getColor(fishPoints.at(i));
            int averageColor = int(GetRValue(colorAtFishPoints.at(i))) + int(GetGValue(colorAtFishPoints.at(i))) + int(GetBValue(colorAtFishPoints.at(i)));
            int curAverageColor = int(GetRValue(curColor)) + int(GetGValue(curColor)) + int(GetBValue(curColor));
            
            clear();
            printw("Looking for fish.\n");
            printw("Fish caught: %d", fishCaught);
            printw("\n\nPress NUM0 to stop fishing");
            refresh();

            if (curAverageColor < averageColor - 10) { 
                clear();
                printw("Fish found.\n");
                printw("Fish caught: %d", fishCaught);
                printw("\n\nPress NUM0 to stop fishing");
                refresh();
                move(withinCircle(fishPoints.at(i).x, fishPoints.at(i).y, 15)); // move to da fish
                click();
                break;
            }

            // reset loop
            if (i == fishPoints.size() - 1) {
                i = 0;
            }
        }

        clear();
        printw("Catching the fish.\n");
        printw("Fish caught: %d", fishCaught);
        printw("\n\nPress NUM0 to stop fishing");
        refresh();

        // maybe check if window appears before moving down just to make sure the catch wasnt fucked up
        // also add a check for when bait has run out, or just base it on catch count or smthn

        move(withinCircle(1090, 890, 10));
        
        Sleep(250);

        //COLORREF catchWindow = getColor(POINT{ 1400, 885 });
        COLORREF circleRight;
        while (true) { // (int(GetRValue(catchWindow) == 51 && int(GetGValue(catchWindow)) == 51 && int(GetBValue(catchWindow))) == 51)
            Sleep(10);
            circleRight = getColor(POINT{ 1090, 890 });
            if (int(GetBValue(circleRight)) == 255) {
                Sleep(20);
                click();
                break;
            }
            //catchWindow = getColor(POINT{ 1400, 885 });
        }
        fishCaught++;

        if (GetAsyncKeyState(VK_END) & 1) {
            break;
        }

        if (fishCaught > 0) {
            clear();
            printw("Caught it!\n");
            printw("Fish caught: %d", fishCaught);
            printw("\n\nPress NUM0 to stop fishing");
            refresh();
        }
        else {
            clear();
            printw("Fish caught: %d", fishCaught);

            printw("\n\nPress NUM0 to stop fishing");
            refresh();
        }
    }
}

void click() {
    srand(time(nullptr)); // generate seed
    INPUT input{ 0 };
    input.type = INPUT_MOUSE;

    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(input)); //click

    ZeroMemory(&input, sizeof(input));

    input.mi.dwFlags = MOUSEEVENTF_LEFTUP; // unclick

    //Sleep(rand() % 70 + 45); // wait between 45ms - 115ms for humanized left click

    SendInput(1, &input, sizeof(input));
    //Sleep(50);
}
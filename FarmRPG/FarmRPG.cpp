#include <iostream>
#include <Windows.h>
#include <curses.h>
#include <vector>
#include <random>
#include <ctime>

// TODO
// make sure window detection is reliable
// organize into seperate files
// add multithreading

// look into parabolic / sinusoidal line algorithm to make it look more human

// making this shit global cuz fuck you
HWND cancer = FindWindow(L"Chrome_WidgetWin_1", NULL);
HWND hwnd = GetWindow(cancer, GW_HWNDNEXT);

std::vector<POINT> fishPoints;

POINT withinSquare(POINT pt1, POINT pt2);
POINT withinCircle(POINT pt, int r);
void move(POINT pt, bool checkForFish = false);
COLORREF getColor(POINT pt);
void fish();
void click();
void scroll(int dir = 1);
void buyWorm();
void enterArea(int area);

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

        if (GetAsyncKeyState(VK_NUMPAD5) & 1) {
            scroll(-1);
        }

    }
}

POINT withinSquare(POINT pt1, POINT pt2) {
    srand(time(nullptr)); // generate seed
    POINT temp{0, 0};
    temp.x = rand() % int(pt2.x - pt1.x) + pt1.x;
    temp.y = rand() % int(pt2.y - pt1.y) + pt1.y;

    return temp;
}

POINT withinCircle(POINT pt, int r) {
    srand(time(nullptr)); // generate seed
    int radius = rand() % r;
    int theta = rand() % 360 + 1;

    POINT temp{0, 0};
    temp.x = radius * sin(theta) + pt.x;
    temp.y = radius * cos(theta) + pt.y;

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

void fish() { 
    buyWorm();

    enterArea(36); // head to emerald
    
    Sleep(400);

    // generate the colors at each point for current area
    std::vector<COLORREF> colorAtFishPoints;
    for (POINT pt : fishPoints) {
        colorAtFishPoints.emplace_back(getColor(pt));
    }

    //Sleep(rand() % 80 + 250);
    //move(withinSquare(745, 295, 1070, 430), true); // prepare to catch by moving to fishing area

    unsigned int fishCaught = 0;
    unsigned int maxFish = 25;
    //unsigned int buyBait = (rand() % 30 + 300);
    unsigned int buyBait = 10;

    while (fishCaught < maxFish) {
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
                move(withinCircle(fishPoints.at(i), 15)); // move to da fish
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

        move(withinCircle(POINT{ 1090, 890 }, 10));
        
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
        // buy more bait if needed
        if (fishCaught % buyBait == 0) {
            buyWorm();
            buyBait = (rand() % 30 + 300);
            Sleep(100);
            enterArea(36);
            Sleep(400);
        }

    }
}

void click() {
    INPUT input{ 0 };
    input.type = INPUT_MOUSE;

    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(input)); //click

    ZeroMemory(&input, sizeof(input));

    input.mi.dwFlags = MOUSEEVENTF_LEFTUP; // unclick

    SendInput(1, &input, sizeof(input));
}

void scroll(int dir) {
    Sleep(500);
    INPUT input{ 0 };
    input.type = INPUT_MOUSE;

    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input.mi.mouseData = dir * 120;
    SendInput(1, &input, sizeof(input));
}

void enterArea(int area) {
    srand(time(nullptr));

    switch (area) {
        // NAVBAR
    case 0:
        move(withinSquare(POINT{10, 130}, POINT{240, 160}));   // HOME 0
        click();
        break;
    case 3:
        move(withinSquare(POINT{10, 265}, POINT{240, 300}));   // WORKSHOP 3
        click();
        break;

        // HOME AREA, must go to HOME first
    case 10:
        enterArea(0);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{330, 170}, POINT{590, 220}));      // FARM 10
        click();
        break;
    case 11:
        enterArea(0);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{330, 370}, POINT{590, 420}));      // TOWN 11
        click();
        scroll(-1); // we need to scroll to have access to pet store
        break;
    case 12:
        enterArea(0);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{330, 440}, POINT{590, 470}));      // FISH 12
        click();
        break;
    case 13:
        enterArea(0);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{330, 500}, POINT{590, 550}));      // EXPLORE 13
        click();
        break;

    // TOWN AREAS, must go to TOWN first
    case 20:
        enterArea(11);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{260, 170}, POINT{560, 220})); // Country Store 20
        click();
        break;
    case 21:
        enterArea(11);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{260, 240}, POINT{560, 290})); // Farmers Markey 21
        click();
        break;
    case 25:
        enterArea(11);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{260, 550}, POINT{560, 600})); // Bank 25
        click();
        break;
    case 27:
        enterArea(11);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{260, 680}, POINT{560, 720})); // Steak Market 27
        click();
        break;
    case 28:
        enterArea(11);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{260, 920}, POINT{560, 970})); // Pet Store 28
        click();
        break;

    // FISHING AREAS must be in FISH area
    case 30:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{330, 270}, POINT{570, 320})); // Farm Pond 30
        click();
        break;
    case 31:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 340}, POINT{ 570, 390})); // Small Pond 31
        click();
        break;
    case 32:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 410}, POINT{ 570, 450})); // Forest Pond 32
        click();
        break;
    case 33:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 470}, POINT{ 570, 520})); // Lake Tempest 33
        click();
        break;
    case 34:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 530}, POINT{ 570, 580})); // Small Island 34
        click();
        break;
    case 35:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 600}, POINT{ 570, 650})); // Crystal River 35
        click();
        break;
    case 36:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 670}, POINT{ 570, 710})); // Emeral Beach 36
        click();
        break;
    case 37:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 720}, POINT{ 570, 780})); // XXX 37
        click();
        break;
    case 38:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 950}, POINT{ 570, 1020})); // get these last two values once unlocked bc i think tool tip may fuck the pos
        click();
        break;
    case 39:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 1040}, POINT{ 570, 1110})); // 
        click();
        break;


    }

    /*
    case 0:
        move(withinSquare(POINT{}, POINT{})); //
        Sleep(rand() % 200 + 400);
        click();
        break;
    */
}

void buyWorm() {
    enterArea(20); // go to country store

    // buy da worm
    scroll(-1000);
    Sleep(400);
    move(withinSquare(POINT{ 1455, 813 }, POINT{ 1513, 833 })); // buy
    click();
    Sleep(400);
    move(withinSquare(POINT{ 740, 950 }, POINT{ 1175, 980 })); // confirm
    click();
    Sleep(400);
    move(withinSquare(POINT{ 840, 617 }, POINT{ 1080, 626 })); // okay
    click();
    Sleep(400);
}

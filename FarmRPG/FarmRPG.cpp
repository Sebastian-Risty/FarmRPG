#include "globals.h"
#include "basic.h"
#include "mouse.h"
#include "fishing.h"

// TODO
// add interactivity to fishing
// settings file
// random 'breaks' during fishing session (2 levels, tiny breaks ~20 - 50 seconds, and longer breaks ~5 - 10 min, long only occurs after 500+
// make an enum for the area selection to make it easier to code & read

// make sure window detection is reliable
// organize into seperate files
// add multithreading

// look into parabolic / sinusoidal line algorithm to make it look more human
void enterArea(LONG area); // own function

int main()
{
    initscr(); // intit curseszz

    globals::cancer = FindWindow(L"Chrome_WidgetWin_1", NULL);
    globals::hwnd = GetWindow(globals::cancer, GW_HWNDNEXT);

    while (!globals::hwnd) {
        Sleep(50);
        clear();
        printw("chromium not found");
        refresh();
        globals::cancer = FindWindow(L"Chrome_WidgetWin_1", NULL);
        GetWindow(globals::cancer, GW_HWNDNEXT);
    }

    SetForegroundWindow(globals::hwnd);
    SetActiveWindow(globals::hwnd);
    SetFocus(globals::hwnd);

    // add fishing points 
    for (LONG i = 0; i < 3; ++i) { 
        for (LONG j = 0; j < 4; ++j) {
            globals::fishPoints.emplace_back(POINT{ 745 + (100 * j), 295 + (75 * i) } );
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
            printw("R: %d | G: %d | B: %d", LONG(GetRValue(color)), LONG(GetGValue(color)), LONG(GetBValue(color)));
            refresh();
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

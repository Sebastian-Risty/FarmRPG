#include "fishing.h"

void fish() {
    LONG fishCaught = 0;
    LONG maxFish = 136;
    LONG buyBait = (rand() % 30 + 300);
    LONG fishingArea = 37;


    buyWorm();

    enterArea(fishingArea); // head to emerald

    Sleep(500);

    // generate the colors at each point for current area
    std::vector<COLORREF> colorAtFishPoints;
    for (POINT pt : globals::fishPoints) {
        colorAtFishPoints.emplace_back(getColor(pt));
    }

    //Sleep(rand() % 80 + 250);
    //move(withinSquare(745, 295, 1070, 430), true); // prepare to catch by moving to fishing area

    while (fishCaught < maxFish) {
        for (LONG i = 0; i < globals::fishPoints.size(); ++i) {
            COLORREF curColor = getColor(globals::fishPoints.at(i));
            LONG averageColor = LONG(GetRValue(colorAtFishPoints.at(i))) + LONG(GetGValue(colorAtFishPoints.at(i))) + LONG(GetBValue(colorAtFishPoints.at(i)));
            LONG curAverageColor = LONG(GetRValue(curColor)) + LONG(GetGValue(curColor)) + LONG(GetBValue(curColor));

            clear();
            printw("Looking for fish.\n");
            printw("Fish caught: %d", fishCaught);
            printw("\n\nPress NUM0 to stop fishing");
            refresh();

            if (curAverageColor < averageColor - 5) {
                clear();
                printw("Fish found.\n");
                printw("Fish caught: %d", fishCaught);
                printw("\n\nPress NUM0 to stop fishing");
                refresh();
                move(withinCircle(globals::fishPoints.at(i), 13)); // move to da fish
                click();
                break;
            }

            // reset loop
            if (i == globals::fishPoints.size() - 1) {
                i = -1;
            }
        }

        clear();
        printw("Catching the fish.\n");
        printw("Fish caught: %d", fishCaught);
        printw("\n\nPress NUM0 to stop fishing");
        refresh();

        // maybe check if window appears before moving down just to make sure the catch wasnt fucked up
        // also add a check for when bait has run out, or just base it on catch count or smthn

        move(withinCircle(POINT{ 1078, 890 }, 8));

        Sleep(250);

        COLORREF circleRight;
        COLORREF catchWindow = getColor(POINT{ 1400, 885 });
        while (int(GetRValue(catchWindow) >= 48 && int(GetRValue(catchWindow) <= 54))) { // make sure fish was actually caught
            Sleep(12);
            circleRight = getColor(POINT{ 1090, 890 });
            catchWindow = getColor(POINT{ 1400, 885 });
            if (LONG(GetBValue(circleRight)) == 255) {
                Sleep(18);
                click();
                break;
            }
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
            buyBait += (rand() % 30 + 300);
            Sleep(100);
            enterArea(fishingArea);
            Sleep(500);
        }

    }
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
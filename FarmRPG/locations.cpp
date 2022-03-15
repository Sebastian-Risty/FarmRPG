#include "locations.h"

void enterArea(LONG area) {
    srand(time(nullptr));

    switch (area) {
        // NAVBAR
    case 0:
        move(withinSquare(POINT{ 10, 130 }, POINT{ 240, 160 }));   // HOME 0
        click();
        break;
    case 3:
        move(withinSquare(POINT{ 10, 265 }, POINT{ 240, 300 }));   // WORKSHOP 3
        click();
        break;

        // HOME AREA, must go to HOME first
    case 10:
        enterArea(0);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 170 }, POINT{ 590, 220 }));      // FARM 10
        click();
        break;
    case 11:
        enterArea(0);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 370 }, POINT{ 590, 420 }));      // TOWN 11
        click();
        scroll(-1); // we need to scroll to have access to pet store
        break;
    case 12:
        enterArea(0);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 440 }, POINT{ 590, 470 }));      // FISH 12
        click();
        break;
    case 13:
        enterArea(0);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 500 }, POINT{ 590, 550 }));      // EXPLORE 13
        click();
        break;

        // TOWN AREAS, must go to TOWN first
    case 20:
        enterArea(11);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 260, 170 }, POINT{ 560, 220 })); // Country Store 20
        click();
        break;
    case 21:
        enterArea(11);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 260, 240 }, POINT{ 560, 290 })); // Farmers Markey 21
        click();
        break;
    case 25:
        enterArea(11);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 260, 550 }, POINT{ 560, 600 })); // Bank 25
        click();
        break;
    case 27:
        enterArea(11);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 260, 680 }, POINT{ 560, 720 })); // Steak Market 27
        click();
        break;
    case 28:
        enterArea(11);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 260, 920 }, POINT{ 560, 970 })); // Pet Store 28
        click();
        break;

        // FISHING AREAS must be in FISH area
    case 30:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 270 }, POINT{ 570, 320 })); // Farm Pond 30
        click();
        break;
    case 31:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 340 }, POINT{ 570, 390 })); // Small Pond 31
        click();
        break;
    case 32:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 410 }, POINT{ 570, 450 })); // Forest Pond 32
        click();
        break;
    case 33:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 470 }, POINT{ 570, 520 })); // Lake Tempest 33
        click();
        break;
    case 34:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 530 }, POINT{ 570, 580 })); // Small Island 34
        click();
        break;
    case 35:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 600 }, POINT{ 570, 650 })); // Crystal River 35
        click();
        break;
    case 36:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 670 }, POINT{ 570, 710 })); // Emeral Beach 36
        click();
        break;
    case 37:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 720 }, POINT{ 570, 780 })); // Vast Ocean 37
        click();
        break;
    case 38:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 950 }, POINT{ 570, 1020 })); // get these last two values once unlocked bc i think tool tip may fuck the pos
        click();
        break;
    case 39:
        enterArea(12);
        Sleep(rand() % 200 + 400);
        move(withinSquare(POINT{ 330, 1040 }, POINT{ 570, 1110 })); // 
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
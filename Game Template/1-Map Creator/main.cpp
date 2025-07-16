#include <iostream>
#include <string>
#include "M_Engine.h"
#include "G_Engine.h"

using std::string;
M_Engine mEngine;
G_Engine gEngine;

string build = "Dev";

int main()
{
    int choice = -1;
    while (choice != 0 && choice != 1) {
        if (build == "Dev") {
            system("CLS");
            std::cout << "What do you want to use?\n"
                "0. Game       1.Map Creator\n";
            std::cin >> choice;
            if (std::cin.fail()) choice = -1;
        }
        else choice = 0;
    }
    

    InitWindow(590, 650, build.c_str());
    SetTargetFPS(60);

    InitAudioDevice();


    switch (choice) {
    case 0:
        gEngine.Start();
        mEngine.~M_Engine();
        break;
    case 1:
        mEngine.Start();
        gEngine.~G_Engine();
        break;
    }
	

	srand(time(NULL));
    while (!WindowShouldClose()){
        //UpdateMusicStream(AssetList::music);
        switch (choice) {
        case 0:
            gEngine.Update();
            gEngine.Draw();
            break;
        case 1:
            mEngine.Update();
            mEngine.Draw();
            break;
        }
	}

    //UnloadMusicStream(AssetList::music);
    CloseAudioDevice();
    CloseWindow();
}
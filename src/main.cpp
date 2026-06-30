#include "levels/firstlevel.hpp"

#include <raylib.h>

int main()
{
    InitWindow(800, 600, "Legend of Nora");
    SetTargetFPS(60);

    FirstLevel level;
    level.Init();

    while (!WindowShouldClose() && level.IsRunning())
    {
        level.Update();
        level.Draw();
    }

    level.Cleanup();
    CloseWindow();
}
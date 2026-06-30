#include <raylib.h>

int main()
{
    InitWindow(800, 600, "Hello Raylib");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello, world!", 190, 200, 30, BLACK);
        EndDrawing();
    }

    CloseWindow();
}

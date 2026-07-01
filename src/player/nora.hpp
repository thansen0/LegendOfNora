#pragma once

#include <raylib.h>

class Nora
{
public:
    Nora();

    static void LoadAssets();
    static void UnloadAssets();

    void Init(int startingBooks = 0);
    void Draw(int playerX, int playerY);
    void Cleanup();

    void CollectBook();
    [[nodiscard]] int GetBooksCollected() const { return booksCollected; }

private:
    static Texture2D texture;
    static bool assetsLoaded;

    int playerSize;
    int booksCollected{0};
};
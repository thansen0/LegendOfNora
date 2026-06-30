#pragma once

#include <raylib.h>

class Book
{
public:
    static void LoadAssets();
    static void UnloadAssets();

    void Spawn(float worldX, float y);
    void Despawn();

    [[nodiscard]] bool IsActive() const { return active; }
    [[nodiscard]] float GetScreenX(float scrollOffset) const { return worldX - scrollOffset; }
    void Draw(float scrollOffset) const;
    [[nodiscard]] bool CollidesWith(float playerX, float playerY, float playerSize, float scrollOffset) const;

private:
    static Texture2D texture;
    static bool assetsLoaded;

    float worldX = 0.0f;
    float y = 0.0f;
    bool active = false;
};
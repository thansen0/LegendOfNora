#pragma once

#include <raylib.h>

class Gorilla
{
public:
    static void LoadAssets();
    static void UnloadAssets();

    void Enter(float scrollOffset, int screenWidth, float floorY);
    void Cleanup();

    [[nodiscard]] bool IsActive() const { return active; }
    [[nodiscard]] float GetScreenX(float scrollOffset) const { return worldX - scrollOffset; }
    [[nodiscard]] float GetFadeAlpha(float scrollOffset, float playerX) const;

    void Draw(float scrollOffset) const;

private:
    static Texture2D texture;
    static bool assetsLoaded;

    static constexpr float FADE_START_DISTANCE = 420.0f;

    float worldX = 0.0f;
    float y = 0.0f;
    bool active = false;
};
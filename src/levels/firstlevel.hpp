#pragma once

#include <raylib.h>

class FirstLevel
{
public:
    void Init();
    void Update();
    void Draw();
    void Cleanup();

    [[nodiscard]] bool IsRunning() const { return running; }

private:
    bool running = true;

    float playerX = 120.0f;
    float playerY = 0.0f;
    float playerVelY = 0.0f;
    bool grounded = false;

    float scrollOffset = 0.0f;

    int screenWidth = 800;
    int screenHeight = 600;
    float floorY = 0.0f;

    Texture2D backgroundTexture{};

    void DrawBackground();

    static const char* ResolveAssetPath(const char* relativePath);
};
#include "firstlevel.hpp"

#include <cmath>
#include <string>

namespace
{
    constexpr float PLAYER_SIZE = 40.0f;
    constexpr float FORWARD_SPEED = 200.0f;
    constexpr float GRAVITY = 900.0f;
    constexpr float JUMP_VELOCITY = -380.0f;
    constexpr float FLOOR_HEIGHT = 80.0f;
    constexpr float TILE_WIDTH = 64.0f;
}

const char* FirstLevel::ResolveAssetPath(const char* relativePath)
{
    if (FileExists(relativePath))
    {
        return relativePath;
    }

    static std::string alternate;
    alternate = std::string("../") + relativePath;
    if (FileExists(alternate.c_str()))
    {
        return alternate.c_str();
    }

    return relativePath;
}

void FirstLevel::DrawBackground()
{
    if (backgroundTexture.id != 0)
    {
        DrawTexturePro(
            backgroundTexture,
            {0, 0, static_cast<float>(backgroundTexture.width), static_cast<float>(backgroundTexture.height)},
            {0, 0, static_cast<float>(screenWidth), static_cast<float>(screenHeight)},
            {0, 0},
            0.0f,
            WHITE);
    }
    else
    {
        DrawRectangle(0, 0, screenWidth, screenHeight, {30, 30, 50, 255});
    }
}

void FirstLevel::Init()
{
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    floorY = static_cast<float>(screenHeight) - FLOOR_HEIGHT;
    playerY = floorY - PLAYER_SIZE;
    grounded = true;

    const char* backgroundPath = ResolveAssetPath("assets/background/congobackground.jpg");
    Image backgroundImage = LoadImage(backgroundPath);
    if (IsImageValid(backgroundImage))
    {
        backgroundTexture = LoadTextureFromImage(backgroundImage);
        UnloadImage(backgroundImage);
    }
    else
    {
        TraceLog(LOG_WARNING, "FIRSTLEVEL: Failed to load background image: %s", backgroundPath);
    }
}

void FirstLevel::Update()
{
    const float dt = GetFrameTime();

    scrollOffset += FORWARD_SPEED * dt;

    playerVelY += GRAVITY * dt;
    playerY += playerVelY * dt;

    const float groundLevel = floorY - PLAYER_SIZE;
    if (playerY >= groundLevel)
    {
        playerY = groundLevel;
        playerVelY = 0.0f;
        grounded = true;
    }
    else
    {
        grounded = false;
    }

    if (IsKeyPressed(KEY_SPACE) && grounded)
    {
        playerVelY = JUMP_VELOCITY;
        grounded = false;
    }
}

void FirstLevel::Draw()
{
    BeginDrawing();

    DrawBackground();

    const float tileOffset = fmodf(scrollOffset, TILE_WIDTH);
    for (float x = -tileOffset; x < static_cast<float>(screenWidth); x += TILE_WIDTH)
    {
        const Color tileColor = (static_cast<int>(x + tileOffset) / static_cast<int>(TILE_WIDTH)) % 2 == 0
                                    ? Color{80, 70, 60, 255}
                                    : Color{65, 55, 45, 255};
        DrawRectangle(static_cast<int>(x), static_cast<int>(floorY), static_cast<int>(TILE_WIDTH),
                      static_cast<int>(FLOOR_HEIGHT), tileColor);
    }

    DrawRectangle(static_cast<int>(playerX), static_cast<int>(playerY), static_cast<int>(PLAYER_SIZE),
                  static_cast<int>(PLAYER_SIZE), GRAY);

    EndDrawing();
}

void FirstLevel::Cleanup()
{
    if (backgroundTexture.id != 0)
    {
        UnloadTexture(backgroundTexture);
        backgroundTexture = {};
    }
}
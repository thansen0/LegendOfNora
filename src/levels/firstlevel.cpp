#include "firstlevel.hpp"

#include "assets/asset_paths.hpp"

namespace
{
    constexpr float PLAYER_SIZE = 40.0f;
    constexpr float FORWARD_SPEED = 200.0f;
    constexpr float GRAVITY = 900.0f;
    constexpr float JUMP_VELOCITY = -380.0f;
}

const char* FirstLevel::ResolveAssetPath(const char* relativePath)
{
    return AssetPaths::Resolve(relativePath);
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

    ground.Init(GroundStyle::Main);
    playerY = ground.GetFloorY() - PLAYER_SIZE;
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

    const float groundLevel = ground.GetFloorY() - PLAYER_SIZE;
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
    ground.Draw(scrollOffset, screenWidth);

    DrawRectangle(static_cast<int>(playerX), static_cast<int>(playerY), static_cast<int>(PLAYER_SIZE),
                  static_cast<int>(PLAYER_SIZE), GRAY);

    EndDrawing();
}

void FirstLevel::Cleanup()
{
    ground.Cleanup();

    if (backgroundTexture.id != 0)
    {
        UnloadTexture(backgroundTexture);
        backgroundTexture = {};
    }
}
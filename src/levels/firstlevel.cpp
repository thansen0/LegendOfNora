#include "firstlevel.hpp"

#include "assets/asset_paths.hpp"

namespace
{
    constexpr float FORWARD_SPEED = 200.0f;
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

void FirstLevel::DrawScreenFade()
{
    if (screenFade <= 0.0f)
    {
        return;
    }

    const unsigned char alpha = static_cast<unsigned char>(screenFade * 255.0f);
    DrawRectangle(0, 0, screenWidth, screenHeight, {0, 0, 0, alpha});
}

void FirstLevel::BeginGorillaEncounter()
{
    if (gorillaEncounter)
    {
        return;
    }

    gorillaEncounter = true;
    books.Deactivate();
    chimps.Deactivate();
    gorilla.Enter(scrollOffset, screenWidth, ground.GetFloorY());
}

void FirstLevel::KillPlayer()
{
    if (playerDead)
    {
        return;
    }

    playerDead = true;
    running = false;
}

void FirstLevel::Init()
{
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    gorillaEncounter = false;
    levelComplete = false;
    playerDead = false;
    screenFade = 0.0f;

    ground.Init(GroundStyle::Main);
    playerY = ground.GetFloorY() - metadata::PLAYER_SIZE;
    jumpState.Reset();

    nora.Init();
    books.Init(ground.GetFloorY());
    chimps.Init(ground.GetFloorY());

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

    playerVelY += metadata::GRAVITY * dt;
    playerY += playerVelY * dt;

    const float groundLevel = ground.GetFloorY() - metadata::PLAYER_SIZE;
    if (playerY >= groundLevel)
    {
        playerY = groundLevel;
        playerVelY = 0.0f;
        jumpState.Land();
    }
    else
    {
        jumpState.grounded = false;
    }

    jumpState.TryJump(playerVelY);

    if (!gorillaEncounter && !playerDead)
    {
        chimps.Update(scrollOffset, screenWidth);

        if (chimps.CheckCollisions(playerX, playerY, static_cast<float>(metadata::PLAYER_SIZE), scrollOffset))
        {
            KillPlayer();
        }
    }

    if (books.IsActive())
    {
        books.Update(scrollOffset, screenWidth);
        books.CheckCollections(playerX, playerY, static_cast<float>(metadata::PLAYER_SIZE), scrollOffset, nora);

        if (nora.GetBooksCollected() >= metadata::BOOKS_TO_COMPLETE_LVL1)
        {
            BeginGorillaEncounter();
        }
    }

    if (gorillaEncounter)
    {
        screenFade = gorilla.GetFadeAlpha(scrollOffset, playerX);

        if (screenFade >= 1.0f)
        {
            levelComplete = true;
            running = false;
        }
    }
}

void FirstLevel::Draw()
{
    BeginDrawing();

    DrawBackground();

    ground.Draw(scrollOffset, screenWidth);
    books.Draw(scrollOffset);
    chimps.Draw(scrollOffset);
    gorilla.Draw(scrollOffset);

    nora.Draw(static_cast<int>(playerX), static_cast<int>(playerY));

    DrawText(TextFormat("Books: %d", nora.GetBooksCollected()), 10, 10, 20, WHITE);
    DrawScreenFade();

    EndDrawing();
}

void FirstLevel::Cleanup()
{
    books.Cleanup();
    chimps.Cleanup();
    gorilla.Cleanup();
    ground.Cleanup();
    nora.Cleanup();

    if (backgroundTexture.id != 0)
    {
        UnloadTexture(backgroundTexture);
        backgroundTexture = {};
    }
}
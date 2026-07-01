#include "secondlevel.hpp"

#include "assets/asset_paths.hpp"
#include "metadata/metadata.hpp"



void SecondLevel::DrawBackground()
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
        DrawRectangle(0, 0, screenWidth, screenHeight, {18, 18, 28, 255});
    }
}

float SecondLevel::GetForwardSpeed() const
{
    return metadata::LEVEL2_BASE_SPEED * speedMultiplier;
}

Rectangle SecondLevel::GetPlayerBounds() const
{
    return {
        playerX,
        playerY,
        static_cast<float>(metadata::PLAYER_SIZE),
        static_cast<float>(metadata::PLAYER_SIZE),
    };
}

void SecondLevel::UpdateSpeed()
{
    speedMultiplier = 1.0f + (scrollOffset / metadata::LEVEL2_SPEED_RAMP_SCALE);

    const bool shouldDisableObstacles =
        speedMultiplier >= metadata::LEVEL2_OBSTACLE_CUTOFF_MULTIPLIER;

    if (shouldDisableObstacles && obstaclePhase)
    {
        obstaclePhase = false;
        groundChimps.Deactivate();
        slidingChimps.Deactivate();
        books.SetFloodMode(true);
    }

    books.SetSpawnPressure(speedMultiplier);
}

void SecondLevel::KillPlayer()
{
    if (playerDead)
    {
        return;
    }

    playerDead = true;
    running = false;
}

void SecondLevel::PopulateBridgeBooks()
{
    for (int i = 0; i < bridges.GetBridgePoolSize(); ++i)
    {
        FloatingBridge& bridge = bridges.GetBridge(i);
        if (!bridge.NeedsBooks())
        {
            continue;
        }

        books.SpawnBooksOnPlatform(bridge.GetWorldStartX(), bridge.GetLengthTiles(), bridge.GetTopY());
        bridge.MarkBooksPlaced();
    }
}

void SecondLevel::UpdatePlayerPhysics()
{
    const float dt = GetFrameTime();

    playerVelY += metadata::GRAVITY * dt;
    playerY += playerVelY * dt;

    Rectangle playerBounds = GetPlayerBounds();
    const float floorLevel = ground.GetFloorY() - static_cast<float>(metadata::PLAYER_SIZE);
    float supportLevel = floorLevel;

    if (bridges.TryLandPlayer(playerBounds, playerVelY, scrollOffset))
    {
        supportLevel = playerBounds.y;
    }

    if (playerY >= supportLevel)
    {
        playerY = supportLevel;
        playerVelY = 0.0f;
        jumpState.Land();
    }
    else
    {
        jumpState.grounded = false;
    }

    jumpState.TryJump(playerVelY);

    if (!jumpState.grounded)
    {
        playerBounds = GetPlayerBounds();
        if (bridges.HitsSide(playerBounds, scrollOffset))
        {
            KillPlayer();
        }
    }
}

void SecondLevel::UpdateObstacles(const float dt)
{
    if (!obstaclePhase || playerDead)
    {
        return;
    }

    groundChimps.Update(scrollOffset, screenWidth);
    slidingChimps.Update(scrollOffset, screenWidth, dt);

    if (groundChimps.CheckCollisions(playerX, playerY, static_cast<float>(metadata::PLAYER_SIZE),
                                     scrollOffset))
    {
        KillPlayer();
        return;
    }

    if (slidingChimps.CheckCollisions(playerX, playerY, static_cast<float>(metadata::PLAYER_SIZE),
                                      scrollOffset))
    {
        KillPlayer();
    }
}

void SecondLevel::BeginVictoryRide()
{
    if (victoryRide)
    {
        return;
    }

    victoryRide = true;
    victoryRideTimer = 0.0f;

    books.Deactivate();
    groundChimps.Deactivate();
    slidingChimps.Deactivate();

    for (int i = 0; i < bridges.GetBridgePoolSize(); ++i)
    {
        bridges.GetBridge(i).Deactivate();
    }
}

void SecondLevel::CheckLevelComplete()
{
    if (victoryRide || levelComplete)
    {
        return;
    }

    if (nora.GetBooksCollected() >= metadata::BOOKS_TO_COMPLETE_LVL2)
    {
        BeginVictoryRide();
    }
}

void SecondLevel::UpdateVictoryRide(const float dt)
{
    UpdateSpeed();
    scrollOffset += GetForwardSpeed() * dt;
    victoryRideTimer += dt;

    playerVelY += metadata::GRAVITY * dt;
    playerY += playerVelY * dt;

    const float groundLevel = ground.GetFloorY() - static_cast<float>(metadata::PLAYER_SIZE);
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

    if (victoryRideTimer >= metadata::LEVEL2_VICTORY_RIDE_DURATION)
    {
        levelComplete = true;
        running = false;
    }
}

void SecondLevel::Init(const int startingBooks)
{
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    running = true;
    levelComplete = false;
    playerDead = false;
    obstaclePhase = true;
    victoryRide = false;
    victoryRideTimer = 0.0f;
    speedMultiplier = 1.0f;
    scrollOffset = 0.0f;

    ground.Init(GroundStyle::Main);
    playerY = ground.GetFloorY() - metadata::PLAYER_SIZE;
    jumpState.Reset();

    nora.Init(startingBooks);
    books.Init(ground.GetFloorY());
    bridges.Init();
    groundChimps.Init(ground.GetFloorY());
    slidingChimps.Init(ground.GetFloorY());

    const char* backgroundPath =
        AssetPaths::Resolve(metadata::STATIC_BACKGROUND_OMINOUS.data());
    Image backgroundImage = LoadImage(backgroundPath);
    if (IsImageValid(backgroundImage))
    {
        backgroundTexture = LoadTextureFromImage(backgroundImage);
        UnloadImage(backgroundImage);
    }
    else
    {
        TraceLog(LOG_WARNING, "SECONDLEVEL: Failed to load background image: %s", backgroundPath);
    }
}

void SecondLevel::Update()
{
    if (playerDead)
    {
        return;
    }

    const float dt = GetFrameTime();

    if (victoryRide)
    {
        UpdateVictoryRide(dt);
        return;
    }

    UpdateSpeed();
    scrollOffset += GetForwardSpeed() * dt;

    bridges.Update(scrollOffset, screenWidth, ground.GetFloorY());
    PopulateBridgeBooks();

    books.Update(scrollOffset, screenWidth);
    UpdateObstacles(dt);
    UpdatePlayerPhysics();

    books.CheckCollections(playerX, playerY, static_cast<float>(metadata::PLAYER_SIZE), scrollOffset,
                           nora);
    CheckLevelComplete();
}

void SecondLevel::Draw()
{
    BeginDrawing();

    DrawBackground();

    ground.Draw(scrollOffset, screenWidth);

    if (!victoryRide)
    {
        bridges.Draw(scrollOffset);
        books.Draw(scrollOffset);
        groundChimps.Draw(scrollOffset);
        slidingChimps.Draw(scrollOffset);
    }

    nora.Draw(static_cast<int>(playerX), static_cast<int>(playerY));

    if (victoryRide)
    {
        DrawText("Victory!", 10, 10, 20, GOLD);
    }
    else
    {
        DrawText(TextFormat("Books: %d / %d", nora.GetBooksCollected(), metadata::BOOKS_TO_COMPLETE_LVL2),
                 10, 10, 20, WHITE);
        DrawText(TextFormat("Speed: %.1fx", speedMultiplier), 10, 36, 18, LIGHTGRAY);
    }

    EndDrawing();
}

void SecondLevel::Cleanup()
{
    books.Cleanup();
    bridges.Cleanup();
    groundChimps.Cleanup();
    slidingChimps.Cleanup();
    ground.Cleanup();
    nora.Cleanup();

    if (backgroundTexture.id != 0)
    {
        UnloadTexture(backgroundTexture);
        backgroundTexture = {};
    }
}
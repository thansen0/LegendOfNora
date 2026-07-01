#pragma once

#include <raylib.h>

#include "assets/floating_bridge.hpp"
#include "assets/ground.hpp"
#include "book/book_spawner.hpp"
#include "entities/angry_chimp_spawner.hpp"
#include "entities/sliding_chimp_spawner.hpp"
#include "player/nora.hpp"
#include "player/player_jump.hpp"

// Speed-ramping book hunt: floating bridges, sliding chimps, and a final book flood.
class SecondLevel
{
public:
    void Init(int startingBooks = 0);
    void Update();
    void Draw();
    void Cleanup();

    [[nodiscard]] bool IsRunning() const { return running; }
    [[nodiscard]] bool IsComplete() const { return levelComplete; }
    [[nodiscard]] bool IsPlayerDead() const { return playerDead; }
    [[nodiscard]] int GetBooksCollected() const { return nora.GetBooksCollected(); }

private:
    bool running = true;
    bool levelComplete = false;
    bool playerDead = false;
    bool obstaclePhase = true;
    bool victoryRide = false;
    float victoryRideTimer = 0.0f;

    float playerX = 120.0f;
    float playerY = 0.0f;
    float playerVelY = 0.0f;
    PlayerJumpState jumpState{};

    float scrollOffset = 0.0f;
    float speedMultiplier = 1.0f;

    int screenWidth = 800;
    int screenHeight = 600;

    Texture2D backgroundTexture{};
    Ground ground{};
    Nora nora{};
    BookSpawner books{};
    FloatingBridgeManager bridges{};
    AngryChimpSpawner groundChimps{};
    SlidingChimpSpawner slidingChimps{};

    void DrawBackground();
    void UpdateSpeed();
    void UpdatePlayerPhysics();
    void UpdateObstacles(float dt);
    void PopulateBridgeBooks();
    void KillPlayer();
    void CheckLevelComplete();
    void BeginVictoryRide();
    void UpdateVictoryRide(float dt);

    [[nodiscard]] float GetForwardSpeed() const;
    [[nodiscard]] Rectangle GetPlayerBounds() const;
};
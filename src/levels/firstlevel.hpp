#pragma once

#include <raylib.h>

#include "assets/ground.hpp"
#include "assets/level_music.hpp"
#include "book/book_spawner.hpp"
#include "entities/angry_chimp_spawner.hpp"
#include "entities/gorilla.hpp"
#include "player/nora.hpp"
#include "player/player_jump.hpp"
#include "metadata/metadata.hpp"

class FirstLevel
{
public:
    void Init();
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
    bool gorillaEncounter = false;

    float playerX = 120.0f;
    float playerY = 0.0f;
    float playerVelY = 0.0f;
    PlayerJumpState jumpState{};

    float scrollOffset = 0.0f;
    float screenFade = 0.0f;

    int screenWidth = 800;
    int screenHeight = 600;

    Texture2D backgroundTexture{};
    Ground ground{};
    Nora nora{};
    BookSpawner books{};
    AngryChimpSpawner chimps{};
    Gorilla gorilla{};
    LevelMusic levelMusic{};

    void DrawBackground();
    void DrawScreenFade();
    void BeginGorillaEncounter();
    void KillPlayer();

    static const char* ResolveAssetPath(const char* relativePath);
};
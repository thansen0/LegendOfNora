#pragma once

#include <raylib.h>

#include "assets/ground.hpp"
#include "book/book_spawner.hpp"
#include "entities/gorilla.hpp"
#include "player/nora.hpp"
#include "metadata/metadata.hpp"

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
    bool gorillaEncounter = false;

    float playerX = 120.0f;
    float playerY = 0.0f;
    float playerVelY = 0.0f;
    bool grounded = false;

    float scrollOffset = 0.0f;
    float screenFade = 0.0f;

    int screenWidth = 800;
    int screenHeight = 600;

    Texture2D backgroundTexture{};
    Ground ground{};
    Nora nora{};
    BookSpawner books{};
    Gorilla gorilla{};

    void DrawBackground();
    void DrawScreenFade();
    void BeginGorillaEncounter();

    static const char* ResolveAssetPath(const char* relativePath);
};
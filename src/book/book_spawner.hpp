#pragma once

#include "book/book.hpp"

class Nora;

class BookSpawner
{
public:
    void Init(float floorY);
    void Update(float scrollOffset, int screenWidth);
    void Draw(float scrollOffset) const;
    void CheckCollections(float playerX, float playerY, float playerSize, float scrollOffset, Nora& nora);
    void Deactivate();
    void Cleanup();

    // Level 2 tuning helpers.
    void SetSpawnPressure(float multiplier);
    void SetFloodMode(bool enabled);
    void SpawnBooksOnPlatform(float worldStartX, int tileCount, float platformTopY);
    void SpawnFloodBooks(float scrollOffset, int screenWidth);

    [[nodiscard]] bool IsActive() const { return active; }

private:
    static constexpr int POOL_SIZE = 64;
    static constexpr float MIN_SPAWN_GAP = 64.0f;
    static constexpr float MAX_SPAWN_GAP = 300.0f;
    static constexpr float JUMP_HEIGHT_OFFSET = 94.0f;

    Book books[POOL_SIZE]{};
    float floorY = 0.0f;
    float nextSpawnWorldX = 0.0f;
    float spawnPressure = 1.0f;
    bool floodMode = false;
    bool active = true;

    Book* GetInactiveBook();
    void SpawnBook(float scrollOffset, int screenWidth);
    void CullOffscreen(float scrollOffset);
    void SpawnBookAt(float worldX, float y);
};
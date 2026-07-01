#pragma once

#include "assets/spritesheet.hpp"

#include <raylib.h>

// A short floating platform made from ground sheet tiles (column 8, rows 0-2).
class FloatingBridge
{
public:
    void Activate(float worldStartX, float topY, int lengthTiles, int variantRow);
    void Deactivate();

    [[nodiscard]] bool IsActive() const { return active; }
    [[nodiscard]] float GetWorldStartX() const { return worldStartX; }
    [[nodiscard]] float GetWorldEndX() const { return worldStartX + static_cast<float>(lengthTiles * tileSize); }
    [[nodiscard]] float GetTopY() const { return topY; }
    [[nodiscard]] int GetLengthTiles() const { return lengthTiles; }
    [[nodiscard]] bool NeedsBooks() const { return active && !booksPlaced; }

    void Draw(float scrollOffset, const SpriteSheet& sheet, const Rectangle& tileSource) const;
    void MarkBooksPlaced() { booksPlaced = true; }

    // Returns true when the player strikes the side or underside of the bridge.
    [[nodiscard]] bool HitsSide(const Rectangle& playerBounds, float scrollOffset) const;

    // Snaps a falling player onto the bridge deck when appropriate.
    [[nodiscard]] bool TryLandPlayer(Rectangle& playerBounds, float playerVelY, float scrollOffset) const;

private:
    static constexpr int tileSize = 64;
    static constexpr float landingTolerance = 10.0f;

    float worldStartX = 0.0f;
    float topY = 0.0f;
    int lengthTiles = 0;
    int variantRow = 0;
    bool active = false;
    bool booksPlaced = false;

    [[nodiscard]] Rectangle GetBounds(float scrollOffset) const;
    [[nodiscard]] bool HasHorizontalOverlap(const Rectangle& playerBounds, float scrollOffset) const;
    [[nodiscard]] bool IsOnDeck(const Rectangle& playerBounds) const;
};

// Spawns and manages floating bridges ahead of the player.
class FloatingBridgeManager
{
public:
    void Init();
    void Update(float scrollOffset, int screenWidth, float floorY);
    void Draw(float scrollOffset) const;
    void Cleanup();

    [[nodiscard]] FloatingBridge& GetBridge(int index);
    [[nodiscard]] int GetBridgePoolSize() const { return POOL_SIZE; }

    [[nodiscard]] bool HitsSide(const Rectangle& playerBounds, float scrollOffset) const;
    [[nodiscard]] bool TryLandPlayer(Rectangle& playerBounds, float playerVelY, float scrollOffset) const;

private:
    static constexpr int POOL_SIZE = 10;
    static constexpr int MIN_BRIDGE_LENGTH = 5;
    static constexpr int MAX_BRIDGE_LENGTH = 12;
    static constexpr float MIN_SPAWN_GAP = 700.0f;
    static constexpr float MAX_SPAWN_GAP = 1200.0f;

    FloatingBridge bridges[POOL_SIZE]{};
    SpriteSheet sheet{};
    Rectangle tileSource{};
    float nextSpawnWorldX = 0.0f;

    FloatingBridge* GetInactiveBridge();
    void SpawnBridge(float scrollOffset, int screenWidth, float floorY);
    void CullOffscreen(float scrollOffset);
};

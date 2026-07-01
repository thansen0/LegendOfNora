#include "floating_bridge.hpp"

#include "metadata/metadata.hpp"

#include <cmath>

namespace
{
    Rectangle GetFloatingTileSource(const int variantRow)
    {
        return {
            static_cast<float>(metadata::FLOATING_BRIDGE_TILE_COLUMN * metadata::TILE_SIZE),
            static_cast<float>(variantRow * metadata::TILE_SIZE),
            static_cast<float>(metadata::TILE_SIZE),
            static_cast<float>(metadata::TILE_SIZE),
        };
    }
}

void FloatingBridge::Activate(const float worldStartX, const float topY, const int lengthTiles,
                              const int variantRow)
{
    this->worldStartX = worldStartX;
    this->topY = topY;
    this->lengthTiles = lengthTiles;
    this->variantRow = variantRow;
    booksPlaced = false;
    active = true;
}

void FloatingBridge::Deactivate()
{
    active = false;
    lengthTiles = 0;
}

Rectangle FloatingBridge::GetBounds(const float scrollOffset) const
{
    return {
        worldStartX - scrollOffset,
        topY,
        static_cast<float>(lengthTiles * tileSize),
        static_cast<float>(tileSize),
    };
}

void FloatingBridge::Draw(const float scrollOffset, const SpriteSheet& sheet,
                          const Rectangle& tileSource) const
{
    if (!active || !sheet.IsLoaded())
    {
        return;
    }

    const Rectangle source = {
        tileSource.x,
        tileSource.y + static_cast<float>(variantRow * tileSize),
        tileSource.width,
        tileSource.height,
    };

    for (int tile = 0; tile < lengthTiles; ++tile)
    {
        const float screenX = worldStartX - scrollOffset + static_cast<float>(tile * tileSize);
        sheet.DrawRegion(source, screenX, topY);
    }
}

bool FloatingBridge::HasHorizontalOverlap(const Rectangle& playerBounds,
                                          const float scrollOffset) const
{
    const Rectangle bridgeBounds = GetBounds(scrollOffset);
    return playerBounds.x < bridgeBounds.x + bridgeBounds.width &&
           playerBounds.x + playerBounds.width > bridgeBounds.x;
}

bool FloatingBridge::IsOnDeck(const Rectangle& playerBounds) const
{
    const float playerBottom = playerBounds.y + playerBounds.height;
    return playerBottom >= topY - landingTolerance && playerBottom <= topY + landingTolerance;
}

bool FloatingBridge::HitsSide(const Rectangle& playerBounds, const float scrollOffset) const
{
    if (!active)
    {
        return false;
    }

    const Rectangle bridgeBounds = GetBounds(scrollOffset);
    if (!CheckCollisionRecs(playerBounds, bridgeBounds))
    {
        return false;
    }

    if (IsOnDeck(playerBounds))
    {
        return false;
    }

    return true;
}

bool FloatingBridge::TryLandPlayer(Rectangle& playerBounds, const float playerVelY,
                                 const float scrollOffset) const
{
    if (!active || playerVelY < 0.0f)
    {
        return false;
    }

    if (!HasHorizontalOverlap(playerBounds, scrollOffset))
    {
        return false;
    }

    const float playerBottom = playerBounds.y + playerBounds.height;

    // Accept any downward entry into the top tile so fast falls still snap to the deck.
    if (playerBottom < topY || playerBottom > topY + static_cast<float>(tileSize))
    {
        return false;
    }

    playerBounds.y = topY - playerBounds.height;
    return true;
}

FloatingBridge* FloatingBridgeManager::GetInactiveBridge()
{
    for (FloatingBridge& bridge : bridges)
    {
        if (!bridge.IsActive())
        {
            return &bridge;
        }
    }

    return nullptr;
}

void FloatingBridgeManager::SpawnBridge(const float scrollOffset, const int screenWidth,
                                        const float floorY)
{
    FloatingBridge* bridge = GetInactiveBridge();
    if (bridge == nullptr)
    {
        return;
    }

    const int lengthTiles = GetRandomValue(MIN_BRIDGE_LENGTH, MAX_BRIDGE_LENGTH);
    const int variantRow = GetRandomValue(0, 2);
    const float gap = static_cast<float>(GetRandomValue(static_cast<int>(MIN_SPAWN_GAP),
                                                        static_cast<int>(MAX_SPAWN_GAP)));
    const float worldStartX = scrollOffset + static_cast<float>(screenWidth) + gap;
    // Keep within jump reach (~128px max with current physics).
    const float topY = floorY - static_cast<float>(GetRandomValue(60, 110));

    bridge->Activate(worldStartX, topY, lengthTiles, variantRow);
    nextSpawnWorldX = bridge->GetWorldEndX() + MIN_SPAWN_GAP;
}

void FloatingBridgeManager::CullOffscreen(const float scrollOffset)
{
    for (FloatingBridge& bridge : bridges)
    {
        if (!bridge.IsActive())
        {
            continue;
        }

        if (bridge.GetWorldEndX() - scrollOffset < -static_cast<float>(metadata::TILE_SIZE))
        {
            bridge.Deactivate();
        }
    }
}

void FloatingBridgeManager::Init()
{
    tileSource = GetFloatingTileSource(0);

    if (!sheet.Load(metadata::GROUND_SHEET.data()))
    {
        TraceLog(LOG_WARNING, "FLOATING_BRIDGE: Failed to load sprite sheet");
    }

    for (FloatingBridge& bridge : bridges)
    {
        bridge.Deactivate();
    }

    nextSpawnWorldX = static_cast<float>(GetScreenWidth()) + MIN_SPAWN_GAP;
}

void FloatingBridgeManager::Update(const float scrollOffset, const int screenWidth, const float floorY)
{
    CullOffscreen(scrollOffset);

    if (scrollOffset + static_cast<float>(screenWidth) >= nextSpawnWorldX)
    {
        SpawnBridge(scrollOffset, screenWidth, floorY);
    }
}

void FloatingBridgeManager::Draw(const float scrollOffset) const
{
    if (!sheet.IsLoaded())
    {
        return;
    }

    for (const FloatingBridge& bridge : bridges)
    {
        bridge.Draw(scrollOffset, sheet, tileSource);
    }
}

bool FloatingBridgeManager::HitsSide(const Rectangle& playerBounds, const float scrollOffset) const
{
    for (const FloatingBridge& bridge : bridges)
    {
        if (bridge.HitsSide(playerBounds, scrollOffset))
        {
            return true;
        }
    }

    return false;
}

bool FloatingBridgeManager::TryLandPlayer(Rectangle& playerBounds, const float playerVelY,
                                        const float scrollOffset) const
{
    bool landed = false;

    for (const FloatingBridge& bridge : bridges)
    {
        if (bridge.TryLandPlayer(playerBounds, playerVelY, scrollOffset))
        {
            landed = true;
        }
    }

    return landed;
}

FloatingBridge& FloatingBridgeManager::GetBridge(const int index)
{
    return bridges[index];
}

void FloatingBridgeManager::Cleanup()
{
    for (FloatingBridge& bridge : bridges)
    {
        bridge.Deactivate();
    }

    sheet.Unload();
    nextSpawnWorldX = 0.0f;
}
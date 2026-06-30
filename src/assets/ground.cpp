#include "ground.hpp"

#include <cmath>

Ground::GroundAsset Ground::GetAsset(const GroundStyle style)
{
    switch (style)
    {
    case GroundStyle::Main:
    default:
        return {
            .sheetPath = "assets/ground/main.png",
            .sourceRect = {64.0f, 0.0f, 64.0f, 64.0f},
            .tileSize = 64,
        };
    }
}

void Ground::Init(const GroundStyle style)
{
    Cleanup();

    const GroundAsset asset = GetAsset(style);
    sourceRect = asset.sourceRect;
    tileSize = asset.tileSize;
    floorY = static_cast<float>(GetScreenHeight()) - tileSize;

    if (!sheet.Load(asset.sheetPath))
    {
        TraceLog(LOG_WARNING, "GROUND: Failed to load sprite sheet for style %d", static_cast<int>(style));
    }
}

void Ground::Draw(const float scrollOffset, const int screenWidth) const
{
    if (!sheet.IsLoaded())
    {
        return;
    }

    const float tileOffset = fmodf(scrollOffset, static_cast<float>(tileSize));

    for (float x = -tileOffset; x < static_cast<float>(screenWidth); x += tileSize)
    {
        sheet.DrawRegion(sourceRect, x, floorY);
    }
}

void Ground::Cleanup()
{
    sheet.Unload();
    sourceRect = {};
    tileSize = 64;
    floorY = 0.0f;
}
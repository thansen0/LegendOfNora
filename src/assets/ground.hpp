#pragma once

#include <raylib.h>

#include "assets/spritesheet.hpp"

enum class GroundStyle
{
    Main,
};

class Ground
{
public:
    void Init(GroundStyle style = GroundStyle::Main);
    void Draw(float scrollOffset, int screenWidth) const;
    void Cleanup();

    [[nodiscard]] bool IsLoaded() const { return sheet.IsLoaded(); }
    [[nodiscard]] float GetFloorY() const { return floorY; }
    [[nodiscard]] int GetTileSize() const { return tileSize; }

private:
    struct GroundAsset
    {
        const char* sheetPath;
        Rectangle sourceRect;
        int tileSize;
    };

    static GroundAsset GetAsset(GroundStyle style);

    SpriteSheet sheet{};
    Rectangle sourceRect{};
    int tileSize = 64;
    float floorY = 0.0f;
};
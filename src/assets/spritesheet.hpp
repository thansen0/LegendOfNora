#pragma once

#include <raylib.h>

class SpriteSheet
{
public:
    SpriteSheet() = default;
    ~SpriteSheet();

    SpriteSheet(const SpriteSheet&) = delete;
    SpriteSheet& operator=(const SpriteSheet&) = delete;

    bool Load(const char* relativePath);
    void Unload();

    [[nodiscard]] bool IsLoaded() const { return texture.id != 0; }

    void DrawRegion(Rectangle source, float x, float y, Color tint = WHITE) const;
    void DrawRegionPro(Rectangle source, Rectangle dest, Color tint = WHITE) const;

private:
    Texture2D texture{};
};
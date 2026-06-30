#include "spritesheet.hpp"

#include "asset_paths.hpp"

SpriteSheet::~SpriteSheet()
{
    Unload();
}

bool SpriteSheet::Load(const char* relativePath)
{
    Unload();

    const char* path = AssetPaths::Resolve(relativePath);
    Image image = LoadImage(path);
    if (!IsImageValid(image))
    {
        TraceLog(LOG_WARNING, "SPRITESHEET: Failed to load image: %s", path);
        return false;
    }

    ImageColorReplace(&image, BLACK, BLANK);

    texture = LoadTextureFromImage(image);
    UnloadImage(image);

    if (texture.id == 0)
    {
        TraceLog(LOG_WARNING, "SPRITESHEET: Failed to upload texture: %s", path);
        return false;
    }

    SetTextureFilter(texture, TEXTURE_FILTER_POINT);
    return true;
}

void SpriteSheet::Unload()
{
    if (texture.id != 0)
    {
        UnloadTexture(texture);
        texture = {};
    }
}

void SpriteSheet::DrawRegion(const Rectangle source, const float x, const float y, const Color tint) const
{
    if (!IsLoaded())
    {
        return;
    }

    DrawTextureRec(texture, source, {x, y}, tint);
}

void SpriteSheet::DrawRegionPro(const Rectangle source, const Rectangle dest, const Color tint) const
{
    if (!IsLoaded())
    {
        return;
    }

    DrawTexturePro(texture, source, dest, {0, 0}, 0.0f, tint);
}
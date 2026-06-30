#include "gorilla.hpp"

#include "assets/asset_paths.hpp"
#include "metadata/metadata.hpp"

#include <algorithm>

Texture2D Gorilla::texture{};
bool Gorilla::assetsLoaded = false;

void Gorilla::LoadAssets()
{
    if (assetsLoaded)
    {
        return;
    }

    const char* path = AssetPaths::Resolve(metadata::GORILLA_TEXTURE.data());
    Image image = LoadImage(path);
    if (!IsImageValid(image))
    {
        TraceLog(LOG_WARNING, "GORILLA: Failed to load image: %s", path);
        return;
    }

    ImageColorReplace(&image, BLACK, BLANK);
    texture = LoadTextureFromImage(image);
    UnloadImage(image);

    if (texture.id == 0)
    {
        TraceLog(LOG_WARNING, "GORILLA: Failed to upload texture: %s", path);
        return;
    }

    SetTextureFilter(texture, TEXTURE_FILTER_POINT);
    assetsLoaded = true;
}

void Gorilla::UnloadAssets()
{
    if (assetsLoaded)
    {
        UnloadTexture(texture);
        texture = {};
        assetsLoaded = false;
    }
}

void Gorilla::Enter(const float scrollOffset, const int screenWidth, const float floorY)
{
    LoadAssets();

    worldX = scrollOffset + static_cast<float>(screenWidth) + 80.0f;
    y = floorY - static_cast<float>(metadata::GORILLA_SIZE) + 64;
    active = assetsLoaded;
}

float Gorilla::GetFadeAlpha(const float scrollOffset, const float playerX) const
{
    if (!active)
    {
        return 0.0f;
    }

    const float distance = GetScreenX(scrollOffset) - playerX;
    if (distance >= FADE_START_DISTANCE)
    {
        return 0.0f;
    }

    return std::clamp(1.0f - (distance / FADE_START_DISTANCE), 0.0f, 1.0f);
}

void Gorilla::Draw(const float scrollOffset) const
{
    if (!active || !assetsLoaded)
    {
        return;
    }

    const float screenX = GetScreenX(scrollOffset);
    DrawTexture(texture, static_cast<int>(screenX), static_cast<int>(y), WHITE);
}

void Gorilla::Cleanup()
{
    active = false;
    UnloadAssets();
}
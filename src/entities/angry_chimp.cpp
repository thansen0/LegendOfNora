#include "angry_chimp.hpp"

#include "assets/asset_paths.hpp"
#include "metadata/metadata.hpp"

Texture2D AngryChimp::texture{};
bool AngryChimp::assetsLoaded = false;

void AngryChimp::LoadAssets()
{
    if (assetsLoaded)
    {
        return;
    }

    const char* path = AssetPaths::Resolve(metadata::ANGRY_CHIMP_TEXTURE.data());
    Image image = LoadImage(path);
    if (!IsImageValid(image))
    {
        TraceLog(LOG_WARNING, "ANGRY_CHIMP: Failed to load image: %s", path);
        return;
    }

    ImageColorReplace(&image, BLACK, BLANK);
    texture = LoadTextureFromImage(image);
    UnloadImage(image);

    if (texture.id == 0)
    {
        TraceLog(LOG_WARNING, "ANGRY_CHIMP: Failed to upload texture: %s", path);
        return;
    }

    SetTextureFilter(texture, TEXTURE_FILTER_POINT);
    assetsLoaded = true;
}

void AngryChimp::UnloadAssets()
{
    if (assetsLoaded)
    {
        UnloadTexture(texture);
        texture = {};
        assetsLoaded = false;
    }
}

void AngryChimp::Spawn(const float worldX, const float y)
{
    this->worldX = worldX;
    this->y = y;
    active = true;
}

void AngryChimp::Despawn()
{
    active = false;
}

void AngryChimp::Draw(const float scrollOffset) const
{
    if (!active || !assetsLoaded)
    {
        return;
    }

    const float screenX = GetScreenX(scrollOffset);
    const float size = static_cast<float>(metadata::ANGRY_CHIMP_DISPLAY_SIZE);

    DrawTexturePro(
        texture,
        {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)},
        {screenX, y, size, size},
        {0, 0},
        0.0f,
        WHITE);
}

bool AngryChimp::CollidesWith(const float playerX, const float playerY, const float playerSize,
                             const float scrollOffset) const
{
    if (!active)
    {
        return false;
    }

    const float size = static_cast<float>(metadata::ANGRY_CHIMP_DISPLAY_SIZE);
    const float inset = static_cast<float>(metadata::ANGRY_CHIMP_HITBOX_INSET);
    const float screenX = worldX - scrollOffset;
    const Rectangle chimpBounds = {
        screenX + inset,
        y + inset,
        size - (2.0f * inset),
        size - (2.0f * inset),
    };

    const Rectangle playerBounds = {
        playerX,
        playerY,
        playerSize,
        playerSize,
    };

    return CheckCollisionRecs(chimpBounds, playerBounds);
}
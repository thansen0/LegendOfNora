#include "sliding_chimp.hpp"

#include "assets/asset_paths.hpp"
#include "metadata/metadata.hpp"

#include <cmath>

Texture2D SlidingChimp::texture{};
bool SlidingChimp::assetsLoaded = false;

void SlidingChimp::LoadAssets()
{
    if (assetsLoaded)
    {
        return;
    }

    const char* path = AssetPaths::Resolve(metadata::ANGRY_CHIMP_TEXTURE.data());
    Image image = LoadImage(path);
    if (!IsImageValid(image))
    {
        TraceLog(LOG_WARNING, "SLIDING_CHIMP: Failed to load image: %s", path);
        return;
    }

    ImageColorReplace(&image, BLACK, BLANK);
    texture = LoadTextureFromImage(image);
    UnloadImage(image);

    if (texture.id == 0)
    {
        TraceLog(LOG_WARNING, "SLIDING_CHIMP: Failed to upload texture: %s", path);
        return;
    }

    SetTextureFilter(texture, TEXTURE_FILTER_POINT);
    assetsLoaded = true;
}

void SlidingChimp::UnloadAssets()
{
    if (assetsLoaded)
    {
        UnloadTexture(texture);
        texture = {};
        assetsLoaded = false;
    }
}

void SlidingChimp::Spawn(const float anchorWorldX, const float y)
{
    this->anchorWorldX = anchorWorldX;
    this->y = y;
    patrolPhase = static_cast<float>(GetRandomValue(0, 628)) / 100.0f;
    active = true;
}

void SlidingChimp::Despawn()
{
    active = false;
}

float SlidingChimp::GetWorldX() const
{
    return anchorWorldX + std::sinf(patrolPhase) * patrolRange;
}

void SlidingChimp::Update(const float dt)
{
    if (!active)
    {
        return;
    }

    patrolPhase += patrolSpeed * dt;
}

void SlidingChimp::Draw(const float scrollOffset) const
{
    if (!active || !assetsLoaded)
    {
        return;
    }

    const float screenX = GetWorldX() - scrollOffset;
    const float size = static_cast<float>(metadata::ANGRY_CHIMP_DISPLAY_SIZE);

    DrawTexturePro(
        texture,
        {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)},
        {screenX, y, size, size},
        {0, 0},
        0.0f,
        WHITE);
}

bool SlidingChimp::CollidesWith(const float playerX, const float playerY, const float playerSize,
                               const float scrollOffset) const
{
    if (!active)
    {
        return false;
    }

    const float size = static_cast<float>(metadata::ANGRY_CHIMP_DISPLAY_SIZE);
    const float inset = static_cast<float>(metadata::ANGRY_CHIMP_HITBOX_INSET);
    const float screenX = GetWorldX() - scrollOffset;

    const Rectangle chimpBounds = {
        screenX + inset,
        y + inset,
        size - (2.0f * inset),
        size - (2.0f * inset),
    };

    const Rectangle playerBounds = {playerX, playerY, playerSize, playerSize};
    return CheckCollisionRecs(chimpBounds, playerBounds);
}
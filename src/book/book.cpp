#include "book.hpp"

#include "assets/asset_paths.hpp"
#include "metadata/metadata.hpp"

Texture2D Book::texture{};
bool Book::assetsLoaded = false;

void Book::LoadAssets()
{
    if (assetsLoaded)
    {
        return;
    }

    const char* path = AssetPaths::Resolve(metadata::BOOK_TEXTURE.data());
    Image image = LoadImage(path);
    if (!IsImageValid(image))
    {
        TraceLog(LOG_WARNING, "BOOK: Failed to load image: %s", path);
        return;
    }

    ImageColorReplace(&image, BLACK, BLANK);
    texture = LoadTextureFromImage(image);
    UnloadImage(image);

    if (texture.id == 0)
    {
        TraceLog(LOG_WARNING, "BOOK: Failed to upload texture: %s", path);
        return;
    }

    SetTextureFilter(texture, TEXTURE_FILTER_POINT);
    assetsLoaded = true;
}

void Book::UnloadAssets()
{
    if (assetsLoaded)
    {
        UnloadTexture(texture);
        texture = {};
        assetsLoaded = false;
    }
}

void Book::Spawn(const float worldX, const float y)
{
    this->worldX = worldX;
    this->y = y;
    active = true;
}

void Book::Despawn()
{
    active = false;
}

void Book::Draw(const float scrollOffset) const
{
    if (!active || !assetsLoaded)
    {
        return;
    }

    const float screenX = worldX - scrollOffset;
    DrawTexture(texture, static_cast<int>(screenX), static_cast<int>(y), WHITE);
}

bool Book::CollidesWith(const float playerX, const float playerY, const float playerSize,
                        const float scrollOffset) const
{
    if (!active)
    {
        return false;
    }

    const Rectangle bookBounds = {
        worldX - scrollOffset,
        y,
        static_cast<float>(metadata::BOOK_SIZE),
        static_cast<float>(metadata::BOOK_SIZE),
    };

    const Rectangle playerBounds = {
        playerX,
        playerY,
        playerSize,
        playerSize,
    };

    return CheckCollisionRecs(bookBounds, playerBounds);
}
#include "nora.hpp"

#include "assets/asset_paths.hpp"
#include "metadata/metadata.hpp"

Texture2D Nora::texture{};
bool Nora::assetsLoaded = false;

Nora::Nora() : playerSize(metadata::PLAYER_SIZE) {}

void Nora::LoadAssets()
{
    if (assetsLoaded)
    {
        return;
    }

    const char* path = AssetPaths::Resolve(metadata::NORA_TEXTURE.data());
    Image image = LoadImage(path);
    if (!IsImageValid(image))
    {
        TraceLog(LOG_WARNING, "NORA: Failed to load image: %s", path);
        return;
    }

    ImageColorReplace(&image, BLACK, BLANK);
    texture = LoadTextureFromImage(image);
    UnloadImage(image);

    if (texture.id == 0)
    {
        TraceLog(LOG_WARNING, "NORA: Failed to upload texture: %s", path);
        return;
    }

    SetTextureFilter(texture, TEXTURE_FILTER_POINT);
    assetsLoaded = true;
}

void Nora::UnloadAssets()
{
    if (assetsLoaded)
    {
        UnloadTexture(texture);
        texture = {};
        assetsLoaded = false;
    }
}

void Nora::Init(const int startingBooks)
{
    LoadAssets();
    booksCollected = startingBooks;
}

void Nora::Draw(const int playerX, const int playerY)
{
    if (!assetsLoaded)
    {
        DrawRectangle(playerX, playerY, playerSize, playerSize, GRAY);
        return;
    }

    const float displayHeight = static_cast<float>(metadata::NORA_DISPLAY_HEIGHT);
    const float aspectRatio =
        static_cast<float>(texture.width) / static_cast<float>(texture.height);
    const float displayWidth = displayHeight * aspectRatio;

    // Keep the motorcycle wheels on the ground while the hitbox stays compact for gameplay.
    const float drawX = static_cast<float>(playerX) - (displayWidth - static_cast<float>(playerSize)) / 2.0f;
    const float drawY =
        static_cast<float>(playerY + playerSize) - displayHeight;

    DrawTexturePro(
        texture,
        {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)},
        {drawX, drawY, displayWidth, displayHeight},
        {0, 0},
        0.0f,
        WHITE);
}

void Nora::CollectBook()
{
    ++booksCollected;
}

void Nora::Cleanup()
{
    UnloadAssets();
}
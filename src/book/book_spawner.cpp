#include "book_spawner.hpp"

#include "metadata/metadata.hpp"
#include "player/nora.hpp"

#include <algorithm>

#include <raylib.h>

Book* BookSpawner::GetInactiveBook()
{
    for (Book& book : books)
    {
        if (!book.IsActive())
        {
            return &book;
        }
    }

    return nullptr;
}

void BookSpawner::SpawnBookAt(const float worldX, const float y)
{
    Book* book = GetInactiveBook();
    if (book == nullptr)
    {
        return;
    }

    book->Spawn(worldX, y);
}

void BookSpawner::SpawnBook(const float scrollOffset, const int screenWidth)
{
    const bool atJumpHeight = GetRandomValue(0, 1) == 1;
    const float y = atJumpHeight ? floorY - metadata::BOOK_SIZE - JUMP_HEIGHT_OFFSET
                                 : floorY - metadata::BOOK_SIZE;

    const float minGap = std::max(24.0f, MIN_SPAWN_GAP / spawnPressure);
    const float maxGap = std::max(minGap + 20.0f, MAX_SPAWN_GAP / spawnPressure);
    const float gap = static_cast<float>(GetRandomValue(static_cast<int>(minGap), static_cast<int>(maxGap)));
    const float worldX = scrollOffset + static_cast<float>(screenWidth) + gap;

    SpawnBookAt(worldX, y);
    nextSpawnWorldX = worldX + minGap;
}

void BookSpawner::SpawnBooksOnPlatform(const float worldStartX, const int tileCount,
                                       const float platformTopY)
{
    const float bookY = platformTopY - static_cast<float>(metadata::BOOK_SIZE);

    for (int tile = 0; tile < tileCount; ++tile)
    {
        const float worldX = worldStartX + static_cast<float>(tile * metadata::TILE_SIZE);
        SpawnBookAt(worldX, bookY);
    }
}

void BookSpawner::SpawnFloodBooks(const float scrollOffset, const int screenWidth)
{
    constexpr int booksPerWave = 6;

    for (int i = 0; i < booksPerWave; ++i)
    {
        const float worldX = scrollOffset + static_cast<float>(GetRandomValue(0, screenWidth));
        const float y = static_cast<float>(GetRandomValue(80, static_cast<int>(floorY - metadata::BOOK_SIZE)));
        SpawnBookAt(worldX, y);
    }
}

void BookSpawner::CullOffscreen(const float scrollOffset)
{
    for (Book& book : books)
    {
        if (!book.IsActive())
        {
            continue;
        }

        if (book.GetScreenX(scrollOffset) < -static_cast<float>(metadata::BOOK_SIZE))
        {
            book.Despawn();
        }
    }
}

void BookSpawner::SetSpawnPressure(const float multiplier)
{
    spawnPressure = std::max(0.5f, multiplier);
}

void BookSpawner::SetFloodMode(const bool enabled)
{
    floodMode = enabled;
}

void BookSpawner::Deactivate()
{
    active = false;

    for (Book& book : books)
    {
        book.Despawn();
    }
}

void BookSpawner::Init(const float floorY)
{
    this->floorY = floorY;
    nextSpawnWorldX = 0.0f;
    spawnPressure = 1.0f;
    floodMode = false;
    active = true;

    Book::LoadAssets();

    for (Book& book : books)
    {
        book.Despawn();
    }

    SpawnBook(0.0f, GetScreenWidth());
}

void BookSpawner::Update(const float scrollOffset, const int screenWidth)
{
    if (!active)
    {
        return;
    }

    CullOffscreen(scrollOffset);

    if (floodMode)
    {
        SpawnFloodBooks(scrollOffset, screenWidth);
        return;
    }

    int spawnGuard = 0;
    while (scrollOffset + static_cast<float>(screenWidth) >= nextSpawnWorldX && spawnGuard < 4)
    {
        SpawnBook(scrollOffset, screenWidth);
        ++spawnGuard;
    }
}

void BookSpawner::Draw(const float scrollOffset) const
{
    if (!active)
    {
        return;
    }

    for (const Book& book : books)
    {
        book.Draw(scrollOffset);
    }
}

void BookSpawner::CheckCollections(const float playerX, const float playerY, const float playerSize,
                                   const float scrollOffset, Nora& nora)
{
    if (!active)
    {
        return;
    }

    for (Book& book : books)
    {
        if (!book.IsActive())
        {
            continue;
        }

        if (book.CollidesWith(playerX, playerY, playerSize, scrollOffset))
        {
            book.Despawn();
            nora.CollectBook();
        }
    }
}

void BookSpawner::Cleanup()
{
    for (Book& book : books)
    {
        book.Despawn();
    }

    Book::UnloadAssets();
}
#include "book_spawner.hpp"

#include "metadata/metadata.hpp"
#include "player/nora.hpp"

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

void BookSpawner::SpawnBook(const float scrollOffset, const int screenWidth)
{
    Book* book = GetInactiveBook();
    if (book == nullptr)
    {
        return;
    }

    const bool atJumpHeight = GetRandomValue(0, 1) == 1;
    const float y = atJumpHeight ? floorY - metadata::BOOK_SIZE - JUMP_HEIGHT_OFFSET
                                 : floorY - metadata::BOOK_SIZE;

    const float gap = static_cast<float>(GetRandomValue(static_cast<int>(MIN_SPAWN_GAP),
                                                        static_cast<int>(MAX_SPAWN_GAP)));
    const float worldX = scrollOffset + static_cast<float>(screenWidth) + gap;

    book->Spawn(worldX, y);
    nextSpawnWorldX = worldX + MIN_SPAWN_GAP;
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

    if (scrollOffset + static_cast<float>(screenWidth) >= nextSpawnWorldX)
    {
        SpawnBook(scrollOffset, screenWidth);
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
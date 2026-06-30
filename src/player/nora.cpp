#include "nora.hpp"

#include "metadata/metadata.hpp"

#include <raylib.h>

Nora::Nora() : playerSize(metadata::PLAYER_SIZE) {}

void Nora::Init()
{
    booksCollected = 0;
}

void Nora::Draw(const int playerX, const int playerY)
{
    DrawRectangle(playerX, playerY, playerSize, playerSize, GRAY);
}

void Nora::CollectBook()
{
    ++booksCollected;
}

void Nora::Cleanup()
{
}
#include "nora.hpp"
#include "metadata/metadata.hpp"

#include <raylib.h>

Nora::Nora() : PLAYER_SIZE{metadata::PLAYER_SIZE} {}

void Nora::Init() {
    books_collected = 0;
}

void Nora::Draw(int playerX, int playerY) {
    DrawRectangle(playerX, playerY, static_cast<int>(PLAYER_SIZE),
                  static_cast<int>(PLAYER_SIZE), GRAY);
}

void Nora::Cleanup() {
    // do nothing as of now...
}

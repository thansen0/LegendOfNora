#include "player_jump.hpp"

#include "metadata/metadata.hpp"

#include <raylib.h>

void PlayerJumpState::Reset()
{
    grounded = true;
    jumpsRemaining = metadata::MAX_JUMPS;
}

void PlayerJumpState::Land()
{
    grounded = true;
    jumpsRemaining = metadata::MAX_JUMPS;
}

bool PlayerJumpState::TryJump(float& playerVelY)
{
    if (!IsKeyPressed(KEY_SPACE) || jumpsRemaining <= 0)
    {
        return false;
    }

    playerVelY = metadata::JUMP_VELOCITY;

    --jumpsRemaining;
    grounded = false;
    return true;
}
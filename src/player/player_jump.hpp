#pragma once

// Tracks grounded state and double-jump availability for side-scrolling levels.
struct PlayerJumpState
{
    bool grounded = true;
    int jumpsRemaining = 2;

    void Reset();
    void Land();

    // Returns true when a jump was triggered this frame.
    bool TryJump(float& playerVelY);
};
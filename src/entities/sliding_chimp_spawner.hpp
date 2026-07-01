#pragma once

#include "entities/sliding_chimp.hpp"

class SlidingChimpSpawner
{
public:
    void Init(float floorY);
    void Update(float scrollOffset, int screenWidth, float dt);
    void Draw(float scrollOffset) const;
    bool CheckCollisions(float playerX, float playerY, float playerSize, float scrollOffset);
    void Deactivate();
    void Cleanup();

    [[nodiscard]] bool IsActive() const { return active; }

private:
    static constexpr int POOL_SIZE = 8;
    static constexpr float MIN_SPAWN_GAP = 900.0f;
    static constexpr float MAX_SPAWN_GAP = 1600.0f;
    static constexpr int SPAWN_CHANCE_PERCENT = 45;

    SlidingChimp chimps[POOL_SIZE]{};
    float floorY = 0.0f;
    float nextSpawnWorldX = 0.0f;
    bool active = true;

    SlidingChimp* GetInactiveChimp();
    void TrySpawnChimp(float scrollOffset, int screenWidth);
    void CullOffscreen(float scrollOffset);
};
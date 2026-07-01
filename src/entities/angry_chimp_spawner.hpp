#pragma once

#include "entities/angry_chimp.hpp"

class AngryChimpSpawner
{
public:
    void Init(float floorY);
    void Update(float scrollOffset, int screenWidth);
    void Draw(float scrollOffset) const;
    bool CheckCollisions(float playerX, float playerY, float playerSize, float scrollOffset);
    void Deactivate();
    void Cleanup();

    [[nodiscard]] bool IsActive() const { return active; }

private:
    static constexpr int POOL_SIZE = 12;
    static constexpr float MIN_SPAWN_GAP = 320.0f;
    static constexpr float MAX_SPAWN_GAP = 720.0f;
    static constexpr int SPAWN_CHANCE_PERCENT = 55;

    AngryChimp chimps[POOL_SIZE]{};
    float floorY = 0.0f;
    float nextSpawnWorldX = 0.0f;
    bool active = true;

    AngryChimp* GetInactiveChimp();
    void TrySpawnChimp(float scrollOffset, int screenWidth);
    void CullOffscreen(float scrollOffset);
};
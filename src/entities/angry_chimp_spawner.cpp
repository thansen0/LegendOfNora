#include "angry_chimp_spawner.hpp"

#include "metadata/metadata.hpp"

#include <raylib.h>

AngryChimp* AngryChimpSpawner::GetInactiveChimp()
{
    for (AngryChimp& chimp : chimps)
    {
        if (!chimp.IsActive())
        {
            return &chimp;
        }
    }

    return nullptr;
}

void AngryChimpSpawner::TrySpawnChimp(const float scrollOffset, const int screenWidth)
{
    if (GetRandomValue(1, 100) > SPAWN_CHANCE_PERCENT)
    {
        const float retryGap = static_cast<float>(GetRandomValue(static_cast<int>(MIN_SPAWN_GAP / 2.0f),
                                                               static_cast<int>(MIN_SPAWN_GAP)));
        nextSpawnWorldX = scrollOffset + static_cast<float>(screenWidth) + retryGap;
        return;
    }

    AngryChimp* chimp = GetInactiveChimp();
    if (chimp == nullptr)
    {
        return;
    }

    const float gap = static_cast<float>(GetRandomValue(static_cast<int>(MIN_SPAWN_GAP),
                                                        static_cast<int>(MAX_SPAWN_GAP)));
    const float worldX = scrollOffset + static_cast<float>(screenWidth) + gap;
    const float y = floorY - static_cast<float>(metadata::ANGRY_CHIMP_DISPLAY_SIZE) +
                    static_cast<float>(metadata::ANGRY_CHIMP_GROUND_OFFSET);

    chimp->Spawn(worldX, y);
    nextSpawnWorldX = worldX + MIN_SPAWN_GAP;
}

void AngryChimpSpawner::CullOffscreen(const float scrollOffset)
{
    for (AngryChimp& chimp : chimps)
    {
        if (!chimp.IsActive())
        {
            continue;
        }

        if (chimp.GetScreenX(scrollOffset) < -static_cast<float>(metadata::ANGRY_CHIMP_DISPLAY_SIZE))
        {
            chimp.Despawn();
        }
    }
}

void AngryChimpSpawner::Deactivate()
{
    active = false;

    for (AngryChimp& chimp : chimps)
    {
        chimp.Despawn();
    }
}

void AngryChimpSpawner::Init(const float floorY)
{
    this->floorY = floorY;
    nextSpawnWorldX = 0.0f;
    active = true;

    AngryChimp::LoadAssets();

    for (AngryChimp& chimp : chimps)
    {
        chimp.Despawn();
    }

    nextSpawnWorldX = static_cast<float>(GetScreenWidth()) + MIN_SPAWN_GAP;
}

void AngryChimpSpawner::Update(const float scrollOffset, const int screenWidth)
{
    if (!active)
    {
        return;
    }

    CullOffscreen(scrollOffset);

    if (scrollOffset + static_cast<float>(screenWidth) >= nextSpawnWorldX)
    {
        TrySpawnChimp(scrollOffset, screenWidth);
    }
}

void AngryChimpSpawner::Draw(const float scrollOffset) const
{
    if (!active)
    {
        return;
    }

    for (const AngryChimp& chimp : chimps)
    {
        chimp.Draw(scrollOffset);
    }
}

bool AngryChimpSpawner::CheckCollisions(const float playerX, const float playerY, const float playerSize,
                                        const float scrollOffset)
{
    if (!active)
    {
        return false;
    }

    for (AngryChimp& chimp : chimps)
    {
        if (!chimp.IsActive())
        {
            continue;
        }

        if (chimp.CollidesWith(playerX, playerY, playerSize, scrollOffset))
        {
            return true;
        }
    }

    return false;
}

void AngryChimpSpawner::Cleanup()
{
    for (AngryChimp& chimp : chimps)
    {
        chimp.Despawn();
    }

    AngryChimp::UnloadAssets();
}
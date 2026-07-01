#include "sliding_chimp_spawner.hpp"

#include "metadata/metadata.hpp"

#include <raylib.h>

SlidingChimp* SlidingChimpSpawner::GetInactiveChimp()
{
    for (SlidingChimp& chimp : chimps)
    {
        if (!chimp.IsActive())
        {
            return &chimp;
        }
    }

    return nullptr;
}

void SlidingChimpSpawner::TrySpawnChimp(const float scrollOffset, const int screenWidth)
{
    if (GetRandomValue(1, 100) > SPAWN_CHANCE_PERCENT)
    {
        nextSpawnWorldX = scrollOffset + static_cast<float>(screenWidth) + MIN_SPAWN_GAP * 0.5f;
        return;
    }

    SlidingChimp* chimp = GetInactiveChimp();
    if (chimp == nullptr)
    {
        return;
    }

    const float gap = static_cast<float>(GetRandomValue(static_cast<int>(MIN_SPAWN_GAP),
                                                        static_cast<int>(MAX_SPAWN_GAP)));
    const float anchorWorldX = scrollOffset + static_cast<float>(screenWidth) + gap;
    const bool elevated = GetRandomValue(0, 1) == 1;
    const float y = elevated ? floorY - metadata::ANGRY_CHIMP_DISPLAY_SIZE - 120.0f
                             : floorY - metadata::ANGRY_CHIMP_DISPLAY_SIZE +
                                   static_cast<float>(metadata::ANGRY_CHIMP_GROUND_OFFSET);

    chimp->Spawn(anchorWorldX, y);
    nextSpawnWorldX = anchorWorldX + MIN_SPAWN_GAP;
}

void SlidingChimpSpawner::CullOffscreen(const float scrollOffset)
{
    for (SlidingChimp& chimp : chimps)
    {
        if (!chimp.IsActive())
        {
            continue;
        }

        if (chimp.GetWorldX() - scrollOffset < -static_cast<float>(metadata::ANGRY_CHIMP_DISPLAY_SIZE))
        {
            chimp.Despawn();
        }
    }
}

void SlidingChimpSpawner::Init(const float floorY)
{
    this->floorY = floorY;
    nextSpawnWorldX = 0.0f;
    active = true;

    SlidingChimp::LoadAssets();

    for (SlidingChimp& chimp : chimps)
    {
        chimp.Despawn();
    }

    nextSpawnWorldX = static_cast<float>(GetScreenWidth()) + MIN_SPAWN_GAP;
}

void SlidingChimpSpawner::Update(const float scrollOffset, const int screenWidth, const float dt)
{
    if (!active)
    {
        return;
    }

    CullOffscreen(scrollOffset);

    for (SlidingChimp& chimp : chimps)
    {
        chimp.Update(dt);
    }

    if (scrollOffset + static_cast<float>(screenWidth) >= nextSpawnWorldX)
    {
        TrySpawnChimp(scrollOffset, screenWidth);
    }
}

void SlidingChimpSpawner::Draw(const float scrollOffset) const
{
    if (!active)
    {
        return;
    }

    for (const SlidingChimp& chimp : chimps)
    {
        chimp.Draw(scrollOffset);
    }
}

bool SlidingChimpSpawner::CheckCollisions(const float playerX, const float playerY, const float playerSize,
                                          const float scrollOffset)
{
    if (!active)
    {
        return false;
    }

    for (SlidingChimp& chimp : chimps)
    {
        if (chimp.IsActive() && chimp.CollidesWith(playerX, playerY, playerSize, scrollOffset))
        {
            return true;
        }
    }

    return false;
}

void SlidingChimpSpawner::Deactivate()
{
    active = false;

    for (SlidingChimp& chimp : chimps)
    {
        chimp.Despawn();
    }
}

void SlidingChimpSpawner::Cleanup()
{
    for (SlidingChimp& chimp : chimps)
    {
        chimp.Despawn();
    }

    SlidingChimp::UnloadAssets();
}
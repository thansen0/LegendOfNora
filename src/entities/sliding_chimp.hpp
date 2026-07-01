#pragma once

#include <raylib.h>

// Patrols horizontally around a fixed world anchor.
class SlidingChimp
{
public:
    static void LoadAssets();
    static void UnloadAssets();

    void Spawn(float anchorWorldX, float y);
    void Despawn();
    void Update(float dt);

    [[nodiscard]] bool IsActive() const { return active; }
    [[nodiscard]] float GetWorldX() const;
    void Draw(float scrollOffset) const;
    [[nodiscard]] bool CollidesWith(float playerX, float playerY, float playerSize, float scrollOffset) const;

private:
    static Texture2D texture;
    static bool assetsLoaded;

    float anchorWorldX = 0.0f;
    float y = 0.0f;
    float patrolPhase = 0.0f;
    float patrolRange = 72.0f;
    float patrolSpeed = 2.8f;
    bool active = false;
};
#pragma once

#include "scenes/scene_data.hpp"

#include <raylib.h>

class Scene
{
public:
    void Load(const SceneData& data);
    void Unload();
    void Reset();

    void Update();
    void Draw();

    [[nodiscard]] bool IsComplete() const { return complete; }

private:
    static constexpr int TEXT_BOX_HEIGHT = 140;
    static constexpr int TEXT_PADDING = 16;
    static constexpr int TEXT_FONT_SIZE = 22;
    static constexpr int HINT_FONT_SIZE = 16;

    SceneData current{};
    Texture2D texture{};
    int screenWidth = 0;
    int screenHeight = 0;
    bool complete = false;

    void DrawTextBox() const;
    void DrawSceneImage() const;
};
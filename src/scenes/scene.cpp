#include "scene.hpp"

#include "assets/asset_paths.hpp"

void Scene::Load(const SceneData& data)
{
    Unload();

    current = data;
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    complete = false;

    const char* path = AssetPaths::Resolve(current.imagePath.data());
    Image image = LoadImage(path);
    if (!IsImageValid(image))
    {
        TraceLog(LOG_WARNING, "SCENE: Failed to load image: %s", path);
        return;
    }

    texture = LoadTextureFromImage(image);
    UnloadImage(image);

    if (texture.id == 0)
    {
        TraceLog(LOG_WARNING, "SCENE: Failed to upload texture: %s", path);
    }
}

void Scene::Unload()
{
    if (texture.id != 0)
    {
        UnloadTexture(texture);
        texture = {};
    }

    current = {};
    complete = false;
}

void Scene::Reset()
{
    complete = false;
}

void Scene::Update()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        complete = true;
    }
}

void Scene::DrawSceneImage() const
{
    if (texture.id == 0)
    {
        return;
    }

    const float imageAreaHeight = static_cast<float>(screenHeight - TEXT_BOX_HEIGHT);
    DrawTexturePro(
        texture,
        {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)},
        {0, 0, static_cast<float>(screenWidth), imageAreaHeight},
        {0, 0},
        0.0f,
        WHITE);
}

void Scene::DrawTextBox() const
{
    const int boxY = screenHeight - TEXT_BOX_HEIGHT;

    DrawRectangle(0, boxY, screenWidth, TEXT_BOX_HEIGHT, {24, 24, 32, 255});
    DrawRectangleLines(0, boxY, screenWidth, TEXT_BOX_HEIGHT, {180, 180, 200, 255});

    DrawText(current.promptText.c_str(), TEXT_PADDING, boxY + TEXT_PADDING, TEXT_FONT_SIZE, RAYWHITE);

    const char* hint = "Press SPACE to continue";
    const int hintWidth = MeasureText(hint, HINT_FONT_SIZE);
    DrawText(hint, screenWidth - hintWidth - TEXT_PADDING, boxY + TEXT_BOX_HEIGHT - HINT_FONT_SIZE - TEXT_PADDING,
             HINT_FONT_SIZE, LIGHTGRAY);
}

void Scene::Draw()
{
    BeginDrawing();
    ClearBackground(BLACK);

    DrawSceneImage();
    DrawTextBox();

    EndDrawing();
}
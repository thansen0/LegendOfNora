#include "levels/firstlevel.hpp"
#include "metadata/metadata.hpp"
#include "scenes/scene.hpp"
#include "scenes/scene_data.hpp"
#include "titlescreens/title_screen.hpp"

#include <raylib.h>

#include <vector>

namespace
{
    bool RunTitleScreen(const TitleScreenType type)
    {
        TitleScreen screen;
        screen.Load(type);
        screen.Reset();

        while (!screen.IsConfirmed() && !WindowShouldClose())
        {
            screen.Update();
            screen.Draw();
        }

        const bool confirmed = screen.IsConfirmed();
        screen.Unload();
        return confirmed;
    }

    void RunCutscenes()
    {
        const std::vector<SceneData> allScenes = {
            {metadata::GORILLA_SCENE_YELL,
             "Nora, I'm proud to see you've made it this far. That's quite \nthe accomplishment"},
            {metadata::NORA_SCENE_INOCENT, "... am I done now? I want to get back to reading"},
            {metadata::GORILLA_SCENE_YELL, "NO! I must tell you something, something important"},
            {metadata::GORILLA_SCENE_YELL,
             "Once on a foreign planet, the computer Deep Thought explained \nthe answer to the question, the "
             "Ultimate Question"},
            {metadata::GORILLA_SCENE_YELL, "The answer was 42"},
            {metadata::NORA_SCENE_INOCENT, " ... "},
            {metadata::GORILLA_SCENE_YELL, "I can see you're deeply moved."},
            {metadata::GORILLA_SCENE_YELL,
             "Deep in the forest lives a eternal being, who is said to be \nomniscient and omnipotent. You've "
             "already collected " +
                 to_string(metadata::BOOKS_TO_COMPLETE_LVL1) +
                 " books, \ncollect " + to_string(metadata::BOOKS_TO_COMPLETE_LVL2) +
                 " in total to find this creature."},
            {metadata::NORA_SCENE_INOCENT, "Do I have to? Are you sure you wouldn't rather go?"},
            {metadata::GORILLA_SCENE_YELL,
             "I'm in reverent awe of your humbleness. Go fourth, small primate, \nand discover the Ultimate "
             "Question for us all"},
        };

        Scene scene;
        for (const SceneData& sceneData : allScenes)
        {
            scene.Load(sceneData);
            scene.Reset();

            while (!scene.IsComplete() && !WindowShouldClose())
            {
                scene.Update();
                scene.Draw();
            }

            scene.Unload();

            if (WindowShouldClose())
            {
                break;
            }
        }
    }

    bool RunFirstLevel()
    {
        FirstLevel level;
        level.Init();

        while (!WindowShouldClose() && level.IsRunning())
        {
            level.Update();
            level.Draw();
        }

        const bool playerDead = level.IsPlayerDead();
        const bool levelComplete = level.IsComplete();
        level.Cleanup();

        if (playerDead)
        {
            RunTitleScreen(TitleScreenType::Death);
            return true;
        }

        if (!levelComplete)
        {
            return false;
        }

        RunCutscenes();
        RunTitleScreen(TitleScreenType::Win);
        return true;
    }
}

int main()
{
    InitWindow(800, 600, "Legend of Nora");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (!RunTitleScreen(TitleScreenType::Start))
        {
            break;
        }

        if (!RunFirstLevel())
        {
            break;
        }
    }

    CloseWindow();
}
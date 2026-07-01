#include "levels/firstlevel.hpp"
#include "levels/secondlevel.hpp"
#include "metadata/metadata.hpp"
#include "scenes/scene.hpp"
#include "scenes/scene_data.hpp"
#include "titlescreens/title_screen.hpp"

#include <raylib.h>

#include <vector>

namespace
{
    enum class LevelEnd
    {
        Quit,              // Window closed — exit the game loop.
        ReturnToStartMenu, // Player died — show death screen, then the start menu.
        Advance,           // Level finished — continue to the next stage.
    };

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

    void RunFinalscenes()
    {
        const std::vector<SceneData> allScenes = {
            {metadata::BOOK_GOD_JUDGEMENT, " ...  "},
            {metadata::NORA_SCENE_FATIGUED, "sooo.. who are you?"},
            {metadata::BOOK_GOD_JUDGEMENT, "I am your God, modeled after the greatest book ."},
            {metadata::NORA_SCENE_FATIGUED, "ok well if that's all may I go home now"},
            {metadata::BOOK_GOD_JUDGEMENT, "Absolutely not. You've come all this way, you must have a \nquestion for me."},
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

    // Runs level one and, on success, the post-level cutscene.
    LevelEnd RunFirstLevel(int& booksCollected)
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
        booksCollected = level.GetBooksCollected();
        level.Cleanup();

        if (playerDead)
        {
            return LevelEnd::ReturnToStartMenu;
        }

        if (!levelComplete)
        {
            return LevelEnd::Quit;
        }

        RunCutscenes();
        return LevelEnd::Advance;
    }

    // Runs the escalating second level.
    LevelEnd RunSecondLevel(const int booksFromLevelOne)
    {
        SecondLevel level;
        level.Init(booksFromLevelOne);

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
            return LevelEnd::ReturnToStartMenu;
        }

        if (!levelComplete)
        {
            return LevelEnd::Quit;
        }

        RunTitleScreen(TitleScreenType::Win);
        return LevelEnd::Advance;
    }

    // Shows the death screen, then waits for the player to reach the start menu.
    void HandleDeath()
    {
        RunTitleScreen(TitleScreenType::Death);
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

        int booksCollected = 0;
        const LevelEnd firstEnd = RunFirstLevel(booksCollected);
        if (firstEnd == LevelEnd::Quit)
        {
            break;
        }
        if (firstEnd == LevelEnd::ReturnToStartMenu)
        {
            HandleDeath();
            continue; // Back to the start menu — do not run level 2.
        }

        const LevelEnd secondEnd = RunSecondLevel(booksCollected);
        if (secondEnd == LevelEnd::Quit)
        {
            break;
        }
        if (secondEnd == LevelEnd::ReturnToStartMenu)
        {
            HandleDeath();
            continue; // Back to the start menu.
        }

        RunFinalscenes();
    }

    CloseWindow();
}
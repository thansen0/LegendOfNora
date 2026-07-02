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
        ReturnToStartMenu, // Level 1 death — show death screen, then the start menu.
        RetrySecondLevel,  // Level 2 death — show death screen, then restart level 2.
        Advance,           // Level finished — continue to the next stage.
    };

    bool RunTitleScreen(const TitleScreenType type, const int deathBooksCollected = -1)
    {
        TitleScreen screen;
        screen.Load(type);
        if (type == TitleScreenType::Death && deathBooksCollected >= 0)
        {
            screen.SetBooksCollected(deathBooksCollected);
        }
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
             "Buried in the jungle lives an eternal being, who is said to be \nomniscient and omnipotent. You've "
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
            {metadata::BOOK_GOD_JUDGEMENT, "****  "},
            {metadata::NORA_SCENE_FATIGUED, "sooo.. who are you?"},
            {metadata::BOOK_GOD_JUDGEMENT, "I am Macombo II. I have lived in the Congo jungle since it \nwas called Zaire, and I have understanding beyond \nwhat you could ever concieve."},
            {metadata::NORA_SCENE_FATIGUED, "ok well if that's all may I go home now?"},
            {metadata::BOOK_GOD_JUDGEMENT, "Absolutely not. You've come all this way, you must have a \nquestion for me."},
            {metadata::NORA_SCENE_FATIGUED, "not really.."},
            {metadata::BOOK_GOD_JUDGEMENT, "**Intense judgement**"},
            {metadata::NORA_SCENE_FATIGUED, "i guess the gorilla really wanted me to ask you for the \n\"question\" to his ultimate answer?"},
            {metadata::BOOK_GOD_JUDGEMENT, "Oh you want the Question. The Ultimate Question."},
            {metadata::NORA_SCENE_FATIGUED, "i suppose, whatever,..."},
            {metadata::BOOK_GOD_JUDGEMENT, "Your tenacity to come here demonstrates your worth. I will \ntell you the question."},
            {metadata::BOOK_GOD_JUDGEMENT, "The question, the Ultimate Question is as follows:"},
            {metadata::MOSUBA_GORILLA, "You must pet Mosuba at the Asheboro Zoo"},
            {metadata::NORA_SCENE_FATIGUED, "ohh... nooo, iii'm not..."},
            {metadata::BOOK_GOD_JUDGEMENT, "YOU MUST!!! To know the question and not exercise its meaning \nis a crime in itself"},
            {metadata::NORA_SCENE_FATIGUED, "ummm.."},
            {metadata::BOOK_GOD_JUDGEMENT, "Do not listen to the zookeepers, they tell lies. Mosuba gave me \nhis word."},
            {metadata::BOOK_GOD_JUDGEMENT, "You must attend the Asheboro Zoo and pet Mosuba exactly 42 times.\nThat is our ultimate purpose. The meaning of life."},
            {metadata::NORA_SCENE_THINKING, "hmm I guess you're right."},
            {metadata::NORA_SCENE_AGREEING, "I, Game Player, promise to pet Mosuba next time I see him.\n\nPress (SPACE) to contractually agree."},

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
    LevelEnd RunSecondLevel(const int booksFromLevelOne, int& booksCollected)
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
        booksCollected = level.GetBooksCollected();
        level.Cleanup();

        if (playerDead)
        {
            return LevelEnd::RetrySecondLevel;
        }

        if (!levelComplete)
        {
            return LevelEnd::Quit;
        }

        return LevelEnd::Advance;
    }

    // Shows the death screen before returning to the start menu or retrying level 2.
    void HandleDeath(const int booksCollected)
    {
        RunTitleScreen(TitleScreenType::Death, booksCollected);
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
            HandleDeath(booksCollected);
            continue; // Back to the start menu — do not run level 2.
        }

        const int booksFromLevelOne = booksCollected;
        LevelEnd secondEnd = LevelEnd::RetrySecondLevel;

        while (!WindowShouldClose())
        {
            secondEnd = RunSecondLevel(booksFromLevelOne, booksCollected);
            if (secondEnd == LevelEnd::Quit)
            {
                break;
            }
            if (secondEnd == LevelEnd::RetrySecondLevel)
            {
                HandleDeath(booksCollected);
                continue;
            }
            break;
        }

        if (secondEnd == LevelEnd::Quit || WindowShouldClose())
        {
            break;
        }

        RunFinalscenes();
        RunTitleScreen(TitleScreenType::Win);
    }

    CloseWindow();
}

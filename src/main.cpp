#include "assets/level_music.hpp"
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

    bool RunTitleScreen(const TitleScreenType type, const int deathBooksCollected = -1,
                        LevelMusic* music = nullptr)
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
            if (music != nullptr)
            {
                music->Update();
            }

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
            {metadata::NORA_SCENE_INNOCENT, "... am I done now? I want to get back to reading"},
            {metadata::GORILLA_SCENE_YELL, "NO! I must tell you something, something important"},
            {metadata::GORILLA_SCENE_YELL,
             "Once on a foreign planet, the computer Deep Thought explained \nthe answer to the question, the "
             "Ultimate Question"},
            {metadata::GORILLA_SCENE_YELL, "The answer was 42"},
            {metadata::NORA_SCENE_INNOCENT, " ... "},
            {metadata::GORILLA_SCENE_YELL, "I can see you're deeply moved."},
            {metadata::GORILLA_SCENE_YELL,
             "Buried in the jungle lives an eternal being, who is said to be \nomniscient and omnipotent. You've "
             "already collected " +
                 to_string(metadata::BOOKS_TO_COMPLETE_LVL1) +
                 " books, \ncollect " + to_string(metadata::BOOKS_TO_COMPLETE_LVL2) +
                 " in total to find this creature."},
            {metadata::NORA_SCENE_INNOCENT, "Do I have to? Are you sure you wouldn't rather go?"},
            {metadata::GORILLA_SCENE_YELL,
             "I'm in reverent awe of your humbleness. Go forth, small primate, \nand discover the Ultimate "
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
            {metadata::BOOK_GOD_JUDGMENT, "****  "},
            {metadata::NORA_SCENE_FATIGUED, "sooo.. who are you?"},
            {metadata::BOOK_GOD_JUDGMENT, "I am Macombo II. I have lived in the Congolese jungle since it \nwas called Zaire, and I have understanding beyond \nwhat you could ever conceive."},
            {metadata::NORA_SCENE_FATIGUED, "ok well if that's all may I go home now?"},
            {metadata::BOOK_GOD_JUDGMENT, "Absolutely not. You've come all this way, you must have a \nquestion for me."},
            {metadata::NORA_SCENE_FATIGUED, "not really.."},
            {metadata::BOOK_GOD_JUDGMENT, "**Intense judgment**"},
            {metadata::NORA_SCENE_FATIGUED, "i guess the gorilla really wanted me to ask you for the \n\"question\" to his ultimate answer?"},
            {metadata::BOOK_GOD_JUDGMENT, "Oh you want the Question. The Ultimate Question."},
            {metadata::NORA_SCENE_FATIGUED, "i suppose, whatever,..."},
            {metadata::BOOK_GOD_JUDGMENT, "Your tenacity to come here demonstrates your worth. I will \ntell you the question."},
            {metadata::BOOK_GOD_JUDGMENT, "The question, the Ultimate Question is as follows:"},
            {metadata::MOSUBA_GORILLA, "You must pet Mosuba at the Asheboro Zoo"},
            {metadata::NORA_SCENE_FATIGUED, "ohh... nooo, iii'm not..."},
            {metadata::BOOK_GOD_JUDGMENT, "YOU MUST!!! To know the question and not exercise its meaning \nis a crime in itself"},
            {metadata::NORA_SCENE_FATIGUED, "ummm.."},
            {metadata::BOOK_GOD_JUDGMENT, "Do not listen to the zookeepers, they tell lies. Mosuba gave me \nhis word."},
            {metadata::BOOK_GOD_JUDGMENT, "You must attend the Asheboro Zoo and pet Mosuba exactly 42 times.\nThat is our ultimate purpose. The meaning of life."},
            {metadata::NORA_SCENE_THINKING, "hmm I guess you're right."},
            {metadata::NORA_SCENE_AGREEING, "I, the player of this game, agree to pet Mosuba when I see him.\n\nPress (SPACE) to contractually agree."},

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
    LevelEnd RunFirstLevel(int& booksCollected, LevelMusic& music)
    {
        FirstLevel level;
        level.Init(&music);

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
    LevelEnd RunSecondLevel(const int booksFromLevelOne, int& booksCollected, LevelMusic& music)
    {
        SecondLevel level;
        level.Init(booksFromLevelOne, &music);

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

    // Level 1 death keeps music playing at reduced volume, then restores it for the retry.
    void HandleLevel1Death(const int booksCollected, LevelMusic& music)
    {
        if (music.IsLoaded())
        {
            music.SetVolume(0.5f);
        }

        RunTitleScreen(TitleScreenType::Death, booksCollected, &music);

        if (music.IsLoaded())
        {
            music.SetVolume(1.0f);
        }
    }

    // Level 2 death keeps music playing at reduced volume, then restores it for the retry.
    void HandleLevel2Death(const int booksCollected, LevelMusic& music)
    {
        if (music.IsLoaded())
        {
            music.SetVolume(0.5f);
        }

        RunTitleScreen(TitleScreenType::Death, booksCollected, &music);

        if (music.IsLoaded())
        {
            music.SetVolume(1.0f);
        }
    }
}

int main()
{
    InitWindow(800, 600, "Legend of Nora");
    SetTargetFPS(60);

    LevelMusic level1Music;
    bool level1MusicActive = false;

    while (!WindowShouldClose())
    {
        LevelMusic* startMenuMusic = level1MusicActive ? &level1Music : nullptr;
        if (!RunTitleScreen(TitleScreenType::Start, -1, startMenuMusic))
        {
            if (level1MusicActive)
            {
                level1Music.Unload();
            }
            break;
        }

        if (!level1MusicActive)
        {
            level1Music.Load(metadata::LEVEL1_MUSIC.data());
            level1Music.Play();
            level1MusicActive = true;
        }

        int booksCollected = 0;
        const LevelEnd firstEnd = RunFirstLevel(booksCollected, level1Music);
        if (firstEnd == LevelEnd::Quit)
        {
            level1Music.Unload();
            break;
        }
        if (firstEnd == LevelEnd::ReturnToStartMenu)
        {
            HandleLevel1Death(booksCollected, level1Music);
            continue; // Back to the start menu — do not run level 2.
        }

        level1Music.Unload();
        level1MusicActive = false;

        const int booksFromLevelOne = booksCollected;
        LevelMusic level2Music;
        level2Music.Load(metadata::LEVEL2_MUSIC.data());
        level2Music.Play();

        LevelEnd secondEnd = LevelEnd::RetrySecondLevel;

        while (!WindowShouldClose())
        {
            secondEnd = RunSecondLevel(booksFromLevelOne, booksCollected, level2Music);
            if (secondEnd == LevelEnd::Quit)
            {
                break;
            }
            if (secondEnd == LevelEnd::RetrySecondLevel)
            {
                HandleLevel2Death(booksCollected, level2Music);
                continue;
            }
            break;
        }

        level2Music.Unload();

        if (secondEnd == LevelEnd::Quit || WindowShouldClose())
        {
            break;
        }

        RunFinalscenes();
        RunTitleScreen(TitleScreenType::Win);
    }

    CloseWindow();
}

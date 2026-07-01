#pragma once

#include <raylib.h>

enum class TitleScreenType
{
    Start,
    Death,
    Win,
};

class TitleScreen
{
public:
    void Load(TitleScreenType type);
    void SetBooksCollected(int count);
    void Unload();
    void Reset();

    void Update();
    void Draw();

    [[nodiscard]] bool IsConfirmed() const { return confirmed; }

private:
    static constexpr int BUTTON_WIDTH = 260;
    static constexpr int BUTTON_HEIGHT = 52;
    static constexpr int BUTTON_FONT_SIZE = 24;
    static constexpr int TITLE_FONT_SIZE = 56;

    TitleScreenType screenType = TitleScreenType::Start;
    Texture2D imageTexture{};
    Rectangle buttonBounds{};
    const char* buttonText = "";
    bool confirmed = false;
    bool hovered = false;
    int screenWidth = 0;
    int screenHeight = 0;
    int booksCollected = -1;

    void UpdateButtonBounds();
    void LoadImageBackground(const char* relativePath);
    void DrawBackground() const;
    void DrawHeadline() const;
    void DrawDeathStats() const;
    void DrawButton() const;
    bool WasButtonActivated() const;

    [[nodiscard]] const char* GetHeadlineText() const;
};
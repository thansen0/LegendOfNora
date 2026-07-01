#include "title_screen.hpp"

#include "assets/asset_paths.hpp"
#include "metadata/metadata.hpp"

void TitleScreen::UpdateButtonBounds()
{
    buttonBounds = {
        static_cast<float>(screenWidth - BUTTON_WIDTH) / 2.0f,
        static_cast<float>(screenHeight) - static_cast<float>(BUTTON_HEIGHT) - 48.0f,
        static_cast<float>(BUTTON_WIDTH),
        static_cast<float>(BUTTON_HEIGHT),
    };
}

void TitleScreen::LoadImageBackground(const char* relativePath)
{
    const char* path = AssetPaths::Resolve(relativePath);
    Image image = LoadImage(path);
    if (!IsImageValid(image))
    {
        TraceLog(LOG_WARNING, "TITLESCREEN: Failed to load image: %s", path);
        return;
    }

    imageTexture = LoadTextureFromImage(image);
    UnloadImage(image);
}

void TitleScreen::Load(const TitleScreenType type)
{
    Unload();

    screenType = type;
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    confirmed = false;
    hovered = false;
    booksCollected = -1;
    UpdateButtonBounds();

    switch (type)
    {
    case TitleScreenType::Start:
        buttonText = "Start";
        LoadImageBackground(metadata::TITLE_START_IMAGE.data());
        break;

    case TitleScreenType::Death:
        buttonText = "Back to Start";
        LoadImageBackground(metadata::TITLE_DEATH_IMAGE.data());
        break;

    case TitleScreenType::Win:
        buttonText = "Back to Start";
        LoadImageBackground(metadata::TITLE_WIN_IMAGE.data());
        break;
    }
}

void TitleScreen::SetBooksCollected(const int count)
{
    booksCollected = count;
}

void TitleScreen::Unload()
{
    if (imageTexture.id != 0)
    {
        UnloadTexture(imageTexture);
        imageTexture = {};
    }

    confirmed = false;
    hovered = false;
    buttonText = "";
    booksCollected = -1;
}

void TitleScreen::Reset()
{
    confirmed = false;
    hovered = false;
}

void TitleScreen::DrawBackground() const
{
    if (imageTexture.id != 0)
    {
        DrawTexturePro(
            imageTexture,
            {0, 0, static_cast<float>(imageTexture.width), static_cast<float>(imageTexture.height)},
            {0, 0, static_cast<float>(screenWidth), static_cast<float>(screenHeight)},
            {0, 0},
            0.0f,
            WHITE);
        return;
    }

    DrawRectangle(0, 0, screenWidth, screenHeight, {20, 20, 30, 255});
}

const char* TitleScreen::GetHeadlineText() const
{
    switch (screenType)
    {
    case TitleScreenType::Death:
        return metadata::TITLE_DEATH_MESSAGE.data();
    case TitleScreenType::Win:
        return metadata::TITLE_WIN_MESSAGE.data();
    case TitleScreenType::Start:
    default:
        return metadata::GAME_TITLE.data();
    }
}

void TitleScreen::DrawHeadline() const
{
    const char* headline = GetHeadlineText();
    const int headlineWidth = MeasureText(headline, TITLE_FONT_SIZE);
    const int headlineX = (screenWidth - headlineWidth) / 2;
    const int headlineY = screenHeight / 5;

    DrawText(headline, headlineX + 2, headlineY + 2, TITLE_FONT_SIZE, {0, 0, 0, 180});
    DrawText(headline, headlineX, headlineY, TITLE_FONT_SIZE, RAYWHITE);
}

void TitleScreen::DrawDeathStats() const
{
    if (screenType != TitleScreenType::Death || booksCollected < 0)
    {
        return;
    }

    const char* statsText = TextFormat("You collected %d books", booksCollected);
    constexpr int statsFontSize = 28;
    const int statsWidth = MeasureText(statsText, statsFontSize);
    const int statsX = (screenWidth - statsWidth) / 2;
    const int statsY = screenHeight / 5 + TITLE_FONT_SIZE + 24;

    DrawText(statsText, statsX + 2, statsY + 2, statsFontSize, {0, 0, 0, 180});
    DrawText(statsText, statsX, statsY, statsFontSize, LIGHTGRAY);
}

void TitleScreen::DrawButton() const
{
    const Color fillColor = hovered ? Color{70, 130, 180, 230} : Color{40, 40, 55, 220};
    const Color borderColor = hovered ? SKYBLUE : RAYWHITE;

    DrawRectangleRec(buttonBounds, fillColor);
    DrawRectangleLinesEx(buttonBounds, 2.0f, borderColor);

    const int textWidth = MeasureText(buttonText, BUTTON_FONT_SIZE);
    const int textX = static_cast<int>(buttonBounds.x + (buttonBounds.width - textWidth) / 2.0f);
    const int textY = static_cast<int>(buttonBounds.y + (buttonBounds.height - BUTTON_FONT_SIZE) / 2.0f);
    DrawText(buttonText, textX, textY, BUTTON_FONT_SIZE, RAYWHITE);
}

bool TitleScreen::WasButtonActivated() const
{
    if (IsKeyPressed(KEY_SPACE))
    {
        return true;
    }

    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
           CheckCollisionPointRec(GetMousePosition(), buttonBounds);
}

void TitleScreen::Update()
{
    hovered = CheckCollisionPointRec(GetMousePosition(), buttonBounds);

    if (WasButtonActivated())
    {
        confirmed = true;
    }
}

void TitleScreen::Draw()
{
    BeginDrawing();

    DrawBackground();
    DrawHeadline();
    DrawDeathStats();
    DrawButton();

    EndDrawing();
}
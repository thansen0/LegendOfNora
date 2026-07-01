#pragma once

#include <string_view>

using namespace std;

namespace metadata {
    const string_view VIDEO_BACKGROUND_PEACEFUL{"assets/background/animated-background.mp4"};
    const string_view VIDEO_BACKGROUND_OMINOUS{"assets/background/animated-background.mp4"};

    const string_view STATIC_BACKGROUND_PEACEFUL{"assets/background/congobackground.jpg"};
    const string_view STATIC_BACKGROUND_OMINOUS{"assets/background/ominous-congobackground.jpg"};

    const string_view BOOK_TEXTURE{"assets/items/book_64.png"};
    const string_view GORILLA_TEXTURE{"assets/entities/gorilla_512.png"};
    const string_view ANGRY_CHIMP_TEXTURE{"assets/entities/angry-chimp.png"};
    const string_view NORA_TEXTURE{"assets/entities/nora-motorcycle.png"};

    const string_view GORILLA_SCENE_YELL{"assets/scenes/gorilla-background.jpg"};
    const string_view NORA_SCENE_INOCENT{"assets/scenes/nora-background.jpg"};
    const string_view BOOK_GOD_JUDGEMENT{"assets/scenes/white-gorilla-stare.jpg"};
    const string_view NORA_SCENE_FATIGUED{"assets/scenes/nora-background-ominous.jpg"};

    const string_view TITLE_START_IMAGE{"assets/background/congobackground.jpg"};
    const string_view TITLE_DEATH_IMAGE{"assets/titlescreens/you-lost.jpg"};
    const string_view TITLE_WIN_IMAGE{"assets/titlescreens/you-win.jpg"};
    const string_view GAME_TITLE{"The Legent of Nora"};
    const string_view TITLE_DEATH_MESSAGE{"Sorry you died"};
    const string_view TITLE_WIN_MESSAGE{"Congratulations, you won!"};

    const int PLAYER_SIZE{40};
    const int NORA_DISPLAY_HEIGHT{96};
    const int BOOK_SIZE{64};
    const int GORILLA_SIZE{512};
    const int ANGRY_CHIMP_DISPLAY_SIZE{88};
    const int ANGRY_CHIMP_GROUND_OFFSET{28};
    const int ANGRY_CHIMP_HITBOX_INSET{12};

    const string_view GROUND_SHEET{"assets/ground/main.png"};

    const int TILE_SIZE{64};
    const int FLOATING_BRIDGE_TILE_COLUMN{7};

    const float LEVEL2_BASE_SPEED{200.0f};
    const float LEVEL2_SPEED_RAMP_SCALE{10000.0f};
    const float LEVEL2_OBSTACLE_CUTOFF_MULTIPLIER{4.0f};

    const int BOOKS_TO_COMPLETE_LVL1{5}; // reduced from {42}; for faster testing
    const int BOOKS_TO_COMPLETE_LVL2{534};
}

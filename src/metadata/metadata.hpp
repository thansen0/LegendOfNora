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

    const int PLAYER_SIZE{40};
    const int BOOK_SIZE{64};
    const int GORILLA_SIZE{512};

    const int BOOKS_TO_COMPLETE_LVL1{5}; // reduced from {42}; for faster testing
    const int BOOKS_TO_COMPLETE_LVL2{534};
}

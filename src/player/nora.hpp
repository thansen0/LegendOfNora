#pragma once

class Nora {
private:
    int PLAYER_SIZE;
    int books_collected{0};

public:
    Nora();

    void Init();
    void Draw(int playerX, int playerY);
    void Cleanup();
};
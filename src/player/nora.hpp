#pragma once

class Nora
{
public:
    Nora();

    void Init();
    void Draw(int playerX, int playerY);
    void Cleanup();

    void CollectBook();
    [[nodiscard]] int GetBooksCollected() const { return booksCollected; }

private:
    int playerSize;
    int booksCollected{0};
};
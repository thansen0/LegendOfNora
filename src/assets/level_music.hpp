#pragma once

#include <raylib.h>

// Optional looping music for a level. Does nothing if the track file is missing.
class LevelMusic
{
public:
    void Load(const char* relativePath);
    void Play();
    void SetVolume(float volume);
    void BeginFadeOut(float durationSeconds);
    void Update();
    void Stop();
    void Unload();

    [[nodiscard]] bool IsLoaded() const { return loaded; }
    [[nodiscard]] bool IsFadingOut() const { return fading; }
    [[nodiscard]] bool HasFadedOut() const { return fadedOut; }

private:
    Music music{};
    bool loaded = false;
    bool audioDeviceInitialized = false;
    bool fading = false;
    bool fadedOut = false;
    float baseVolume = 1.0f;
    float fadeStartVolume = 1.0f;
    float fadeDuration = 0.0f;
    float fadeTimer = 0.0f;
};
#include "level_music.hpp"

#include "assets/asset_paths.hpp"

void LevelMusic::Load(const char* relativePath)
{
    Unload();

    const char* path = AssetPaths::Resolve(relativePath);
    if (!FileExists(path))
    {
        TraceLog(LOG_INFO, "LEVEL_MUSIC: Track not found, skipping: %s", path);
        return;
    }

    if (!audioDeviceInitialized)
    {
        InitAudioDevice();
        audioDeviceInitialized = IsAudioDeviceReady();
        if (!audioDeviceInitialized)
        {
            TraceLog(LOG_WARNING, "LEVEL_MUSIC: Audio device failed to initialize");
            return;
        }
    }

    music = LoadMusicStream(path);
    if (!IsMusicValid(music))
    {
        TraceLog(LOG_WARNING, "LEVEL_MUSIC: Failed to load music: %s", path);
        music = {};
        return;
    }

    music.looping = true;
    fading = false;
    fadedOut = false;
    fadeTimer = 0.0f;
    loaded = true;
}

void LevelMusic::Play()
{
    if (!loaded)
    {
        return;
    }

    baseVolume = 1.0f;
    SetMusicVolume(music, baseVolume);
    fading = false;
    fadedOut = false;
    fadeTimer = 0.0f;
    PlayMusicStream(music);
}

void LevelMusic::SetVolume(const float volume)
{
    if (!loaded || fadedOut)
    {
        return;
    }

    baseVolume = volume;
    if (!fading)
    {
        SetMusicVolume(music, baseVolume);
    }
}

void LevelMusic::BeginFadeOut(const float durationSeconds)
{
    if (!loaded || fading || fadedOut)
    {
        return;
    }

    fading = true;
    fadedOut = false;
    fadeStartVolume = baseVolume;
    fadeDuration = durationSeconds;
    fadeTimer = 0.0f;
}

void LevelMusic::Update()
{
    if (!loaded)
    {
        return;
    }

    if (fading)
    {
        fadeTimer += GetFrameTime();
        const float progress = fadeTimer / fadeDuration;

        if (progress >= 1.0f)
        {
            SetMusicVolume(music, 0.0f);
            StopMusicStream(music);
            fading = false;
            fadedOut = true;
            return;
        }

        SetMusicVolume(music, fadeStartVolume * (1.0f - progress));
    }

    if (!fadedOut)
    {
        UpdateMusicStream(music);
    }
}

void LevelMusic::Stop()
{
    if (!loaded)
    {
        return;
    }

    StopMusicStream(music);
}

void LevelMusic::Unload()
{
    if (loaded)
    {
        StopMusicStream(music);
        UnloadMusicStream(music);
        music = {};
        loaded = false;
    }

    if (audioDeviceInitialized)
    {
        CloseAudioDevice();
        audioDeviceInitialized = false;
    }
}
# The Legend of Nora

This is a mini-game written in C++ and [raylib](https://github.com/raysan5/raylib). It follows our protagonist, Nora, who rides a motorcycle through the Congo jungle collecting books. To survive she must avoid chimps while making other ape friends along the way.

If you haven't read [*The Hitchhiker's Guide to the Galaxy*](https://www.goodreads.com/book/show/11.The_Hitchhiker_s_Guide_to_the_Galaxy), then the references won't make any sense and I apologize. Additionally if you're not Nora then some of the references won't make sense and I apologize.

## Building locally

The project requires a C++23 compiler, CMake 3.20+, and ~~SDL~~GLFW.

Clone with submodules:

```bash
git clone --recurse-submodules https://github.com/thansen0/LegendOfNora.git
cd LegendOfNora
```

If you already cloned without submodules:

```bash
git submodule update --init --recursive
```

### Linux (apt pkg manager)

Install build tools and dependencies with `apt`:

```bash
sudo apt update
sudo apt install build-essential cmake libsdl2-dev libgl1-mesa-dev
```

In the event you want to use SDL3

```bash
sudo apt install build-essential cmake libsdl3-dev libgl1-mesa-dev
```

Configure, build, and run:

```bash
mkdir build && cd build/
cmake ../
make -j
./LegendOfNora
```

Assets are copied into `build/assets` automatically during the build.

### macOS

Install CMake with Homebrew:

```bash
brew install cmake
```

Configure, build, and run:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j"$(sysctl -n hw.ncpu)"
./build/LegendOfNora
```

Assets are copied into `build/assets` automatically during the build.

## macOS Apple Silicon builds (GitHub Actions)

Pushing a **git tag** triggers `.github/workflows/build-macos-arm64.yml`, which builds a native **arm64** binary on GitHub's macOS runners. That produces builds for Apple Silicon Macs (M1, M2, M3, M4, and later). Admittedly I don't have a mac and so these instructions were written by AI.

### Files required in the repository

The workflow uses files already in this repo. You do not need to add secrets or extra config beyond committing:

| Path | Purpose |
|------|---------|
| `.github/workflows/build-macos-arm64.yml` | The workflow definition |
| `CMakeLists.txt` | Top-level CMake project |
| `src/` | Game source code |
| `assets/` | Images, sprites, and other game data |
| `extern/raylib/` | raylib git submodule |
| `.gitmodules` | Submodule configuration |

### Download the build

1. Open the repository on GitHub → **Actions** → **Build macOS (Apple Silicon)** → select the run for your tag.
2. Download **LegendOfNora-&lt;tag&gt;-macos-arm64** from **Artifacts** (available for a limited time), **or**
3. Open **Releases**, open the release for your tag, and download the same zip from release assets.

### Run on your Mac

```bash
unzip LegendOfNora-v0.1.0-macos-arm64.zip
cd LegendOfNora-v0.1.0-macos-arm64
# Note: if you want to add music you will have to at this stage, since my
# music was copyrighted and I didn't want to upload it to gitlab.
chmod +x LegendOfNora
./LegendOfNora
```

The zip includes `assets/` and a self-contained binary (raylib uses embedded GLFW), so Homebrew is not required on the target machine.

If macOS blocks the app as unidentified, either right-click → **Open**, or remove the quarantine flag:

```bash
xattr -cr LegendOfNora-v0.1.0-macos-arm64
```

### Adding (back) music

Most music is copyrighted, and is not included in the repo. When I distribute the game I usually include Mochat Doma songs (I liked На дне and Судно), however they're not needed and you may include anything or nothing.

Songs must be added to `assets/music/`, and must be named `level-1-copyright.ogg` and `level-2-copyright.ogg`.

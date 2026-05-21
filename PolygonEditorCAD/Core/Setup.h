#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>
#include <random>
#include <functional> //for event based programing


//project needs C++ 17 in order to not have header variable conflicts
//pallete for colors https://coolors.co

using namespace std;

namespace PALETTE {
    constexpr Color CAD_CYAN = { 80, 125, 188, 122 };
    constexpr Color MIDNIGHT_VIOLET = { 4, 8, 15, 122 };
}

namespace ENGINE {//global variables no linker error

    inline float GRID_SPACING = 50.0f;
    inline bool CENTER_AREAS = true;
    inline bool BUILD_DEBUG_TOOLS = true;
    inline bool SHOW_POINT_COORDINATES = true;

    // Initialize to 0 so Raylib doesn't throw a fit before loading
    inline Font MAIN_FONT = { 0 };

    // Functions inside headers should also be inline
    inline void initialize() {
        MAIN_FONT = LoadFontEx("Assets/InterVariable.ttf", 64, 0, 0);

        if (MAIN_FONT.texture.id == 0) {
            std::cout << "Font not loaded!\n";
        }
    }

    inline void shutdown() {
        UnloadFont(MAIN_FONT);
    }
}

constexpr int windowWidth = 1920;
constexpr int windowHeight = 1080;

constexpr Rectangle NULLREC = { 0.0f,0.0f,0.0f,0.0f };

//wrappers
inline void DrawUIText(const char* text,float x,float y,float size,Color color)
{
    DrawTextEx(ENGINE::MAIN_FONT, text, Vector2{x, y}, size, 1.0f, color);
}
inline void DrawUITextV(const char* text, Vector2 pos, float size, Color color)
{
    DrawTextEx(ENGINE::MAIN_FONT, text, pos, size, 1.0f, color);
}
//Helpers.h
namespace {
    static mt19937_64 _gen{ random_device{}() };
}

//Global Functions

// Returns 0.0f to 1.0f
inline float randf() {
    return uniform_real_distribution<float>(0.0f, 1.0f)(_gen);
}

// Returns a value between min and max (inclusive)
inline float randf_range(float min, float max) {
    return uniform_real_distribution<float>(min, max)(_gen);
}

// Returns an integer between min and max
inline int randi_range(int min, int max) {
    return uniform_int_distribution<int>(min, max)(_gen);
}

// Returns -1.0f or 1.0f
inline float randsign() {
    return (randf() < 0.5f) ? -1.0f : 1.0f;
}
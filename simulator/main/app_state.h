#pragma once
#include "SDL3/SDL.h"

struct AppState
{
    SDL_Window* window{nullptr};
    SDL_Renderer* renderer{nullptr};

    bool show_debug_menu = false;
    float led_color[4] = {1.0f, 0.0f, 1.0f, 1.0f};
};

#pragma once

#include <vector>

#include "SDL3/SDL.h"

#include "../app_state.h"
#include "GC/led_matrix.h"
#include "GC/wled.h"


namespace sim
{
    class LedMatrix final : public gc::LedMatrix
    {
    public:
        LedMatrix(const uint8_t width, const uint8_t height) : width(width), height(height)
        {
            leds.reserve(width * height);
        }

        void draw(const AppState* appState) const;
        void setPixelColor(std::uint16_t n, std::uint32_t c) override;
        void clear() override;
        void show() override;

    private:
        static void drawCircle(const AppState* appState, const SDL_FPoint* position);

        uint8_t width;
        uint8_t height;
        std::vector<gc::WLED> leds;
    };
}

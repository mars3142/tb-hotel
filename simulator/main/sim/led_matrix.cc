#include "led_matrix.h"

#include <algorithm>
#include <cmath>

#include "SDL3/SDL.h"

#include "../constants.h"

namespace sim
{
    Uint8 convert(float value)
    {
        value = std::clamp(value, 0.0f, 1.0f);
        float scaled_value = value * 255.0f;
        scaled_value = std::clamp(scaled_value, 0.0f, 255.0f);
        return static_cast<Uint8>(std::round(scaled_value));
    }

    void LedMatrix::drawCircle(const AppState* appState, const SDL_FPoint* position)
    {
        // const auto r = static_cast<Uint8>((static_cast<float>(SDL_GetTicks() % 3000) / 3000.0f) * 255.0f);
        // SDL_SetRenderDrawColor(appState->renderer, r, 0, 0, 255);
        SDL_SetRenderDrawColor(appState->renderer, convert(appState->led_color[0]), convert(appState->led_color[1]),
                               convert(appState->led_color[2]), convert(appState->led_color[3]));

        auto topPadding = 0.0f;
        if (appState->show_debug_menu)
        {
            topPadding = IMGUI_MENU_HEIGHT;
        }
        const auto rect = SDL_FRect{
            position->x,
            position->y + topPadding,
            WLED_SIZE - WLED_SPACING,
            WLED_SIZE - WLED_SPACING
        };
        SDL_RenderFillRect(appState->renderer, &rect);
    }

    void LedMatrix::draw(const AppState* appState) const
    {
        for (auto h = 0; h < height; ++h)
        {
            for (auto w = 0; w < width; ++w)
            {
                auto position = SDL_FPoint(
                    static_cast<float>(w) * WLED_SIZE + WLED_SPACING,
                    static_cast<float>(h) * WLED_SIZE + WLED_SPACING
                );
                drawCircle(appState, &position);
            }
        }
    }

    void LedMatrix::setPixelColor(std::uint16_t n, std::uint32_t c)
    {
    }

    void LedMatrix::clear()
    {
    }

    void LedMatrix::show()
    {
    }
}

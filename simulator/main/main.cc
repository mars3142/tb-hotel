#include <imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "game/game.h"
#include "app_state.h"
#include "constants.h"
#include "sim/led_matrix.h"

sim::LedMatrix led_matrix(LED_COUNT_WIDTH, LED_COUNT_HEIGHT);
const gc::Game game(led_matrix);

bool show_unhandled_events = false;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("GCXXXX", "1.0", "dev.mars3142.gcxxxx");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) == false)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL!\n%s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    const auto appState = new AppState();
    *appstate = appState;

    constexpr uint32_t window_flags = SDL_WINDOW_HIDDEN;
    appState->window = SDL_CreateWindow("GCXXXX - TB-Hotel", WINDOW_WIDTH, WINDOW_HEIGHT, window_flags);
    if (appState->window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window!\n%s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    appState->renderer = SDL_CreateRenderer(appState->window, nullptr);
    if (appState->renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer!\n%s", SDL_GetError());
    }

    SDL_SetRenderVSync(appState->renderer, 1);
    SDL_SetWindowPosition(appState->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(appState->window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(appState->window, appState->renderer);
    ImGui_ImplSDLRenderer3_Init(appState->renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    const auto appState = static_cast<AppState*>(appstate);
    if (appState->show_debug_menu)
    {
        ImGui_ImplSDL3_ProcessEvent(event);
    }

    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        // end the program, reporting success to the OS
        return SDL_APP_SUCCESS;

    case SDL_EVENT_KEY_DOWN:

        if (event->key.key == SDLK_ESCAPE)
        {
            return SDL_APP_SUCCESS;
        }
        break;

    case SDL_EVENT_KEY_UP:
        if (event->key.key == SDLK_LSHIFT)
        {
            appState->show_debug_menu = !appState->show_debug_menu;
        }
        break;

    default:
        if (show_unhandled_events)
        {
            SDL_Log("Unused event: %d", event->type);
        }
        break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    const auto appState = static_cast<AppState*>(appstate);
    SDL_SetRenderDrawColor(appState->renderer, 0, 0, 0, 255);
    SDL_RenderClear(appState->renderer);

    led_matrix.draw(appState);
    led_matrix.setPixelColor(0, 0);

    SDL_SetWindowSize(appState->window, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (appState->show_debug_menu)
    {
        SDL_SetWindowSize(appState->window, WINDOW_WIDTH, WINDOW_HEIGHT + IMGUI_MENU_HEIGHT);
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Information"))
            {
                ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
                ImGui::Separator();
                ImGui::ColorEdit4("LED", appState->led_color);
                ImGui::Checkbox("Show unhandled Events", &show_unhandled_events);
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), appState->renderer);
    }

    SDL_RenderPresent(appState->renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    const auto appState = static_cast<AppState*>(appstate);

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    free(appState);
}

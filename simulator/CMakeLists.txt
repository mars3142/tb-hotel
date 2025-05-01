cmake_minimum_required(VERSION 3.30)

set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)
set(BUILD_VERSION 1)

project(
        gcxxxx
        LANGUAGES CXX C
        VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${BUILD_VERSION}"
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS YES)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

include(FetchContent)
# define a function for adding git dependencies
function(include_dependency libName gitURL gitTag)
    # setup the declare
    FetchContent_Declare(${libName}
            GIT_REPOSITORY ${gitURL}
            GIT_TAG ${gitTag}
            GIT_SHALLOW TRUE
            GIT_PROGRESS FALSE
    )

    FetchContent_MakeAvailable(${libName})
endfunction()

# add SDL3 support
find_package(SDL3 QUIET)
if (NOT SDL3_FOUND)
    message(STATUS "Getting SDL3 from Github")
    include_dependency(SDL3 https://github.com/libsdl-org/SDL release-3.2.10)
else ()
    message(STATUS "Using local SDL3")
endif ()

# add SDL3_image support
find_package(SDL_image QUIET)
if (NOT SDL_image_FOUND)
    message(STATUS "Getting SDL3_image from Github")
    include_dependency(SDL_image https://github.com/libsdl-org/SDL_image release-3.2.4)
else ()
    message(STATUS "Using local SDL3_image")
endif ()

# add SDL3_ttf support
find_package(SDL_ttf QUIET)
if (NOT SDL_ttf_FOUND)
    message(STATUS "Getting SDL3_ttf from Github")
    include_dependency(SDL_ttf https://github.com/libsdl-org/SDL_ttf release-3.2.0)
else ()
    message(STATUS "Using local SDL3_ttf")
endif ()

# add ImGui support
find_package(ImGui REQUIRED)
if (NOT ImGui_FOUND)
    message(STATUS "Getting ImGui from Github")
    include_dependency(ImGui https://github.com/ocornut/imgui/ docking)
else ()
    message(STATUS "Using local ImGui")
endif ()

add_subdirectory(
        ../firmware/components/common common
)
add_subdirectory(
        ../firmware/components/game game
)

add_executable(${PROJECT_NAME}
        MACOSX_BUNDLE
        main/main.cc
        main/sim/led_matrix.cc
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER dev.mars3142.${PROJECT_NAME}
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/CMake/Info.plist.in
)

target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_MAIN_USE_CALLBACKS)

target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        ImGui::ImGui
        GC::GAME
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        GC::COMMON
)

target_sources(${PROJECT_NAME} PRIVATE
        $<TARGET_PROPERTY:ImGui::ImGui,INTERFACE_SOURCES>
        ${CMAKE_CURRENT_SOURCE_DIR}/components/imgui/backends/imgui_impl_sdl3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/imgui/backends/imgui_impl_sdlrenderer3.cpp
)

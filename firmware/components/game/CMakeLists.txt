cmake_minimum_required(VERSION 3.30)

if (SDKCONFIG)
    message(STATUS "Building for ESP32")

    idf_component_register(SRCS
            INCLUDE_DIRS "include"
            PRIV_REQUIRES
                common
    )
else ()
    message(STATUS "Building for Desktop")

    project(game VERSION 1.0.0 DESCRIPTION "Game Logic for Geocache")

    add_library(${PROJECT_NAME} SHARED
            game.cpp
    )
    add_library(GC::GAME ALIAS game)

    set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
    target_include_directories(${PROJECT_NAME} PUBLIC include)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            GC::COMMON
    )
endif ()

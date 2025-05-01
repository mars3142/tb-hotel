cmake_minimum_required(VERSION 3.12)

set(IMGUI_SOURCE_DIR "${CMAKE_SOURCE_DIR}/components/imgui")

find_path(IMGUI_ROOT_DIR
        NAMES imgui.h
        HINTS "${IMGUI_SOURCE_DIR}"
        NO_DEFAULT_PATH
        DOC "Path to ImGui include directory (containing imgui.h)")

if (IMGUI_ROOT_DIR)
    file(GLOB IMGUI_CORE_SOURCES_RELATIVE
            RELATIVE "${IMGUI_ROOT_DIR}"
            ${IMGUI_ROOT_DIR}/imgui.cpp
            ${IMGUI_ROOT_DIR}/imgui_demo.cpp
            ${IMGUI_ROOT_DIR}/imgui_draw.cpp
            ${IMGUI_ROOT_DIR}/imgui_tables.cpp
            ${IMGUI_ROOT_DIR}/imgui_widgets.cpp
    )

    set(IMGUI_SOURCES "")
    foreach (src ${IMGUI_CORE_SOURCES_RELATIVE})
        list(APPEND IMGUI_SOURCES "${IMGUI_ROOT_DIR}/${src}")
    endforeach ()

    if (NOT TARGET ImGui::ImGui)
        add_library(imgui_internal INTERFACE)

        target_include_directories(imgui_internal INTERFACE
                "${IMGUI_ROOT_DIR}"
                "${IMGUI_ROOT_DIR}/backends"
        )
        target_sources(imgui_internal INTERFACE ${IMGUI_SOURCES})

        add_library(ImGui::ImGui ALIAS imgui_internal)

        message(STATUS "Found ImGui (internal): ${IMGUI_ROOT_DIR}. Providing ALIAS target ImGui::ImGui with backend includes.")
    endif ()

    set(IMGUI_ROOT_DIRS ${IMGUI_ROOT_DIR})
else ()
    message(STATUS "Could NOT find ImGui sources in ${IMGUI_SOURCE_DIR}")
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ImGui
        FOUND_VAR ImGui_FOUND
        REQUIRED_VARS IMGUI_ROOT_DIR IMGUI_SOURCES
)

mark_as_advanced(IMGUI_ROOT_DIR IMGUI_SOURCES)

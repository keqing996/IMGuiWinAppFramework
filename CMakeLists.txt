cmake_minimum_required(VERSION 3.22)

project (IMGuiWinAppFramework)

set (CMAKE_CXX_STANDARD 20)
set (LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/artifacts/${CMAKE_CXX_COMPILER_ID}/${CMAKE_BUILD_TYPE}/)

file (GLOB_RECURSE IMGUI_APP_FRAMEWORK_SRC ./src/*.cpp)
file (GLOB_RECURSE IMGUI_SRC
        ./thirdparty/imgui/imgui.cpp
        ./thirdparty/imgui/imgui_draw.cpp
        ./thirdparty/imgui/imgui_tables.cpp
        ./thirdparty/imgui/imgui_widgets.cpp
        ./thirdparty/imgui/backends/imgui_impl_win32.cpp
        ./thirdparty/imgui/backends/imgui_impl_dx11.cpp
        ./thirdparty/imgui/backends/imgui_impl_opengl3.cpp
        ./thirdparty/imgui/backends/imgui_impl_vulkan.cpp
)

add_library                 (imgui_win_app_framework STATIC ${IMGUI_APP_FRAMEWORK_SRC} ${IMGUI_SRC})
target_include_directories  (imgui_win_app_framework PUBLIC ./include/)
target_include_directories  (imgui_win_app_framework PUBLIC ./thirdparty/imgui/)
target_include_directories  (imgui_win_app_framework PUBLIC ./thirdparty/glad/)

option (ENABLE_IMGUI_WIN_APP_DEMO OFF)

if (ENABLE_IMGUI_WIN_APP_DEMO)
    add_executable          (demo_window ./example/Main.cpp)
    target_link_libraries   (demo_window PUBLIC imgui_win_app_framework)
endif ()



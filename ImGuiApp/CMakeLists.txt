
set (CMAKE_CXX_STANDARD 20)

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

find_package(Vulkan REQUIRED)

add_library                 (imgui_win_app_framework STATIC ${IMGUI_APP_FRAMEWORK_SRC} ${IMGUI_SRC})
target_include_directories  (imgui_win_app_framework PUBLIC ./include/)
target_include_directories  (imgui_win_app_framework PUBLIC ./thirdparty/imgui/)
target_include_directories  (imgui_win_app_framework PUBLIC ./thirdparty/glad/)
target_link_libraries       (imgui_win_app_framework PUBLIC native_window)
target_link_libraries       (imgui_win_app_framework PUBLIC Vulkan::Vulkan)
target_compile_definitions  (imgui_win_app_framework PUBLIC VK_USE_PLATFORM_WIN32_KHR)

option (ENABLE_IMGUI_WIN_APP_DEMO OFF)

if (ENABLE_IMGUI_WIN_APP_DEMO)
    add_executable          (demo_window example/ImGuiDemoWindow.cpp)
    target_link_libraries   (demo_window PUBLIC imgui_win_app_framework)
    set_target_properties   (demo_window PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif ()
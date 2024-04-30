cmake_minimum_required(VERSION 3.22)

project(IMGuiWinAppFramework)

set(CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE IMGUI_APP_SRC ./src/ImGuiWinApp/*.cpp)
file (GLOB_RECURSE NATIVE_ src/ImGuiWinApp/*.cpp)

add_executable(demo_window
        ${INFRA_SRC}
        ${FRAMEWORK_SRC}
        ./example/Main.cpp
)

target_include_directories(demo_window PUBLIC src/)


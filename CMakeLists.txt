cmake_minimum_required(VERSION 3.22)

project(IMGuiWinAppFramework)

set(CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE IMGUI_APP_FRAMEWORK_SRC ./src/*.cpp)
file (GLOB_RECURSE IMGUI_APP_FRAMEWORK_RES ./resource/*.rc)

add_executable(demo_window
        ${IMGUI_APP_FRAMEWORK_SRC}
        ${IMGUI_APP_FRAMEWORK_RES}
        ./example/Main.cpp
)

target_include_directories(demo_window PRIVATE
        ./src/
        ./thirdparty/CppInfra/include/
)


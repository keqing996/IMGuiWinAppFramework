cmake_minimum_required(VERSION 3.22)

project(IMGuiWinAppFramework)

set(CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE INFRA_SRC
        ./src/Infra/*.cpp
)

file (GLOB_RECURSE FRAMEWORK_SRC
        ./src/Framework/*.cpp
)

add_executable(demo_window
        ${INFRA_SRC}
        ${FRAMEWORK_SRC}
        ./example/Main.cpp
)

target_include_directories(demo_window PUBLIC src/)


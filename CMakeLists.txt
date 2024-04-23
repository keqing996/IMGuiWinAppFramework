cmake_minimum_required(VERSION 3.22)

project(IMGuiWinAppFramework)

set(CMAKE_CXX_STANDARD 20)
set(TARGET_NAME imguiwinappframework)

# helper lib
file (GLOB_RECURSE FRAMEWORK_SRC src/Infra/*.cpp)
add_library(${TARGET_NAME} STATIC ${FRAMEWORK_SRC})
target_include_directories(${TARGET_NAME} PUBLIC src/)

# test proj
# add_subdirectory()

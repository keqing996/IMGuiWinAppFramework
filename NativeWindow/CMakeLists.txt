

set (CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE NATIVE_WINDOW_SRC ./src/*.cpp)

add_library                 (native_window STATIC ${NATIVE_WINDOW_SRC})
target_include_directories  (native_window PUBLIC ./include/)

option (ENABLE_NATIVE_WINDOW_TEST OFF)

if (ENABLE_NATIVE_WINDOW_TEST)
    add_executable          (test_native_window test/NativeWindowTest.cpp)
    target_link_libraries   (test_native_window PUBLIC native_window)
    set_target_properties   (test_native_window PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif ()
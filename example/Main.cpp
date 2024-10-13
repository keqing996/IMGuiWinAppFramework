#include "ImApp/ImGuiWinApp.h"

int main()
{
    NativeWindow::Window window(800, 600, "TestOpenGL");
    while (true)
    {
        window.EventLoop();

        if (std::ranges::any_of(window.PopAllEvent(), [](const NativeWindow::WindowEvent& event) -> bool { return event.type == NativeWindow::WindowEvent::Type::Close; }))
            break;



        window.SwapBuffer();
    }

    return 0;
}
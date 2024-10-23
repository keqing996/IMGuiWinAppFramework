
#include <ImApp/Utility/WindowsInclude.h>
#include <ImApp/NativeWindow/Window.h>

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PWSTR lpCmdLine, int nCmdShow)
{
    NativeWindow::Window window(800, 600, "Test");

    while (true)
    {
        if (!window.EventLoop())
            break;
    }
}

#include <NativeWindow/WindowsInclude.h>
#include <NativeWindow/Window.h>

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PWSTR lpCmdLine, int nCmdShow)
{
    NativeWindow::Window window;
    window.Create(800, 600, "Test", NativeWindow::WindowStyle::DefaultStyle());

    while (true)
    {
        bool finish;
        window.EventLoop(&finish);
        if (finish)
            break;
    }

    return 0;
}